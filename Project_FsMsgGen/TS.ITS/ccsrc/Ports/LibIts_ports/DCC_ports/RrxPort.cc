// This Test Port skeleton source file was generated by the
// TTCN-3 Compiler of the TTCN-3 Test Executor version CRL 113 200/5 R3A
// for U-ERICSSON\ethgry (ethgry@HU00078339) on Fri Aug 14 21:19:06 2015

// Copyright Ericsson Telecom AB 2000-2014

// You may modify this file. Complete the body of empty functions and
// add your member functions here.

#include "RrxPort.hh"

namespace LibItsDcc__TestSystem {

  RrxPort::RrxPort(const char *par_port_name) : RrxPort_BASE(par_port_name) {}

  RrxPort::~RrxPort() {}

  void RrxPort::set_parameter(const char * /*parameter_name*/, const char * /*parameter_value*/) {}

  /*void RrxPort::Handle_Fd_Event(int fd, boolean is_readable,
          boolean is_writable, boolean is_error) {}*/

  void RrxPort::Handle_Fd_Event_Error(int /*fd*/) {}

  void RrxPort::Handle_Fd_Event_Writable(int /*fd*/) {}

  void RrxPort::Handle_Fd_Event_Readable(int /*fd*/) {}

  /*void RrxPort::Handle_Timeout(double time_since_last_call) {}*/

  void RrxPort::user_map(const char * /*system_port*/) {}

  void RrxPort::user_unmap(const char * /*system_port*/) {}

  void RrxPort::user_start() {}

  void RrxPort::user_stop() {}

} // namespace LibItsDcc__TestSystem
