#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <chrono>

#include "tcp_layer_factory.hh"

#include "loggers.hh"

tcp_layer::tcp_layer(const std::string & p_type, const std::string & param) : layer(p_type), SSL_Socket(), PORT(p_type.c_str()), _params(), _client_id{-1}, _time_key("tcp_layer::Handle_Fd_Event_Readable"), _reconnect_on_send{false} {
  loggers::get_instance().log(">>> tcp_layer::tcp_layer (1): %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  _params.log();

  init();                                                                                                                                                   }

tcp_layer::tcp_layer(const std::string & p_type, const params & param) : layer(p_type), SSL_Socket(), PORT(p_type.c_str()), _params(), _client_id{-1}, _time_key("tcp_layer::Handle_Fd_Event_Readable"), _reconnect_on_send{false} {
  loggers::get_instance().log(">>> tcp_layer::tcp_layer (2): %s", to_string().c_str());
  // Setup parameters
  _params = param;

  init();
}

void tcp_layer::init() {
  loggers::get_instance().log(">>> tcp_layer::init");

  set_socket_debugging(false);
  params::const_iterator it = _params.find(params::debug);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("debug"), "0"));
  } else if (it->second.compare("1") == 0) {
    set_socket_debugging(true);
  }
  it = _params.find(std::string("tcp_fragmented"));
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("tcp_fragmented"), "0"));
  }
  bool server_mode = false;
  it = _params.find(params::server_mode);
  if (it != _params.cend()) {
    server_mode = (1 == std::stoi(it->second));
  } else {
    _params.insert(std::pair<std::string, std::string>(std::string("server_mode"), "0"));
  }
  it = _params.find(params::server);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("server"), "127.0.0.1")); // TODO Try using params::server instead of std::string("server")
  }
  if (!parameter_set(params::server.c_str(), _params[params::server].c_str())) {
    loggers::get_instance().warning("tcp_layer::set_parameter: Unprocessed parameter: %s", params::server.c_str());
  }
  bool ssl_mode = false;
  it = _params.find(params::use_ssl);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("use_ssl"), "0"));
  } else if (it->second.compare("1") == 0) {
    _params.insert(std::pair<std::string, std::string>(std::string("use_ssl"), "1"));
    ssl_mode = true;
  }
  set_ssl_use_ssl(ssl_mode);
  it = _params.find(params::port);
  if (it == _params.cend()) {
    if (_params[params::use_ssl].compare("0") == 0) { // Use standard HTTP port
      _params.insert(std::pair<std::string, std::string>(std::string("port"), "80"));
    } else { // Use standard HTTPS port
      _params.insert(std::pair<std::string, std::string>(std::string("port"), "443"));
    }
  }
  if (!parameter_set(remote_port_name(), _params[params::port].c_str())) {
    loggers::get_instance().warning("tcp_layer::set_parameter: Unprocessed parameter: %s", params::port.c_str());
  }
  it = _params.find(params::local_port);
  if (it == _params.cend()) {
    if (_params[params::use_ssl].compare("0") == 0) { // Use standard HTTP local_port
      _params.insert(std::pair<std::string, std::string>(std::string("local_port"), "80"));
    } else { // Use standard HTTPS local_port
      _params.insert(std::pair<std::string, std::string>(std::string("local_port"), "443"));
    }
  }
  if (!parameter_set(local_port_name(), _params[params::local_port].c_str())) {
    loggers::get_instance().warning("tcp_layer::set_parameter: Unprocessed parameter: %s", params::local_port.c_str());
  }

  parameter_set(use_connection_ASPs_name(), (!server_mode) ? "yes" : "no");
  loggers::get_instance().warning("tcp_layer::set_parameter: Limit to one simultanneous accepted connection (server_backlog == 1");
  parameter_set(server_backlog_name(), "1"); // Limit to one simultanneous accepted connection 
  loggers::get_instance().log("tcp_layer::init: server_mode=%x", server_mode);
  set_server_mode(server_mode);
  if (server_mode) {
    parameter_set("serverPort", _params[params::local_port].c_str());
  }
  if (ssl_mode) { // Add certificate bundle
    // Check mutual authentication param
    _params.insert(std::pair<std::string, std::string>(std::string("mutual_tls"), "0"));
    parameter_set(ssl_verifycertificate_name(), "no");
    it = _params.find(params::mutual_auth);
    if (it == _params.cend()) {
      if (_params[params::mutual_auth].compare("1") == 0) { // Use mutual authentication
        parameter_set(ssl_verifycertificate_name(), "yes");
        _params.insert(std::pair<std::string, std::string>(std::string("mutual_tls"), "1"));
      }
    }
    // Set trusted CA file
    it = _params.find(params::mutual_auth);
    if (it == _params.cend()) {
      parameter_set(ssl_trustedCAlist_file_name(), it->second.c_str());
      _params.insert(std::pair<std::string, std::string>(std::string("mutual_tls"), it->second));
    } else {
      parameter_set(ssl_trustedCAlist_file_name(), "/usr/share/ca-certificates/mozilla/Amazon_Root_CA_1.crt");
    }
    // Set additional certificates
    //parameter_set(ssl_private_key_file_name(), "../certificates/out/privates/e5e11abad8003766e4a7b721afb175a189b5f4cc7046af9b0d8eaebb86f28c40_server_dsa.key.pem");
    //parameter_set(ssl_certificate_file_name(), "../certificates/out/certs/e5e11abad8003766e4a7b721afb175a189b5f4cc7046af9b0d8eaebb86f28c40_server_dsa.cert.pem");
  }
  set_ttcn_buffer_usercontrol(false);
  set_handle_half_close(true);

  map_user();

  parameter_set(client_TCP_reconnect_name(), "yes");

  if (server_mode == 0) {
    loggers::get_instance().log("tcp_layer::init: Establish connection: %s/%s", _params[params::server].c_str(), _params[params::port].c_str());
	  open_client_connection(_params[params::server].c_str(), _params[params::port].c_str(), NULL, NULL);
  }
}

tcp_layer::~tcp_layer() {
  loggers::get_instance().log(">>> tcp_layer::~tcp_layer: %d", _client_id);
  if (_client_id != -1) {
    remove_client(_client_id);
  }

  unmap_user();
}

void tcp_layer::Handle_Fd_Event(int fd, boolean is_readable, boolean is_writable, boolean is_error)
{
  loggers::get_instance().log(">>> tcp_layer::Handle_Fd_Event: %d", fd);
  Handle_Socket_Event(fd, is_readable, is_writable, is_error);
  log_debug("<<< tcp_layer::Handle_Fd_Event");
}

void tcp_layer::Handle_Timeout(double time_since_last_call)
{
  loggers::get_instance().log(">>> tcp_layer::Handle_Timeout: %f", time_since_last_call);
  Handle_Timeout_Event(time_since_last_call);
  loggers::get_instance().log("<<< tcp_layer::Handle_Timeout");
}

void tcp_layer::send_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log_msg(">>> tcp_layer::send_data: ", data);

  loggers::get_instance().log("tcp_layer::send_data: SSL mode: %x", get_ssl_use_ssl());
  loggers::get_instance().log("tcp_layer::send_data: server_mode: %s", _params[params::server_mode].c_str());
  loggers::get_instance().log("tcp_layer::send_data: peer_list_get_nr_of_peers: %d", peer_list_get_nr_of_peers());

  if ((_params[params::server_mode].compare("0") == 0) && (peer_list_get_nr_of_peers() == 0)) {
    // Reconnect (e.g. HTTP connection lost
    loggers::get_instance().log("tcp_layer::send_data: Re-establish connection: %s/%s", _params[params::server].c_str(), _params[params::port].c_str());
	  open_client_connection(_params[params::server].c_str(), _params[params::port].c_str(), NULL, NULL);
  }
  send_outgoing(static_cast<const unsigned char*>(data), data.lengthof(), _client_id);
}

void tcp_layer::receive_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log_msg(">>> tcp_layer::receive_data: ", data);

  receive_to_all_layers(data, params);
}

void tcp_layer::message_incoming(const unsigned char* message_buffer, int length, int client_id) {
  loggers::get_instance().log(">>> tcp_layer::message_incoming");
  loggers::get_instance().log_to_hexa("tcp_layer::message_incoming: ", message_buffer, length);

  float duration;
  loggers::get_instance().set_start_time(_time_key);
  OCTETSTRING data(length, message_buffer);
  params params;
  params.insert(std::pair<std::string, std::string>(
    std::string("timestamp"),
    std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())));
  this->receive_data(data, params); // TODO Check execution time for decoding operation
  loggers::get_instance().set_stop_time(_time_key, duration);
}

void tcp_layer::client_connection_opened(int p_client_id)
{
  loggers::get_instance().log(">>> tcp_layer::client_connection_opened: %d", p_client_id);
  _client_id = p_client_id;
}

bool tcp_layer::add_user_data(int p_client_id)
{
  loggers::get_instance().log(">>> tcp_layer::add_user_data: %d", p_client_id);
  if (_params[params::use_ssl].compare("0") == 0) {
    loggers::get_instance().log("tcp_layer::add_user_data: Non secured mode");
    return Abstract_Socket::add_user_data(p_client_id);
  }
  loggers::get_instance().log("tcp_layer::add_user_data: SSL mode");
  return SSL_Socket::add_user_data(p_client_id);
}

int tcp_layer::send_message_on_fd(int p_client_id, const unsigned char * message_buffer, int length_of_message)
{
  loggers::get_instance().log(">>> tcp_layer::send_message_on_fd: %d", p_client_id);

  if(get_user_data(p_client_id)) {
    loggers::get_instance().log("tcp_layer::send_message_on_fd: SSL mode");
    return SSL_Socket::send_message_on_fd(p_client_id, message_buffer, length_of_message);
  }

  loggers::get_instance().log("tcp_layer::send_message_on_fd: Non secured mode");
  return Abstract_Socket::send_message_on_fd(p_client_id, message_buffer, length_of_message);
}

int tcp_layer::send_message_on_nonblocking_fd(int p_client_id, const unsigned char * message_buffer, int length_of_message)
{
	loggers::get_instance().log(">>> tcp_layer::send_message_on_nonblocking_fd: %d", p_client_id);

  if(get_user_data(p_client_id)) {
    loggers::get_instance().log("tcp_layer::send_message_on_nonblocking_fd: SSL mode");
    return SSL_Socket::send_message_on_nonblocking_fd(p_client_id, message_buffer, length_of_message);
  }

  loggers::get_instance().log("tcp_layer::send_message_on_nonblocking_fd: Non secured mode");
  return Abstract_Socket::send_message_on_nonblocking_fd(p_client_id, message_buffer, length_of_message);
}

int tcp_layer::receive_message_on_fd(int p_client_id)
{
	loggers::get_instance().log(">>> tcp_layer::receive_message_on_fd: %d", p_client_id);

  if(get_user_data(p_client_id)) {
    // INFO: it is assumed that only SSL_Socket assigns user data to each peer
    loggers::get_instance().log("tcp_layer::receive_message_on_fd: SSL mode");
    return SSL_Socket::receive_message_on_fd(p_client_id);
  }

  loggers::get_instance().log("tcp_layer::receive_message_on_fd: Non secured mode");
  if (_params[std::string("tcp_fragmented")].compare("1") == 0) {
    sleep(5); // FIXME When HTTP paquet is fragmented into several TCP packets, a timer is required. This is a Q&D solution
  }
  return Abstract_Socket::receive_message_on_fd(p_client_id);
}

void tcp_layer::peer_disconnected(int p_client_id)
{
	loggers::get_instance().log(">>> tcp_layer::peer_disconnected: %d", p_client_id);

  Abstract_Socket::peer_disconnected(p_client_id);
	_client_id = -1;
}

tcp_layer_factory tcp_layer_factory::_f;

