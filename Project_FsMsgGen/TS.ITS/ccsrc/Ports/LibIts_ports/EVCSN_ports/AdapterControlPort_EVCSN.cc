#include "AdapterControlPort_EVCSN.hh"
//=============================================================================
namespace LibItsEvcsn__TestSystem {

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

  void AdapterControlPort::outgoing_send(const LibItsCommon__TypesAndValues::AcSecPrimitive & /*send_par*/) {}

} // namespace LibItsEvcsn__TestSystem
