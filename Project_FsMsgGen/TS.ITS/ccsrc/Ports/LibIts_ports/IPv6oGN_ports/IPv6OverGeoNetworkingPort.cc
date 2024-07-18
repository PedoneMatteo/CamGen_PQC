// This Test Port skeleton source file was generated by the
// TTCN-3 Compiler of the TTCN-3 Test Executor version CRL 113 200/5 R3A
// for U-ERICSSON\ethgry (ethgry@HU00078339) on Sun Aug 16 09:18:30 2015

// Copyright Ericsson Telecom AB 2000-2014

// You may modify this file. Complete the body of empty functions and
// add your member functions here.

#include "IPv6OverGeoNetworkingPort.hh"

namespace LibItsIpv6OverGeoNetworking__TestSystem {

  IPv6OverGeoNetworkingPort::IPv6OverGeoNetworkingPort(const char *par_port_name) : IPv6OverGeoNetworkingPort_BASE(par_port_name) {}

  IPv6OverGeoNetworkingPort::~IPv6OverGeoNetworkingPort() {}

  void IPv6OverGeoNetworkingPort::set_parameter(const char * /*parameter_name*/, const char * /*parameter_value*/) {}

  /*void IPv6OverGeoNetworkingPort::Handle_Fd_Event(int fd, boolean is_readable,
          boolean is_writable, boolean is_error) {}*/

  void IPv6OverGeoNetworkingPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void IPv6OverGeoNetworkingPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void IPv6OverGeoNetworkingPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void IPv6OverGeoNetworkingPort::Handle_Timeout(double time_since_last_call) {}*/

  void IPv6OverGeoNetworkingPort::user_map(const char * /*system_port*/) {}

  void IPv6OverGeoNetworkingPort::user_unmap(const char * /*system_port*/) {}

  void IPv6OverGeoNetworkingPort::user_start() {}

  void IPv6OverGeoNetworkingPort::user_stop() {}

  void IPv6OverGeoNetworkingPort::outgoing_send(const LibItsIpv6OverGeoNetworking__TypesAndValues::IPv6OverGeoNetworkingReq & /*send_par*/) {}

} // namespace LibItsIpv6OverGeoNetworking__TestSystem
