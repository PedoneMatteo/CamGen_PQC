#include <TTCN3.hh>

#include "UpperTesterPkiPort.hh"

#include "loggers.hh"

#include "uppertester_pki_layer.hh"
#include "uppertester_pki_layer_factory.hh"

//=============================================================================
namespace LibItsPki__TestSystem {

  UpperTesterPkiPort::UpperTesterPkiPort(const char *par_port_name)
    : UpperTesterPkiPort_BASE(par_port_name), _cfg_params(), _layer_params(), _layer(NULL), _time_key("UpperTesterPkiPort_Pki::outgoing_send") {
    loggers::get_instance().log("UpperTesterPkiPort_Pki::UpperTesterPkiPort_Pki");
  }

  UpperTesterPkiPort::~UpperTesterPkiPort() {
    if (_layer != NULL) {
      delete _layer;
    }
  }

  void UpperTesterPkiPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("UpperTesterPkiPort_Pki::set_parameter: %s=%s", parameter_name, parameter_value);
    _cfg_params.insert(std::pair<std::string, std::string>(std::string(parameter_name), std::string(parameter_value)));
    _cfg_params.log();
  }

  /*void UpperTesterPkiPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void UpperTesterPkiPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void UpperTesterPkiPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void UpperTesterPkiPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void UpperTesterPkiPort::Handle_Timeout(double time_since_last_call) {}*/

  void UpperTesterPkiPort::user_map(const char *system_port) {
    loggers::get_instance().log(">>> UpperTesterPkiPort_Pki::user_map: %s", system_port);
    // Build layer stack
    params::iterator it = _cfg_params.find(std::string("params"));
    if (it != _cfg_params.end()) {
      loggers::get_instance().log("UpperTesterPkiPort_Pki::user_map: %s", it->second.c_str());
      _layer = layer_stack_builder::get_instance()->create_layer_stack(it->second.c_str());
      if (static_cast<uppertester_pki_layer *>(_layer) == NULL) {
        loggers::get_instance().error("UpperTesterPkiPort_Pki::user_map: Invalid stack configuration: %s", it->second.c_str());
      }
      static_cast<uppertester_pki_layer *>(_layer)->add_upper_port(this);
    } else {
      loggers::get_instance().error("UpperTesterPkiPort_Pki::user_map: No layers defined in configuration file");
    }
  }

  void UpperTesterPkiPort::user_unmap(const char *system_port) {
    loggers::get_instance().log(">>> UpperTesterPkiPort_Pki::user_unmap: %s", system_port);
    if (_layer != NULL) {
      delete _layer;
      _layer = NULL;
    }
  }

  void UpperTesterPkiPort::user_start() {}

  void UpperTesterPkiPort::user_stop() {}

  void UpperTesterPkiPort::outgoing_send(const LibItsPki__TypesAndValues::UtPkiInitialize &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_pki_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPkiPort::outgoing_send(const LibItsPki__TypesAndValues::UtPkiTrigger &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_pki_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPkiPort::outgoing_send(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par) {
    loggers::get_instance().log_msg(">>> UppertesterPort::outgoing_send: ", send_par);

    float duration;
    loggers::get_instance().set_start_time(_time_key);
    static_cast<uppertester_pki_layer *>(_layer)->sendMsg(send_par, _layer_params);
    loggers::get_instance().set_stop_time(_time_key, duration);
  }

  void UpperTesterPkiPort::receiveMsg(const Base_Type &p_ind, const params &p_params) {
    loggers::get_instance().log_msg(">>> UpperTesterPkiPort_Pki::receive_msg: ", p_ind);
    // Sanity check
    if (!p_ind.is_bound()) {
      return;
    }

    if (std::string(p_ind.get_descriptor()->name).compare("@LibItsPki_TypesAndValues.UtPkiResults") == 0) { // TODO To be refined
      incoming_message(static_cast<const LibItsPki__TypesAndValues::UtPkiResults &>(p_ind));
    } else if (std::string(p_ind.get_descriptor()->name).compare("@LibItsPki_TypesAndValues.UtPkiTriggerInd") == 0) { // TODO To be refined
      incoming_message(static_cast<const LibItsPki__TypesAndValues::UtPkiTriggerInd &>(p_ind));
    } else {
      loggers::get_instance().warning("UpperTesterPkiPort_Pki::receive_msg: Message not processed: %s", p_ind.get_descriptor()->name);
    }
  }

} // namespace LibItsPki__TestSystem
