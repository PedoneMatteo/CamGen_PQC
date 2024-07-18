#if !defined(__CYGWIN__)
#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <net/if.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Port.hh>

#include "pcap_layer_factory.hh"

#include "loggers.hh"

static const char *_hexDigits = "0123456789ABCDEF";
static char *      _bin2hex(char *hex, size_t hlen, const char *bin, size_t blen) {
  const unsigned char *b, *e;
  char *               s;

  // sanity check
  if (hlen >= 0 && hlen < blen * 2)
    return NULL;

  b = (const unsigned char *)bin;
  e = b + blen - 1;
  s = hex + blen * 2;
  if (s < hex + hlen)
    *s = 0;
  for (; b <= e; e--) {
    *(--s) = _hexDigits[(*e) & 0xF];
    *(--s) = _hexDigits[(*e) >> 4];
  }
  return hex + blen * 2;
}

pcap_layer::pcap_layer(const std::string &p_type, const std::string &param)
  : layer(p_type), PORT(p_type.c_str()), _params(), _device(NULL), _pcap_h(-1), _time_key("pcap_layer::Handle_Fd_Event_Readable") {
  char                   error_buffer[PCAP_ERRBUF_SIZE];
  params::const_iterator it;
  std::string            nic;

  loggers::get_instance().log(">>> pcap_layer::pcap_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  // Prepare capture processing
  it = _params.find(params::nic);
  if ((it == _params.end()) || it->second.empty()) { // Use online capture
    loggers::get_instance().error("pcap_layer::pcap_layer: NIC name must be specified");
    return;
  }

  nic = _params[params::nic];
  {
    bpf_u_int32 net, mask; // ip address and subnet mask
    if (pcap_lookupnet(nic.c_str(), &net, &mask, error_buffer) != 0) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to fetch newtork address for device %s", nic.c_str());
    } else {
      loggers::get_instance().log("pcap_layer::pcap_layer: Device %s Network address: %d", nic.c_str(), net);
    }
  }
  // Open the device in promiscuous mode
  _device = pcap_open_live(nic.c_str(), 65536 /*64*1024*/, 1, 100, error_buffer); // TODO Replace hard coded values by pcap_layer::<constants>
  if (_device == NULL) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to open device %s", nic.c_str());
    return;
  } // else, continue
  // Set non-blocking flag for the polling procedure
  if (pcap_setnonblock(_device, 1, error_buffer) != 0) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to set blocking mode: %s", error_buffer);
  }
  // Retrieve the device file handler
  _pcap_h = pcap_get_selectable_fd(_device);
  if (_pcap_h == -1) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to get device handler");
  }

  // Setup filter
  std::string filter = "";
  std::string mac_src;
  it = _params.find(params::mac_src);
  if (it != _params.end()) { // Use online capture
    mac_src = it->second;
  } else {
    // Detect MAC address of NIC
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    nic.copy(ifr.ifr_name, sizeof(ifr.ifr_name));
    if (ioctl(_pcap_h, SIOCGIFHWADDR, &ifr) == -1) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to get device MAC address");
    } else {
      char buf[13];
      *_bin2hex(buf, sizeof(buf), ifr.ifr_hwaddr.sa_data, 6) = 0;
      mac_src                                                = buf;
      loggers::get_instance().user("pcap_layer::pcap_layer: local MAC is %s", mac_src.c_str());
      _params[params::mac_src] = mac_src;
    }
  }

  // Accept ITS broadcasted messages
  std::string mac_bc;
  it = _params.find(params::mac_bc);
  if (it != _params.end() && !it->second.empty()) {
    mac_bc = it->second;
  } else {
    mac_bc = "ffffffffffff";
  }

  if ((mac_bc == mac_src) || mac_src.empty()) {
    filter = "ether dst " + mac_bc;
  } else {
    filter = "( ether dst " + mac_bc + " or ether dst " + mac_src + " )";
  }

  if (!mac_src.empty()) {
    // Reject ITS messages sent by this component
    filter += " and not ether src " + mac_src;
  }

  // Add user defined filter
  it = _params.find(params::filter);
  if ((it != _params.end()) && !it->second.empty()) {
    filter += std::string(" ") + it->second;
  }
  // Log final PCAP filter
  loggers::get_instance().user("pcap_layer::pcap_layer: Filter: %s", filter.c_str());

  {
    struct bpf_program f = {0};
    if (pcap_compile(_device, &f, filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) != 0) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to compile PCAP filter");
    } else {
      if (pcap_setfilter(_device, &f) != 0) {
        loggers::get_instance().error("pcap_layer::pcap_layer: Failed to set PCAP filter");
      }
    }
    pcap_freecode(&f);
  }

  // Pass the device file handler to the polling procedure
  Handler_Add_Fd_Read(_pcap_h);
} // End of ctor

pcap_layer::~pcap_layer() {
  loggers::get_instance().log(">>> pcap_layer::~pcap_layer");

  if (_device != NULL) {
    pcap_close(_device);
  }
} // End of dtor

void pcap_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> pcap_layer::send_data: ", data);

  if (pcap_sendpacket(_device, static_cast<const unsigned char *>(data), data.lengthof()) == -1) {
    loggers::get_instance().error("pcap_layer::send_data: Failed to send packet: %s", pcap_geterr(_device));
  }
}

void pcap_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> pcap_layer::receive_data: Received %d bytes", data.lengthof());
  loggers::get_instance().log_to_hexa("Packet dump", data);

  // Pass the packet to the upper layers
  receive_to_all_layers(data, params);
}

void pcap_layer::Handle_Fd_Event_Readable(int fd) {
  // loggers::get_instance().log(">>> pcap_layer::Handle_Fd_Event_Readable: %d", fd);

  pcap_pkthdr * pkt_header;
  const u_char *pkt_data;
  int           result = pcap_next_ex(_device, &pkt_header, &pkt_data);
  if (result == 1) {               // Succeed
    if (pkt_header->caplen > 14) { // Reject too small packet
      loggers::get_instance().log("pcap_layer::Handle_Fd_Event_Readable: %ld - %ld - %ld - %d", pkt_header->ts.tv_sec, pkt_header->ts.tv_usec,
                                  pkt_header->ts.tv_sec * 1000 + static_cast<unsigned int>(pkt_header->ts.tv_usec / 1000), pkt_header->len);
      // Fill parameters from PCAP layer
      params params;
      params.insert(std::pair<std::string, std::string>(
        params::timestamp, std::to_string(pkt_header->ts.tv_sec * 1000 + static_cast<unsigned int>(pkt_header->ts.tv_usec / 1000)))); // Use milliseconds
      // Process the packet at this layer
      OCTETSTRING os(pkt_header->caplen, pkt_data);
      // loggers::get_instance().log_to_hexa("pcap_layer::Handle_Fd_Event_Readable: ", os);
      // TODO Case of caplen != len !!!
      float duration;
      loggers::get_instance().set_start_time(_time_key);
      this->receive_data(os, params); // TODO Check execution time for decoding operation
      loggers::get_instance().set_stop_time(_time_key, duration);
    }
  } // else, skip the packet
}

pcap_layer_factory pcap_layer_factory::_f;

#endif // !CYGWIN
