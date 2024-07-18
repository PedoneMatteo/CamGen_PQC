#include <TTCN3.hh>

#include "loggers.hh"
#include "registration.hh"

#include "AdapterControlPort_CAM.hh"
#include "cam_layer.hh"

//=============================================================================
namespace LibItsCam__TestSystem {

  AdapterControlPort::AdapterControlPort(const char *par_port_name) : AdapterControlPort_BASE(par_port_name) {}

  AdapterControlPort::~AdapterControlPort() {}

  void AdapterControlPort::set_parameter(const char * /*parameter_name*/, const char * /*parameter_value*/) {}

  /*void AdapterControlPort::Handle_Fd_Event(int fd, boolean is_readable,
          boolean is_writable, boolean is_error) {}*/

  void AdapterControlPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void AdapterControlPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void AdapterControlPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void AdapterControlPort::Handle_Timeout(double time_since_last_call) {}*/

  void AdapterControlPort::user_map(const char * /*system_port*/) {}

  void AdapterControlPort::user_unmap(const char * /*system_port*/) {}

  void AdapterControlPort::user_start() {}

  void AdapterControlPort::user_stop() {}

  void AdapterControlPort::outgoing_send(const LibItsCommon__TypesAndValues::AcGnssPrimitive & /*send_par*/) {}

  void AdapterControlPort::outgoing_send(const LibItsCommon__TypesAndValues::AcSecPrimitive &send_par) {
    loggers::get_instance().log_msg(">>> AdapterControlPort::outgoing_send: ", send_par);

    // Register this object for AdapterControlPort
    cam_layer *p = registration<cam_layer>::get_instance().get_item(std::string("CAM"));
    if (p != NULL) {
      loggers::get_instance().log("AdapterControlPort::outgoing_send: Got CAM layer %p", p);
      LibItsCommon__TypesAndValues::AdapterControlResults response;
      response.acSecResponse() = BOOLEAN(true);
      if (send_par.ischosen(LibItsCommon__TypesAndValues::AcSecPrimitive::ALT_acEnableSecurity)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Enable secured mode");
        std::string str(static_cast<const char *>(send_par.acEnableSecurity().certificateId()));
        if (p->enable_secured_mode(str, send_par.acEnableSecurity().enforceSecurity()) == -1) {
          response.acSecResponse() = BOOLEAN(false);
        }
      } else if (send_par.ischosen(LibItsCommon__TypesAndValues::AcSecPrimitive::ALT_acDisableSecurity)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Disable secured mode");
        if (p->disable_secured_mode() == -1) {
          response.acSecResponse() = BOOLEAN(false);
        }
      } else {
        response.acSecResponse() = BOOLEAN(false);
      }
      // Send response
      loggers::get_instance().log_msg("AdapterControlPort::outgoing_send: Send response: ", response);
      incoming_message(response);
    } else {
      loggers::get_instance().error("AdapterControlPort::outgoing_send: CAM not registered");
    }
  }

} // namespace LibItsCam__TestSystem
