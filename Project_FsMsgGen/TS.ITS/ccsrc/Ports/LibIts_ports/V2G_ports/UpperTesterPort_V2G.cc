//=============================================================================
#include "UpperTesterPort_V2G.hh"
namespace LibItsV2G__TestSystem {

  UpperTesterPort::UpperTesterPort(const char *par_port_name) : UpperTesterPort_BASE(par_port_name) {}

  UpperTesterPort::~UpperTesterPort() {}

  void UpperTesterPort::set_parameter(const char * /*parameter_name*/, const char * /*parameter_value*/) {}

  /*void UpperTesterPort::Handle_Fd_Event(int fd, boolean is_readable,
          boolean is_writable, boolean is_error) {}*/

  void UpperTesterPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void UpperTesterPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void UpperTesterPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void UpperTesterPort::Handle_Timeout(double time_since_last_call) {}*/

  void UpperTesterPort::user_map(const char * /*system_port*/) {}

  void UpperTesterPort::user_unmap(const char * /*system_port*/) {}

  void UpperTesterPort::user_start() {}

  void UpperTesterPort::user_stop() {}

  void UpperTesterPort::outgoing_send(const LibItsV2G__TypesAndValues::UtInitialize & /*send_par*/) {}

  void UpperTesterPort::outgoing_send(const LibItsV2G__TypesAndValues::UtTrigger & /*send_par*/) {}

  void UpperTesterPort::outgoing_send(const LibItsV2G__TypesAndValues::UtCheck & /*send_par*/) {}

} // namespace LibItsV2G__TestSystem
