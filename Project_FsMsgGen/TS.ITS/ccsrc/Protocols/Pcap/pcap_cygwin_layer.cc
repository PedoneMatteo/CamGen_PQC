#if defined(__CYGWIN__)

#define _GNU_SOURCE

#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

#include <Port.hh>

#include "pcap_layer_factory.hh"

#include "loggers.hh"

#include <pcap.h>

typedef struct {
  bpf_int32 tv_sec;  /* seconds */
  bpf_int32 tv_usec; /* microseconds */
} pcap_o_timeval;

typedef struct pcap_o_pkthdr {
  pcap_o_timeval ts;     /* time stamp */
  bpf_u_int32    caplen; /* length of portion present */
  bpf_u_int32    len;    /* length this packet (off wire) */
} pcap_o_pkthdr;

extern "C" int pcap_oid_get_request(pcap_t *p, bpf_u_int32 oid, void *data, size_t *lenp);

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
  : layer(p_type), PORT(p_type.c_str()), _params(), _device(NULL), _running(FALSE), _time_key("pcap_layer::Handle_Fd_Event_Readable") {
  loggers::get_instance().log(">>> pcap_layer::pcap_layer: %s, %s", p_type.c_str(), param.c_str());
  params::convert(_params, param);

  char                   error_buffer[PCAP_ERRBUF_SIZE];
  params::const_iterator it;
  std::string            nic;  // network interface name
  bpf_u_int32            mask; // subnet mask
  bpf_u_int32            net;  // ip address

  it = _params.find(params::nic);
  if ((it == _params.end()) || it->second.empty()) {
    loggers::get_instance().error("pcap_layer::pcap_layer: NIC must be specified");
    return;
  }

  nic = std::string("\\Device\\NPF_{") + it->second + "}";

  if (pcap_lookupnet(nic.c_str(), &net, &mask, error_buffer) != 0) {
    loggers::get_instance().error("pcap_layer::pcap_layer: pcap_layer::pcap_layer: Failed to fetch newtork address for device %s", nic.c_str());
  }
  loggers::get_instance().log("pcap_layer::pcap_layer: Device %s Network address: %d", nic.c_str(), net);

  // Open the device
  _device = pcap_open_live(nic.c_str(), 65536 /*64*1024*/, 1, 100, error_buffer);
  if (_device == NULL) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to open device %s", nic.c_str());
    return;
  }

  // Setup filter
  std::string filter = "";
  std::string mac_src;
  it = _params.find(params::mac_src);
  if (it != _params.end() && !it->second.empty()) {
    mac_src = it->second;
  } else {
    // Not found
    // determine it automatically
#ifndef OID_802_3_CURRENT_ADDRESS
#define OID_802_3_CURRENT_ADDRESS 0x01010102
#endif
    char   mac[6] = {0};
    size_t l      = sizeof(mac);
    pcap_oid_get_request(_device, OID_802_3_CURRENT_ADDRESS, mac, &l);
    char buf[13];
    *_bin2hex(buf, sizeof(buf), mac, 6) = 0;
    mac_src                             = buf;
    loggers::get_instance().user("pcap_layer::pcap_layer: local MAC is %s", mac_src.c_str());
    _params[params::mac_src] = mac_src;
  }

  std::string mac_bc;
  it = _params.find(params::mac_bc);
  if (it != _params.end() && !it->second.empty())
    mac_bc = it->second;
  else
    mac_bc = "ffffffffffff";

  if (mac_bc == mac_src || mac_src.empty())
    filter = "ether dst " + mac_bc;
  else
    filter = "( ether dst " + mac_bc + " or ether dst " + mac_src + " )";

  if (!mac_src.empty())
    // Reject ITS messages sent by this component
    filter += " and not ether src " + mac_src;

  // Add user defined filter
  it = _params.find(params::filter);
  if ((it != _params.end()) && !it->second.empty()) {
    filter += std::string(" ") + it->second;
  }

  // Log final PCAP filter
  loggers::get_instance().user("pcap_layer::pcap_layer: Filter: %s", filter.c_str());

  // setup filter
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

  _o_params.insert(std::pair<std::string, std::string>(std::string("timestamp"), std::string()));

  // create pipe and run thread
  if (pipe2(_fd, O_NONBLOCK) == -1) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to create a pipe: %s", ::strerror(errno));
  }
  // Pass the pipe handler to the polling procedure
  loggers::get_instance().log("pcap_layer::pcap_layer: Call handler with descriptor %d", _fd[0]);
  Handler_Add_Fd_Read(_fd[0]);

  // Create the reader thread
  _thread = new std::thread(&pcap_layer::run, (void *)this);
  if (_thread == NULL) {
    loggers::get_instance().error("pcap_layer::pcap_layer: Failed to start offline thread");
  }
  while (_running == FALSE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  // Thread was started
  loggers::get_instance().log("<<< pcap_layer::pcap_layer");
} // End of ctor

pcap_layer::~pcap_layer() {
  loggers::get_instance().log(">>> pcap_layer::~pcap_layer");

  if (_device != NULL) {
    if (_thread != NULL) {
      _running = FALSE;
      // Wait for the working thread to terminate
      _thread->join();
      loggers::get_instance().log("pcap_layer::~pcap_layer: Thread were stops");
      // Cleanup
      delete _thread;
      close(_fd[0]);
      close(_fd[1]);
    }
    pcap_close(_device);
  }
} // End of dtor

void *pcap_layer::run(void *p_this) {
  pcap_layer &p = *static_cast<pcap_layer *>(p_this);
  return p.thread();
}

void *pcap_layer::thread() {
  pcap_o_pkthdr *pkt_header;
  const u_char * pkt_data;
  unsigned char  pkt_count = 0;

  //  loggers::get_instance().log(">>> pcap_layer::run");

  _running = TRUE;

  // wait a bit before sending first packet
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  while (_running) { // Loop while _running flag is up
    // get next frame
    int result = pcap_next_ex(_device, (struct pcap_pkthdr **)&pkt_header, &pkt_data);
    if (result == 0) {
      continue;
    }
    if (result < 0) {
      //      loggers::get_instance().log("<<< pcap_layer::run: error %s", pcap_geterr(_device));
      break;
    }

    while (_running && !_resume.try_lock()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
#if 0
    {
      char buf[128];
      std::time_t t = pkt_header->ts.tv_sec;
      std::tm * pt = std::localtime( &t );
      t = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", pt);
      std::sprintf(buf+t, "%ld", pkt_header->ts.tv_sec * 1000 + static_cast<int>(pkt_header->ts.tv_usec / 1000)); // Use milliseconds
      _o_params[params::timestamp] = std::string(buf);
    }
#else
    _o_params[params::timestamp] = std::to_string(pkt_header->ts.tv_sec * 1000 + static_cast<int>(pkt_header->ts.tv_usec / 1000)); // Use milliseconds
#endif
    _o_data = OCTETSTRING(pkt_header->len, pkt_data);
    write(_fd[1], &pkt_count, 1);
    pkt_count++;
  }
  _running = FALSE;
  //  loggers::get_instance().log("<<< pcap_layer::run");
  return NULL;
}

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
  char  c[2];
  float duration;
  loggers::get_instance().set_start_time(_time_key);
  this->receive_data(_o_data, _o_params);
  loggers::get_instance().set_stop_time(_time_key, duration);
  read(_fd[0], &c, 1);
  _resume.unlock();
}

pcap_layer_factory pcap_layer_factory::_f;

#endif //__CYGWIN__
