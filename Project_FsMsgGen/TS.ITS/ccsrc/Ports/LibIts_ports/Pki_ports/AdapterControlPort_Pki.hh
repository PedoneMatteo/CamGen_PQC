//=============================================================================
#ifndef AdapterControlPort_Pki_HH
#define AdapterControlPort_Pki_HH

#include "LibItsPki_TestSystem.hh"

//#include "params.hh"

namespace LibItsPki__TestSystem {

  class AdapterControlPort : public AdapterControlPort_BASE {
//    params _params;

  public:
    AdapterControlPort(const char *par_port_name = NULL);
    ~AdapterControlPort();

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

    void outgoing_send(const LibItsPki__TypesAndValues::AcPkiPrimitive &send_par);
  };

} // namespace LibItsPki__TestSystem
#endif
