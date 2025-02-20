// This Test Port skeleton source file was generated by the
// TTCN-3 Compiler of the TTCN-3 Test Executor version CRL 113 200/5 R3A
// for U-ERICSSON\ethgry (ethgry@HU00078339) on Fri Aug 14 16:32:05 2015

// Copyright Ericsson Telecom AB 2000-2014

// You may modify this file. Complete the body of empty functions and
// add your member functions here.

#include "CamPort.hh"
#include "cam_layer_factory.hh"
#include "loggers.hh"

namespace LibItsCam__TestSystem {

  CamPort::CamPort(const char *par_port_name)
    : CamPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(NULL), _time_key("CamPort::outgoing_send") {}

  CamPort::~CamPort() {
    if (_layer != NULL) {
      delete _layer;
    }
  }

  void CamPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("CamPort::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
  }

  /*void CamPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void CamPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void CamPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void CamPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void CamPort::Handle_Timeout(double time_since_last_call) {}*/

  void CamPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> CamPort::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("CamPort::user_map: %s", it->second.c_str());
      // Setup parameters
      params::convert(_layer_params, it->second); // TODO This _layer_params seems to be useless
      // Create layer
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<cam_layer *>(_layer) == NULL) {
        loggers::get_instance().error("CamPort::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<cam_layer *>(_layer)->add_upper_port(this);
    } else {
      loggers::get_instance().error("CamPort::user_map: No layers defined in configuration file");
    }
  }

  void CamPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> CamPort::user_unmap: %s", system_port);
    if (_layer != NULL) {
      delete _layer;
      _layer = NULL;
    }
  }

  void CamPort::user_start() {}

  void CamPort::user_stop() {}

  void CamPort::outgoing_send(const LibItsCam__TypesAndValues::CamReq &send_par) {
    loggers::get_instance().log_msg(">>> CamPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    params params;
    static_cast<cam_layer *>(_layer)->sendMsg(send_par, params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void CamPort::receiveMsg(const LibItsCam__TypesAndValues::CamInd &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> CamPort::receive_msg: ", p_ind);

    incoming_message(p_ind);
  }

} // namespace LibItsCam__TestSystem
