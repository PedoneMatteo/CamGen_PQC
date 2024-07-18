#pragma once

#include "LibItsHttp_TestSystem.hh"

#include "layer.hh"
#include "params.hh"

namespace LibItsHttp__TypesAndValues {
  class HttpMessage;
}

namespace LibItsHttp__TestSystem {

  class HttpPort : public HttpPort_BASE {
    params      _cfg_params;
    params      _layer_params;
    layer *     _layer;
    std::string _time_key;

  public:
    HttpPort(const char *par_port_name);
    ~HttpPort();

    void set_parameter(const char *parameter_name, const char *parameter_value);
    void receiveMsg(const LibItsHttp__TypesAndValues::HttpMessage &p_ind, const params &p_params);

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

  protected:
    void outgoing_send(const LibItsHttp__TypesAndValues::HttpMessage &send_par);
  }; // End of class HttpPort

} // namespace LibItsHttp__TestSystem
