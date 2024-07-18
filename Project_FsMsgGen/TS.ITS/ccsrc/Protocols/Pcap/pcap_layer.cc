#if 0
#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Port.hh>

#include "pcap_layer_factory.hh"

typedef struct pcap_pkthdr pcap_o_pkthdr;
typedef struct timeval pcap_o_timeval;

#include "loggers.hh"

pcap_layer::pcap_layer(const std::string& p_type, const std::string& param) : layer(p_type), PORT(p_type.c_str()), _params(), _device(NULL), _pcap_h(-1), _thread(NULL), _running(FALSE), _resume(), _sent_file(NULL), _time_key("pcap_layer::Handle_Fd_Event_Readable") {
  bool online = false;
  loggers::get_instance().log(">>> pcap_layer::pcap_layer: %s, %s", to_string().c_str(), param.c_str());
  _fd[0] = -1; _fd[1] = -1;
  // Setup parameters
  params::convert(_params, param);
  //_params.log();
  // Prepare capture processing
  char error_buffer[PCAP_ERRBUF_SIZE];
  params::const_iterator it = _params.find(params::nic); 
  if ((it != _params.end()) && !it->second.empty()) { // Use online capture
    // Fetch the network address and network mask
    bpf_u_int32 mask; // subnet mask
    bpf_u_int32 net; // ip address
    std::string nic;
    online = true;
    nic = _params[params::nic];
    if (pcap_lookupnet(nic.c_str(), &net, &mask, error_buffer) != 0) {
      loggers::get_instance().error("pcap_layer::pcap_layer: pcap_layer::pcap_layer: Failed to fetch newtork address for device %s", nic.c_str());
    }
    loggers::get_instance().log("pcap_layer::pcap_layer: Device %s Network address: %d", nic.c_str(), net);
    // Open the device
    _device = pcap_open_live(nic.c_str(), 65536, 1, 1000, error_buffer); // TODO Replace hard coded values by pcap_layer::<constants>
    if (_device == NULL) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to open device %s", nic.c_str());
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
  } else {
    // Check file name
    it = _params.find(std::string("file"));
    if ((it != _params.cend()) && !it->second.empty()) { // Use offline capture
      struct stat s = {0};
      if ((stat(_params["file"].c_str(), &s) != 0) || !S_ISREG(s.st_mode)) {
        loggers::get_instance().error("pcap_layer::pcap_layer: Failed to acces PCAP file %s", _params["file"].c_str());
      }
      // File exist, open it
      _device = pcap_open_offline(_params["file"].c_str(), error_buffer);
      if (_device == NULL) {
        loggers::get_instance().error("pcap_layer::pcap_layer: Failed to open PCAP file %s", error_buffer);	
      } // else, continue
      // Create the dump file for the sent packet based on the openned file name and the current time in milliseconds
      it =  _params.find("save_mode");
      if ((it != _params.cend()) && (it->second.compare("1") == 0)) {
        unsigned long ms = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
        std::string ext("_" + std::to_string(ms));
        int i = _params["file"].find(".pcap");
        if (i > 0) {
          std::string f(_params["file"].substr(0, i) + ext + ".pcap");
          loggers::get_instance().log("pcap_layer::pcap_layer: Save file name: %s", f.c_str());
          if ((_sent_file = pcap_dump_open(_device, f.c_str())) == NULL) {
            loggers::get_instance().warning("pcap_layer::pcap_layer: Failed to open save file %s", f.c_str());
          }
        }
      } // else, nothing to do
    } else {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to open PCAP file %s", error_buffer);
    }
  }

  // Setup filter
  std::string filter = "";
  it = _params.find(params::mac_src);
  if (it == _params.end()) { // Not found
    loggers::get_instance().error("pcap_layer::pcap_layer: mac_src parameter not found, cannot continue");
  } else {
    // Reject ITS messages sent by this component
    filter = "not ether src " + _params[params::mac_src]; 
    // Accept ITS broadcasted to this componenet
    filter += " and (ether dst " + _params[params::mac_src];
    // Accept ITS broadcasted messages
    it = _params.find(params::mac_bc);
    if ((it != _params.end()) && !it->second.empty()) {
      filter += " or ether dst " + it->second + ")";
    } else {
      filter += " or ether dst ffffffffffff) ";
    }
    // Add user defined filter
    it = _params.find(params::filter);
    if ((it != _params.end()) && !it->second.empty()) {
      filter += _params["filter"];
    } // else nothing to do
  }
  // Log final PCAP filter
  loggers::get_instance().user("pcap_layer::pcap_layer: Filter: %s", filter.c_str());
  if (!filter.empty()) {
    struct bpf_program f = {0};
    if (pcap_compile(_device, &f, filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) != 0) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to compile PCAP filter");
    }
    if (pcap_setfilter(_device, &f) != 0) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to set PCAP filter");
    }
    pcap_freecode(&f);
  }

  // Pass the device file handler to the polling procedure
  if (_pcap_h != -1) { // Live capture
    Handler_Add_Fd_Read(_pcap_h);
  } else { // Offline capture or cygwin
    // Create a pipe
    if (pipe2(_fd, O_NONBLOCK) == -1) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to create a pipe: %s", ::strerror(errno));
    }
    if(online){
      _pcap_h = _fd[0];
    }
    // Pass the pipe handler to the polling procedure
    loggers::get_instance().log("pcap_layer::pcap_layer: Call handler with descriptor %d", _fd[0]);
    Handler_Add_Fd_Read(_fd[0]);
    // Create the offline reader thread
    _thread = new std::thread(&pcap_layer::run, (void *)this);
    if (_thread == NULL) {
      loggers::get_instance().error("pcap_layer::pcap_layer: Failed to start offline thread");
    }
    // Start it to dispatch packet to a pipe
    while (_running == FALSE) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    // Thread was started
    loggers::get_instance().log("<<< pcap_layer::pcap_layer");
  }
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
    if (_sent_file != NULL) {
      pcap_dump_close(_sent_file);
    }
    pcap_close(_device);
  }
} // End of dtor

void* pcap_layer::run(void* p_this) {
  pcap_layer& p = *static_cast<pcap_layer *>(p_this);
  return p.thread();
}

void* pcap_layer::thread() {
  loggers::get_instance().log(">>> pcap_layer::run");

  // Wait a little bit before to start sending packet
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  params::const_iterator it = _params.find("frame_offset");
  if ((it != _params.cend()) && (it->second.compare("0") != 0)) {
    // TODO Try t use PCAP filter to start directly to the correct frame offset
    /*try {
      unsigned int offset = std::stoul(str_dec, &s);
      // Skip frames
      struct pcap_pkthdr *pkt_header;
      const u_char *pkt_data;
      int result = pcap_next_ex(_device, &pkt_header, &pkt_data);
      if (result == 1) { // Succeed
      }
      }
      catch (invalid_argument& i) {
      }
      catch (out_of_range& o) {
      }*/
  }
  // Let's go
  _running = TRUE;
  while (_running) { // Loop while _running flag is up
    if (_resume.try_lock() == TRUE) { // Previous packet was consumed, lock for the next one
      write(_fd[1], "\n", 1); // Any character will do the job
    } else { // not ready yet
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  
  loggers::get_instance().log("<<< pcap_layer::run");
  return NULL;
}

void pcap_layer::send_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log_msg(">>> pcap_layer::send_data: ", data);

  if (_pcap_h != -1) { // Check if offline mode is used
    if (pcap_sendpacket(_device, static_cast<const unsigned char *>(data), data.lengthof()) == -1) {
      loggers::get_instance().error("pcap_layer::send_data: Failed to send packet: %s", pcap_geterr(_device));
    }
  } else if (_sent_file != NULL) {
    struct pcap_pkthdr  hdr;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    hdr.ts.tv_sec = ms.count() / 1000;
    hdr.ts.tv_usec = (ms.count() % 1000) * 1000;
    hdr.caplen = data.lengthof();
    hdr.len = hdr.caplen;
    pcap_dump((u_char *)_sent_file, &hdr, static_cast<const unsigned char *>(data));
  } else {
    loggers::get_instance().log("pcap_layer::send_data: Offline mode, operation was skipped");
  }
}

void pcap_layer::receive_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log(">>> pcap_layer::receive_data: Received %d bytes", data.lengthof());
  loggers::get_instance().log_to_hexa("Packet dump", data);
  
  // Pass the packet to the upper layers
  receive_to_all_layers(data, params);
}

void pcap_layer::Handle_Fd_Event_Readable(int fd) {
  //loggers::get_instance().log(">>> pcap_layer::Handle_Fd_Event_Readable: %d", fd);
  
  pcap_o_pkthdr *pkt_header;
  const u_char *pkt_data;
  int result = pcap_next_ex(_device, (struct pcap_pkthdr**)&pkt_header, &pkt_data);
  if (result == 1) { // Succeed
    if (pkt_header->caplen > 14) { // Reject too small packet
      //loggers::get_instance().log("pcap_layer::Handle_Fd_Event_Readable: %ld - % ld - %d", pkt_header->ts.tv_sec, pkt_header->ts.tv_usec, pkt_header->len);
      // Fill parameters from PCAP layer
      params params;
      params.insert(std::pair<std::string, std::string>(params::timestamp, std::to_string(pkt_header->ts.tv_sec * 1000 + static_cast<int>(pkt_header->ts.tv_usec / 1000)))); // Use milliseconds
      // Process the packet at this layer
      OCTETSTRING os(pkt_header->caplen, pkt_data);
      //loggers::get_instance().log_to_hexa("pcap_layer::Handle_Fd_Event_Readable: ", os);
      // TODO Case of caplen != len !!!
      float duration;
      loggers::get_instance().set_start_time(_time_key);
      this->receive_data(os, params); // TODO Check execution time for decoding operation
      loggers::get_instance().set_stop_time(_time_key, duration);
    }
  } // else, skip the packet
  // Specific to offline mode
  if (_fd[0] != -1) { // Check if offline mode is used
    //loggers::get_instance().log("pcap_layer::Handle_Fd_Event_Readable: Read pipe");
    char c[2];
    read(_fd[0], &c, 1);
    if (result == -2) { // End of file, therminate worker thread
      _running = FALSE;
    }
      //loggers::get_instance().log("pcap_layer::Handle_Fd_Event_Readable: pcap_next_ex failed: result=%d", result);
    _resume.unlock();
  } // else, nothing to do
}

pcap_layer_factory pcap_layer_factory::_f;

#endif // !CYGWIN
