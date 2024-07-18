//=============================================================================
#ifndef UpperTesterPort_BTP_HH
#define UpperTesterPort_BTP_HH
#include "LibItsBtp_TestSystem.hh"
namespace LibItsBtp__TestSystem {

  class UpperTesterPort : public UpperTesterPort_BASE {
  public:
    UpperTesterPort(const char *par_port_name = NULL);
    ~UpperTesterPort();

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

    void outgoing_send(const LibItsBtp__TypesAndValues::UtBtpInitialize &send_par);

    void outgoing_send(const LibItsBtp__TypesAndValues::UtBtpTrigger &send_par);
  };

} // namespace LibItsBtp__TestSystem
#endif
