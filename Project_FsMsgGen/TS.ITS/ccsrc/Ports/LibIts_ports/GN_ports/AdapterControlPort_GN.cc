#include <TTCN3.hh>

#include "loggers.hh"
#include "registration.hh"

#include "AdapterControlPort_GN.hh"
#include "geonetworking_layer.hh"
//=============================================================================
namespace LibItsGeoNetworking__TestSystem {

  AdapterControlPort::AdapterControlPort(const char *par_port_name) : AdapterControlPort_BASE(par_port_name), _params() {
    loggers::get_instance().log("AdapterControlPort::AdapterControlPort");
  }

  AdapterControlPort::~AdapterControlPort() {}

  void AdapterControlPort::set_parameter(const char *parameter_name, const char *parameter_value) {
    loggers::get_instance().log("AdapterControlPort::set_parameter: %s=%s", parameter_name, parameter_value);
  }

  /*void AdapterControlPort::Handle_Fd_Event(int fd, boolean is_readable,
    boolean is_writable, boolean is_error) {}*/

  void AdapterControlPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void AdapterControlPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void AdapterControlPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void AdapterControlPort::Handle_Timeout(double time_since_last_call) {}*/

  void AdapterControlPort::user_map(const char *system_port) { loggers::get_instance().log(">>> AdapterControlPort::user_map: %s", system_port); }

  void AdapterControlPort::user_unmap(const char *system_port) { loggers::get_instance().log(">>> AdapterControlPort::user_unmap: %s", system_port); }

  void AdapterControlPort::user_start() {}

  void AdapterControlPort::user_stop() {}

  void AdapterControlPort::outgoing_send(const LibItsGeoNetworking__TypesAndValues::AcGnPrimitive &send_par) {
    loggers::get_instance().log_msg(">>> AdapterControlPort::outgoing_send: ", send_par);

    // Register this object for AdapterControlPort
    geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
    if (p != NULL) {
      loggers::get_instance().log("AdapterControlPort::outgoing_send: Got GN layer %p", p);
      LibItsGeoNetworking__TypesAndValues::AcGnResponse response;
      response.failure() = LibItsGeoNetworking__TypesAndValues::AcGnResponseFailure(BOOLEAN(false));
      if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::AcGnPrimitive::ALT_startBeaconing)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Start beaconing");
        p->start_beaconing(send_par.startBeaconing().beaconPacket());
        // No response
        return;
      } else if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::AcGnPrimitive::ALT_stopBeaconing)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Stop beaconing");
        p->stop_beaconing();
        // No response
        return;
      } else if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::AcGnPrimitive::ALT_getLongPosVector)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Get LongPosVector");
        const LibItsGeoNetworking__TypesAndValues::LongPosVector *lpv = p->get_lpv(send_par.getLongPosVector().gnAddress());
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Return value: %p", lpv);
        if (lpv == nullptr) {
          response.failure() = LibItsGeoNetworking__TypesAndValues::AcGnResponseFailure(BOOLEAN(true));
        } else {
          response.getLongPosVector() = *lpv;
        }
      } else if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::AcGnPrimitive::ALT_startPassBeaconing)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Start pass beaconing");
        p->start_pass_beaconing(send_par.startPassBeaconing().beaconHeader());
        // No response
        return;
      } else if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::AcGnPrimitive::ALT_stopPassBeaconing)) {
        loggers::get_instance().log("AdapterControlPort::outgoing_send: Stop pass beaconing");
        p->stop_pass_beaconing();
        // No response
        return;
      } else {
        response.failure() = LibItsGeoNetworking__TypesAndValues::AcGnResponseFailure(BOOLEAN(true));
      }
      // Send response
      loggers::get_instance().log_msg("AdapterControlPort::outgoing_send: Send response: ", response);
      incoming_message(response);
    } else {
      loggers::get_instance().error("AdapterControlPort::outgoing_send: %s not registered", "geoNetworkingPort");
    }
  }

  void AdapterControlPort::outgoing_send(const LibItsIpv6OverGeoNetworking__TypesAndValues::AcGn6Primitive & /*send_par*/) {}

  void AdapterControlPort::outgoing_send(const LibItsCommon__TypesAndValues::AcGnssPrimitive & /*send_par*/) {}

  void AdapterControlPort::outgoing_send(const LibItsCommon__TypesAndValues::AcSecPrimitive &send_par) {
    loggers::get_instance().log_msg(">>> AdapterControlPort::outgoing_send: ", send_par);

    // Register this object for AdapterControlPort
    geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
    if (p != NULL) {
      loggers::get_instance().log("AdapterControlPort::outgoing_send: Got GN layer %p", p);
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
      loggers::get_instance().error("AdapterControlPort::outgoing_send: %s not registered", "geoNetworkingPort");
    }
  }

} // namespace LibItsGeoNetworking__TestSystem
