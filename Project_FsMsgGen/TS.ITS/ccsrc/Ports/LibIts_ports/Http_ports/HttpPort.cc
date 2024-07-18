#include "HttpPort.hh"
#include "http_layer_factory.hh"
#include "loggers.hh"

#include "LibItsHttp_TypesAndValues.hh"

namespace LibItsHttp__TestSystem {

  HttpPort::HttpPort(const char *par_port_name)
    : HttpPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(nullptr), _time_key("HttpPort::outgoing_send") {
    // Nothing to do
  } // End of constructor

  HttpPort::~HttpPort() {
    loggers::get_instance().log(">>> HttpPort::~HttpPort");

    if (_layer != nullptr) {
      delete _layer;
    }
  } // End of destructor

  void HttpPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("HttpPort::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
  }

  /*void HttpPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void HttpPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void HttpPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void HttpPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void HttpPort::Handle_Timeout(double time_since_last_call) {}*/

  void HttpPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> HttpPort::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("HttpPort::user_map: %s", it->second.c_str());
      // Setup parameters
      params::convert(_layer_params, it->second); // TODO This _layer_params seems to be useless
      // Create layer
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<http_layer *>(_layer) == nullptr) {
        loggers::get_instance().error("HttpPort::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<http_layer *>(_layer)->add_upper_port(this);

    } else {
      loggers::get_instance().error("HttpPort::user_map: No layers defined in configuration file");
    }
  } // End of user_map method

  void HttpPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> HttpPort::user_unmap: %s", system_port);

    // Reset layers
    if (_layer != nullptr) {
      delete _layer;
      _layer = nullptr;
    }
  } // End of user_unmap method

  void HttpPort::user_start() { loggers::get_instance().log(">>> HttpPort::user_start"); } // End of user_start method

  void HttpPort::user_stop() { loggers::get_instance().log(">>> HttpPort::user_stop"); } // End of user_stop method

  void HttpPort::outgoing_send(const LibItsHttp__TypesAndValues::HttpMessage &send_par) {
    loggers::get_instance().log_msg(">>> HttpPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    params params;
    static_cast<http_layer *>(_layer)->sendMsg(send_par, params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void HttpPort::receiveMsg(const LibItsHttp__TypesAndValues::HttpMessage &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> HttpPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

} // namespace LibItsHttp__TestSystem
