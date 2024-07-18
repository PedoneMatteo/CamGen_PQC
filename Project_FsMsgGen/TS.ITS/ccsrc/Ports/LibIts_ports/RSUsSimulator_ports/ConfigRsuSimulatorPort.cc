#include "ConfigRsuSimulatorPort.hh"
#include "ConfigRsuSimulatorLayerFactory.hh"
#include "loggers.hh"

namespace ItsRSUsSimulator__TestSystem {

  ConfigRsuSimulatorPort::ConfigRsuSimulatorPort(const char *par_port_name)
    : ConfigRsuSimulatorPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(nullptr), _time_key("ConfigRsuSimulatorPort::outgoing_send") {
    // Nothing to do
  } // End of constructor

  ConfigRsuSimulatorPort::~ConfigRsuSimulatorPort() {
    loggers::get_instance().log(">>> ConfigRsuSimulatorPort::~ConfigRsuSimulatorPort");

    if (_layer != nullptr) {
      delete _layer;
    }
  } // End of destructor

  void ConfigRsuSimulatorPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("ConfigRsuSimulatorPort::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
  }

  /*void ConfigRsuSimulatorPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void ConfigRsuSimulatorPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void ConfigRsuSimulatorPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void ConfigRsuSimulatorPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void ConfigRsuSimulatorPort::Handle_Timeout(double time_since_last_call) {}*/

  void ConfigRsuSimulatorPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> ConfigRsuSimulatorPort::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("ConfigRsuSimulatorPort::user_map: %s", it->second.c_str());
      // Setup parameters
      params::convert(_layer_params, it->second); // TODO This _layer_params seems to be useless
      // Create layer
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<ConfigRsuSimulatorLayer *>(_layer) == nullptr) {
        loggers::get_instance().error("ConfigRsuSimulatorPort::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<ConfigRsuSimulatorLayer *>(_layer)->add_upper_port(this);

    } else {
      loggers::get_instance().error("ConfigRsuSimulatorPort::user_map: No layers defined in configuration file");
    }
  } // End of user_map method

  void ConfigRsuSimulatorPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> ConfigRsuSimulatorPort::user_unmap: %s", system_port);

    // Reset layers
    if (_layer != nullptr) {
      delete _layer;
      _layer = nullptr;
    }
  } // End of user_unmap method

  void ConfigRsuSimulatorPort::user_start() { loggers::get_instance().log(">>> ConfigRsuSimulatorPort::user_start"); } // End of user_start method

  void ConfigRsuSimulatorPort::user_stop() { loggers::get_instance().log(">>> ConfigRsuSimulatorPort::user_stop"); } // End of user_stop method

  void ConfigRsuSimulatorPort::outgoing_send(const CfInitialize &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    /*float duration;
    loggers::get_instance().set_start_time(_time_key);
    params params;
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, params);
    loggers::get_instance().set_stop_time(_time_key, duration);*/
  }

  void ConfigRsuSimulatorPort::outgoing_send(const BOOLEAN &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsCam__TypesAndValues::UtCamResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsCam__TypesAndValues::UtCamEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsPki__TypesAndValues::UtPkiResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsIvim__TypesAndValues::UtIvimResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsIvim__TypesAndValues::UtIvimEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsSremSsem__TypesAndValues::UtSremResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsSremSsem__TypesAndValues::UtSremEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsSremSsem__TypesAndValues::UtSsemEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsRtcmem__TypesAndValues::UtRtcmemResults &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::outgoing_send(const LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &send_par) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::outgoing_send: payload=", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<ConfigRsuSimulatorLayer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const BOOLEAN &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    // incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsCam__TypesAndValues::UtCamInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsCam__TypesAndValues::UtCamTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsDenm__TypesAndValues::UtDenmInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsDenm__TypesAndValues::UtDenmTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsDenm__TypesAndValues::UtDenmUpdate &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsDenm__TypesAndValues::UtDenmTermination &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsPki__TypesAndValues::UtPkiInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsPki__TypesAndValues::UtPkiTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsIvim__TypesAndValues::UtIvimInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsIvim__TypesAndValues::UtIvimTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsIvim__TypesAndValues::UtIvimUpdate &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsIvim__TypesAndValues::UtIvimTermination &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremUpdate &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremCancelation &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemInitialize &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemUpdate &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

  void ConfigRsuSimulatorPort::receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTermination &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> ConfigRsuSimulatorPort::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    incoming_message(p_ind);
  }

} // namespace ItsRSUsSimulator__TestSystem
