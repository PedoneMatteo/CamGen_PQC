#include "uppertester_denm_layer_factory.hh"

#include "UpperTesterPort_DENM.hh"
#include "loggers.hh"
//=============================================================================
namespace LibItsDenm__TestSystem {

  UpperTesterPort::UpperTesterPort(const char *par_port_name)
    : UpperTesterPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(NULL), _time_key("UpperTesterPort_Denm::outgoing_send") {
    loggers::get_instance().log("UpperTesterPort_Denm::UpperTesterPort_Denm");
  }

  UpperTesterPort::~UpperTesterPort() {
    if (_layer != NULL) {
      delete _layer;
    }
  }

  void UpperTesterPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("UpperTesterPort_Denm::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
    _cfg_params.log();
  }

  /*void UpperTesterPort_Denm::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void UpperTesterPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void UpperTesterPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void UpperTesterPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void UpperTesterPort_Denm::Handle_Timeout(double time_since_last_call) {}*/

  void UpperTesterPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> UpperTesterPort_Denm::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("UpperTesterPort_Denm::user_map: %s", it->second.c_str());
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<uppertester_denm_layer *>(_layer) == NULL) {
        loggers::get_instance().error("UpperTesterPort_Denm::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<uppertester_denm_layer *>(_layer)->add_upper_port(this);
    } else {
      loggers::get_instance().error("UpperTesterPort_Denm::user_map: No layers defined in configuration file");
    }
  }

  void UpperTesterPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> UpperTesterPort_Denm::user_unmap: %s", system_port);
    if (_layer != NULL) {
      delete _layer;
      _layer = NULL;
    }
  }

  void UpperTesterPort::user_start() {}

  void UpperTesterPort::user_stop() {}

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmInitialize &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmTrigger &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmUpdate &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmTermination &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmChangePosition &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::outgoing_send(const LibItsDenm__TypesAndValues::UtDenmChangePseudonym &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_denm_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPort::receiveMsg(const Base_Type &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> UpperTesterPort_Denm::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    if (std::string(p_ind.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmResults") == 0) { // TODO To be refined
      incoming_message(static_cast<const LibItsDenm__TypesAndValues::UtDenmResults &>(p_ind));
    } else if (std::string(p_ind.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmEventInd") == 0) {
      incoming_message(static_cast<const LibItsDenm__TypesAndValues::UtDenmEventInd &>(p_ind));
    } else {
      loggers::get_instance().warning("UpperTesterPort_Denm::receive_msg: Message not processed: %s", p_ind.get_descriptor()->name);
    }
  }

} // namespace LibItsDenm__TestSystem
