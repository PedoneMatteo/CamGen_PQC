//=============================================================================
#ifndef AdapterControlPort_SremSsem_HH
#define AdapterControlPort_SremSsem_HH

#include "LibItsSremSsem_TestSystem.hh"

namespace LibItsSremSsem__TestSystem {

  class AdapterControlPort : public AdapterControlPort_BASE {
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

    void outgoing_send(const LibItsCommon__TypesAndValues::AcSecPrimitive &send_par);
  };

} // namespace LibItsSremSsem__TestSystem
#endif
