#ifndef UpperTesterPort_GN_HH
#define UpperTesterPort_GN_HH
#include "layer.hh"
#include "params.hh"

//=============================================================================
#include "LibItsGeoNetworking_TestSystem.hh"
namespace LibItsGeoNetworking__TestSystem {

  class UpperTesterPort : public UpperTesterPort_BASE {
    params      _cfg_params;
    params      _layer_params;
    layer *     _layer;
    std::string _time_key;

  public:
    UpperTesterPort(const char *par_port_name = NULL);
    ~UpperTesterPort();

    void set_parameter(const char *parameter_name, const char *parameter_value);

    void receiveMsg(const Base_Type &, const params &);

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

    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &send_par);

    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnChangePosition &send_par);

    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &send_par);

    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtAutoInteropTrigger &send_par);
  };

} // namespace LibItsGeoNetworking__TestSystem
#endif
