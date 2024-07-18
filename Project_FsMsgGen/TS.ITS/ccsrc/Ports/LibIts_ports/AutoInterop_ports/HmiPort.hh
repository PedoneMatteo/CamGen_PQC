// This Test Port skeleton header file was generated by the
// TTCN-3 Compiler of the TTCN-3 Test Executor version CRL 113 200/5 R3A
// for U-ERICSSON\ethgry (ethgry@HU00078339) on Fri Aug 14 16:32:05 2015

// Copyright Ericsson Telecom AB 2000-2014

// You may modify this file. Add your attributes and prototypes of your
// member functions here.

#ifndef HmiPort_HH
#define HmiPort_HH

#include "ItsAutoInterop_TestSystem.hh"
#include "ItsAutoInterop_TypesAndValues.hh"

namespace ItsAutoInterop__TestSystem {

  class HmiPort : public HmiPort_BASE {
  public:
    HmiPort(const char *par_port_name = NULL);
    ~HmiPort();

    void set_parameter(const char *parameter_name, const char *parameter_value);

  private:
    /* void Handle_Fd_Event(int fd, boolean is_readable,
            boolean is_writable, boolean is_error); */
    void Handle_Fd_Event_Error(int fd);
    void Handle_Fd_Event_Writable(int fd);
    void Handle_Fd_Event_Readable(int fd);
    /* void Handle_Timeout(double time_since_last_call); */
  protected:
    void user_map(const char *system_port);
    void user_unmap(const char *system_port);

    void user_start();
    void user_stop();

    void outgoing_send(const ItsAutoInterop__TypesAndValues::HmiInitialize &send_par);
  };

} // namespace ItsAutoInterop__TestSystem

#endif
