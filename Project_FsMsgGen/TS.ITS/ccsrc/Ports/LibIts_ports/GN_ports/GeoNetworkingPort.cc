// This Test Port skeleton source file was generated by the
// TTCN-3 Compiler of the TTCN-3 Test Executor version CRL 113 200/5 R3A
// for U-ERICSSON\ethgry (ethgry@HU00078339) on Sat Aug 15 22:33:04 2015

// Copyright Ericsson Telecom AB 2000-2014

// You may modify this file. Complete the body of empty functions and
// add your member functions here.

#include "GeoNetworkingPort.hh"
#include "geonetworking_layer_factory.hh"
#include "loggers.hh"

namespace LibItsGeoNetworking__TestSystem {

  GeoNetworkingPort::GeoNetworkingPort(const char *par_port_name)
    : GeoNetworkingPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(nullptr), _time_key("GeoNetworkingPort::outgoing_send") {
    // Nothing to do
  } // End of constructor

  GeoNetworkingPort::~GeoNetworkingPort() {
    loggers::get_instance().log(">>> GeoNetworkingPort::~GeoNetworkingPort");

    if (_layer != nullptr) {
      delete _layer;
    }
  } // End of destructor

  void GeoNetworkingPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("GeoNetworkingPort::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
  }

  /*void GeoNetworkingPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void GeoNetworkingPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void GeoNetworkingPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void GeoNetworkingPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void GeoNetworkingPort::Handle_Timeout(double time_since_last_call) {}*/

  void GeoNetworkingPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> GeoNetworkingPort::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("GeoNetworkingPort::user_map: %s", it->second.c_str());
      // Setup parameters
      params::convert(_layer_params, it->second); // TODO This _layer_params seems to be useless
      // Create layer
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<geonetworking_layer *>(_layer) == nullptr) {
        loggers::get_instance().error("GeoNetworkingPort::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<geonetworking_layer *>(_layer)->add_upper_port(this);

    } else {
      loggers::get_instance().error("GeoNetworkingPort::user_map: No layers defined in configuration file");
    }
  } // End of user_map method

  void GeoNetworkingPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> GeoNetworkingPort::user_unmap: %s", system_port);

    // Reset layers
    if (_layer != nullptr) {
      delete _layer;
      _layer = nullptr;
    }
  } // End of user_unmap method

  void GeoNetworkingPort::user_start() { loggers::get_instance().log(">>> GeoNetworkingPort::user_start"); } // End of user_start method

  void GeoNetworkingPort::user_stop() { loggers::get_instance().log(">>> GeoNetworkingPort::user_stop"); } // End of user_stop method

  void GeoNetworkingPort::outgoing_send(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingReq &send_par) {
    loggers::get_instance().log_msg(">>> GeoNetworkingPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    params params;
    static_cast<geonetworking_layer *>(_layer)->sendMsg(send_par, params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void GeoNetworkingPort::receiveMsg(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> GeoNetworkingPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

} // namespace LibItsGeoNetworking__TestSystem
