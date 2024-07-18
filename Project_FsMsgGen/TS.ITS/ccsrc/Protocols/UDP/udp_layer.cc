#include <chrono>
#include <netdb.h>
#include <unistd.h>

#include <Port.hh>

#include "loggers.hh"

#include "udp_layer_factory.hh"

#include <net/if.h>
#include <sys/ioctl.h>

udp_layer::udp_layer(const std::string &p_type, const std::string &param)
  : layer(p_type), PORT(p_type.c_str()), _params(), _saddr{0}, _daddr{0}, _reuse_incoming_source_adddress(false), _fd(-1),
    _time_key("udp_layer::Handle_Fd_Event_Readable") {
  loggers::get_instance().log(">>> udp_layer::udp_layer (1): %s, %s", to_string().c_str(), param.c_str());

  // Setup parameters
  params::convert(_params, param);

  init();
}

udp_layer::udp_layer(const std::string &p_type, const params &param)
  : layer(p_type), PORT(p_type.c_str()), _params(), _saddr{0}, _daddr{0}, _reuse_incoming_source_adddress(false), _fd(-1),
    _time_key("udp_layer::Handle_Fd_Event_Readable") {
  loggers::get_instance().log(">>> udp_layer::udp_layer (2): %s", to_string().c_str());

  // Setup parameters
  _params = param;

  init();
}

void udp_layer::init() {
  loggers::get_instance().log(">>> udp_layer::init");

  params::const_iterator it = _params.find("src_ip");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("src_ip"), "127.0.0.1"));
  }
  it = _params.find("src_port");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("src_port"), "0")); // Dynamic binding requested
  }
  it = _params.find("dst_ip");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("dst_ip"), "127.0.0.1"));
  }
  it = _params.find("dst_port");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("dst_port"), "12345"));
  }
  it = _params.find("reuse_incoming_source_adddress");
  if (it != _params.cend()) {
    _reuse_incoming_source_adddress = (boolean)(it->second.compare("1") == 0);
  }
  loggers::get_instance().log("udp_layer::init: _reuse_incoming_source_adddress: %d", _reuse_incoming_source_adddress);

  // Initialize the socket
  _saddr.sin_family      = AF_INET;
  _saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  loggers::get_instance().log("udp_layer::init: Port to listen=%d", std::atoi(_params["src_port"].c_str()));
  _saddr.sin_port = htons(std::atoi(_params["src_port"].c_str()));
  // Create socket
  _fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_fd == -1) {
    loggers::get_instance().error("udp_layer::init: Failed to create socket");
  }
  loggers::get_instance().log("udp_layer::init: socket id: %d", _fd);
  int reuse = 1;
  if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    loggers::get_instance().warning("udp_layer::init: Failed to set SO_REUSEADDR");
  }
  // Bind it
  /*struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth1");
  if (setsockopt(_fd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr.ifr_name, strlen(ifr.ifr_name)) < 0) {
    close();
    loggers::get_instance().error("udp_layer::init: Failed to bind socket to %s", ifr.ifr_name);
  }
  loggers::get_instance().log("udp_layer::init: Bound to device %s", ifr.ifr_name);*/
  if (::bind(_fd, (struct sockaddr *)&_saddr, sizeof(_saddr)) < 0) {
    close();
    loggers::get_instance().error("udp_layer::init: Failed to bind socket");
  }
  loggers::get_instance().log("udp_layer::init: Bound on port %s", _params["src_port"].c_str());
  // Pass the device file handler to the polling procedure
  Handler_Add_Fd_Read(_fd);

  _daddr.sin_family      = AF_INET;
  _daddr.sin_addr.s_addr = htonl(get_host_id(_params["dst_ip"]));
  _daddr.sin_port        = htons(std::atoi(_params["dst_port"].c_str()));
}

udp_layer::~udp_layer() {
  loggers::get_instance().log(">>> udp_layer::~udp_layer");

  close();
}

void udp_layer::close() {
  loggers::get_instance().log(">>> udp_layer::close: %d", _fd);

  if (_fd != -1) {
    ::close(_fd);
    _fd = -1;
  }
}

void udp_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> udp_layer::send_data: %d", _fd);
  loggers::get_instance().log_msg(">>> udp_layer::send_data: ", data);

  int result = ::sendto(_fd, (const char *)static_cast<const unsigned char *>(data), data.lengthof(), 0, (struct sockaddr *)&_daddr, sizeof(_daddr));
  loggers::get_instance().log("udp_layer::send_data: #bytes sent: %d to %s:%d", result, ::inet_ntoa(_daddr.sin_addr), ntohs(_daddr.sin_port));
}

void udp_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> udp_layer::receive_data: ", data);

  receive_to_all_layers(data, params);
}

void udp_layer::Handle_Fd_Event_Readable(int fd) {
  loggers::get_instance().log(">>> udp_layer::Handle_Fd_Event_Readable: %d", fd);

  unsigned char              buffer[3072] = {0};
  struct sockaddr_in         from         = {0};
  socklen_t                  len          = sizeof(struct sockaddr_in); // Length of sender's address
  params                     params;
  std::vector<unsigned char> acc;
  int                        result = ::recvfrom(fd, buffer, 3072, 0, (struct sockaddr *)&from, &len);
  loggers::get_instance().log("udp_layer::Handle_Fd_Event_Readable: src_port = %s:%d, payload length = %d, errno = %d", ::inet_ntoa(from.sin_addr),
                              ntohs(from.sin_port), result, errno);
  while ((result == 3072) && (errno == 0)) {
    std::copy((unsigned char *)buffer, (unsigned char *)((unsigned char *)buffer + result), std::back_inserter(acc));
    result = ::recvfrom(fd, buffer, 3072, 0, (struct sockaddr *)&from, &len);
    loggers::get_instance().log("udp_layer::Handle_Fd_Event_Readable: src_port = %s:%d, payload length = %d, errno = %d", ::inet_ntoa(from.sin_addr),
                                ntohs(from.sin_port), result, errno);
  } // End of 'while' statement
  if (errno < 0) {
    loggers::get_instance().warning("udp_layer::Handle_Fd_Event_Readable: Failed to read data, discard them: errno=%d", errno);
    return;
  } else {
    std::copy((unsigned char *)buffer, (unsigned char *)((unsigned char *)buffer + result), std::back_inserter(acc));
    if (_reuse_incoming_source_adddress) { // Reuse the incoming address/port for sending
      memcpy((void *)&_daddr, (const void *)&from, sizeof(struct sockaddr_in));
      loggers::get_instance().log("udp_layer::Handle_Fd_Event_Readable: New _daddr: %s:%d", ::inet_ntoa(_daddr.sin_addr), ntohs(_daddr.sin_port));
    }
  }
  params.insert(std::pair<std::string, std::string>(
    std::string("timestamp"),
    std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())));

  float duration;
  loggers::get_instance().set_start_time(_time_key);
  OCTETSTRING os(acc.size(), acc.data());
  receive_data(os, params); // TODO Check execution time for decoding operation
  loggers::get_instance().set_stop_time(_time_key, duration);
}

unsigned long udp_layer::get_host_id(const std::string &p_host_name) {
  loggers::get_instance().log(">>> udp_layer::get_host_id");

  if (p_host_name.empty()) {
    loggers::get_instance().warning("udp_layer::get_host_id: Wrong parameter");
    return INADDR_ANY;
  }

  unsigned long ip_addr = 0;
  if (p_host_name.compare("255.255.255.255") == 0) {
    loggers::get_instance().warning("udp_layer::get_host_id: Host ip is 255.255.255.255");
    ip_addr = 0xffffffff;
  } else {
    in_addr_t addr = ::inet_addr(p_host_name.c_str());
    if (addr != (in_addr_t)-1) { // host name in XX:XX:XX:XX form
      ip_addr = addr;
    } else { // host name in domain.com form
      struct hostent *hptr;
      if ((hptr = ::gethostbyname(p_host_name.c_str())) == 0) {
        close();
        loggers::get_instance().error("udp_layer::get_host_id: Invalid host name: %s", p_host_name.c_str());
      }
      ip_addr = *((unsigned long *)hptr->h_addr_list[0]);
    }
  }

  loggers::get_instance().log("udp_layer::get_host_id: Host name: %s, Host address: %u", p_host_name.c_str(), ip_addr);

  return htonl(ip_addr);
}

udp_layer_factory udp_layer_factory::_f;
