#if defined(__CYGWIN__)
#define _GNU_SOURCE
#endif
#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Port.hh>

#include "pcap_offline_layer_factory.hh"

#include "loggers.hh"

#include <pcap.h>

#ifdef __CYGWIN__
typedef struct {
  bpf_int32 tv_sec;  /* seconds */
  bpf_int32 tv_usec; /* microseconds */
} pcap_o_timeval;

typedef struct pcap_o_pkthdr {
  pcap_o_timeval ts;     /* time stamp */
  bpf_u_int32    caplen; /* length of portion present */
  bpf_u_int32    len;    /* length this packet (off wire) */
} pcap_o_pkthdr;
#else
typedef struct pcap_pkthdr pcap_o_pkthdr;
typedef struct timeval     pcap_o_timeval;
#endif

pcap_offline_layer::pcap_offline_layer(const std::string &p_type, const std::string &param)
  : layer(p_type), PORT(p_type.c_str()), _params(), _device(NULL), _running(FALSE), _time_key("pcap_offline_layer::Handle_Fd_Event_Readable") {
  loggers::get_instance().log(">>> pcap_offline_layer::pcap_offline_layer: %s, %s", p_type.c_str(), param.c_str());
  params::convert(_params, param);

  _o_params.insert(std::pair<std::string, std::string>(std::string("timestamp"), std::string()));

  char                   error_buffer[PCAP_ERRBUF_SIZE];
  params::const_iterator it;

  it        = _params.find(std::string("realtime"));
  _realtime = ((it != _params.end()) && !it->second.empty());

  it    = _params.find(std::string("loop"));
  _loop = ((it != _params.end()) && !it->second.empty());

  it = _params.find(std::string("file"));
  if ((it != _params.end()) && !it->second.empty()) {
    const std::string &file = it->second;
    _device                 = pcap_open_offline(file.c_str(), error_buffer);
    if (_device) {

      // Add user defined filter
      it = _params.find(params::filter);
      if ((it != _params.end()) && !it->second.empty()) {
        const std::string &filter = it->second;
        // Log final PCAP filter
        loggers::get_instance().user("pcap_offline_layer::pcap_offline_layer: Filter: %s", filter.c_str());
        struct bpf_program f = {0};
        if (pcap_compile(_device, &f, filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) != 0) {
          loggers::get_instance().error("pcap_offline_layer::pcap_offline_layer: Failed to compile PCAP filter");
        } else {
          if (pcap_setfilter(_device, &f) != 0) {
            loggers::get_instance().error("pcap_offline_layer::pcap_offline_layer: Failed to set PCAP filter");
          }
        }
        pcap_freecode(&f);
      }

      // create pipe and run thread
      if (pipe2(_fd, O_NONBLOCK) == -1) {
        loggers::get_instance().error("pcap_offline_layer::pcap_offline_layer: Failed to create a pipe: %s", ::strerror(errno));
      }
      // Pass the pipe handler to the polling procedure
      loggers::get_instance().log("pcap_offline_layer::pcap_offline_layer: Call handler with descriptor %d", _fd[0]);
      Handler_Add_Fd_Read(_fd[0]);
      // Create the offline reader thread
      _thread = new std::thread(&pcap_offline_layer::run, (void *)this);
      if (_thread == NULL) {
        loggers::get_instance().error("pcap_offline_layer::pcap_offline_layer: Failed to start offline thread");
      }
      while (_running == FALSE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
      // Thread was started
      loggers::get_instance().log("<<< pcap_offline_layer::pcap_offline_layer");
    }
  }
} // End of ctor

pcap_offline_layer::~pcap_offline_layer() {
  loggers::get_instance().log(">>> pcap_offline_layer::~pcap_offline_layer");

  if (_device != NULL) {
    if (_thread != NULL) {
      _running = FALSE;
      // Wait for the working thread to terminate
      _thread->join();
      loggers::get_instance().log("pcap_offline_layer::~pcap_offline_layer: Thread were stops");
      // Cleanup
      delete _thread;
      close(_fd[0]);
      close(_fd[1]);
    }
    pcap_close(_device);
  }
} // End of dtor

void *pcap_offline_layer::run(void *p_this) {
  pcap_offline_layer &p = *static_cast<pcap_offline_layer *>(p_this);
  return p.thread();
}

static long timeval_diff(const pcap_o_timeval &x, const pcap_o_timeval &y) {
  pcap_o_timeval z = y;
  /* Perform the carry for the later subtraction by updating y. */
  if (x.tv_usec < y.tv_usec) {
    int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
    z.tv_usec -= 1000000 * nsec;
    z.tv_sec += nsec;
  }
  if (x.tv_usec - z.tv_usec > 1000000) {
    int nsec = (x.tv_usec - z.tv_usec) / 1000000;
    z.tv_usec += 1000000 * nsec;
    z.tv_sec -= nsec;
  }

  return (x.tv_sec - z.tv_sec) * 1000 + ((x.tv_usec - z.tv_usec) / 1000);
}

void *pcap_offline_layer::thread() {
  pcap_o_pkthdr *pkt_header;
  pcap_o_pkthdr  lh;
  const u_char * pkt_data;
  unsigned char  pkt_count = 0;

  //  loggers::get_instance().log(">>> pcap_offline_layer::run");

  memset(&lh, 0, sizeof(lh));

  _running = TRUE;

  int                    delay = 1000;
  params::const_iterator it;
  it = _params.find(std::string("delay"));
  if (it != _params.cend()) {
    delay = std::stoi(it->second);
  }

  // wait a bit before sending first packet
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  while (_running) { // Loop while _running flag is up
    // get next frame
    int result = pcap_next_ex(_device, (struct pcap_pkthdr **)&pkt_header, &pkt_data);
    if (result == 2) {
      if (_loop) {

      } else {
        _running = FALSE;
        return NULL;
      }
    }
    if (_realtime) {
      // wait for next packet timestamp
      if (lh.ts.tv_sec | lh.ts.tv_usec) {
        long diff = timeval_diff(pkt_header->ts, lh.ts);
        if (diff > 0) {
          loggers::get_instance().log("<<< pcap_offline_layer::run: Wait %d msec", diff);
          std::this_thread::sleep_for(std::chrono::milliseconds(diff));
          loggers::get_instance().log("<<< pcap_offline_layer::run: Wait done");
        }
      }
    }
    while (_running && !_resume.try_lock()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    lh = *pkt_header;
#if 0
    {
      char buf[128];
      std::time_t t = pkt_header->ts.tv_sec;
      std::tm * pt = std::localtime( &t );
      t = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", pt);
      std::sprintf(buf+t, ".%06ld", pkt_header->ts.tv_usec);
      _o_params["timestamp"] = std::string(buf);
    }
#else
    _o_params["timestamp"] = std::to_string(pkt_header->ts.tv_usec);
#endif
    _o_data = OCTETSTRING(pkt_header->len, pkt_data);
    write(_fd[1], &pkt_count, 1);
    pkt_count++;
  }

  //  loggers::get_instance().log("<<< pcap_offline_layer::run");
  return NULL;
}

void pcap_offline_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log("pcap_offline_layer::send_data: Offline mode, operation was skipped");
}

void pcap_offline_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> pcap_offline_layer::receive_data: Received %d bytes", data.lengthof());
  loggers::get_instance().log_to_hexa("Packet dump", data);

  // Pass the packet to the upper layers
  receive_to_all_layers(data, params);
}

void pcap_offline_layer::Handle_Fd_Event_Readable(int fd) {
  // loggers::get_instance().log(">>> pcap_offline_layer::Handle_Fd_Event_Readable: %d", fd);
  char  c[2];
  float duration;
  // Process the packet at this layer
  loggers::get_instance().set_start_time(_time_key);
  this->receive_data(_o_data, _o_params);
  loggers::get_instance().set_stop_time(_time_key, duration);
  read(_fd[0], &c, 1);
  _resume.unlock();
}

pcap_offline_layer_factory pcap_offline_layer_factory::_f;
