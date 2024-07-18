#ifndef AdapterControlPort_GN_HH
#define AdapterControlPort_GN_HH

//=============================================================================
#include "LibItsGeoNetworking_TestSystem.hh"

#include "params.hh"

namespace LibItsGeoNetworking__TestSystem {

  class AdapterControlPort : public AdapterControlPort_BASE {
    params _params;

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

    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::AcGnPrimitive &send_par);

    void outgoing_send(const LibItsIpv6OverGeoNetworking__TypesAndValues::AcGn6Primitive &send_par);

    void outgoing_send(const LibItsCommon__TypesAndValues::AcGnssPrimitive &send_par);

    void outgoing_send(const LibItsCommon__TypesAndValues::AcSecPrimitive &send_par);
  };

} // namespace LibItsGeoNetworking__TestSystem
#endif
