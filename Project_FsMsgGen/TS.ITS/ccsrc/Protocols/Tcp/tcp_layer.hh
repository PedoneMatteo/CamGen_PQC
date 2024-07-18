/*!
 * \file      tcp_layer.hh
 * \brief     Header file for ITS TCP socket based protocol port layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer.hh"

using namespace std; // Required for isnan()
#include "Abstract_Socket.hh"

class PORT; //! Forward declaration of TITAN class

/*!
 * \class tcp_layer
 * \brief  This class provides description of ITS TCP port protocol layer
 */
class tcp_layer : public layer, public SSL_Socket, public PORT {
  params      _params;            //! Layer parameters
  int         _client_id;         //! Connection identifier
  std::string _time_key;          //! \todo
  bool        _reconnect_on_send; //! Set to true if connection shall be done when sending data. Otherwise, connection is established by the \see constructor

public: //! \publicsection
  //tcp_layer(): PORT("TCP") {};
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the tcp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  tcp_layer(const std::string &p_type, const std::string &p_param);
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the tcp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  tcp_layer(const std::string &p_type, const params &p_param);
  /*!
   * \brief Default destructor
   * \remark If \see _reconnect_on_send is set to false, the disconnection is done by the destructor
   */
  virtual ~tcp_layer();

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   * \virtual
   */
  virtual void send_data(OCTETSTRING &data, params &params);
  /*!
   * \virtual
   * \fn void receive_data(OCTETSTRING& data, params& params);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_data The bytes formated data received
   * \param[in] p_params Some lower layers parameters values when data was received
   */
  virtual void receive_data(OCTETSTRING &data, params &info);

  /*!
   * \virtual
   * \fn void message_incoming(const unsigned char* message_buffer, int length, int client_id = -1);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_buffer The bytes formated data received
   * \param[in] p_length The number of bytes received
   * \param[in] p_client_id The connection identifier.Default: -1
   */
  virtual void message_incoming(const unsigned char *p_buffer, int p_length, int p_client_id = -1);

protected: //! \protectedsection
  void init();

  void Add_Fd_Read_Handler(int fd) { Handler_Add_Fd_Read(fd); };
  void Add_Fd_Write_Handler(int fd) { Handler_Add_Fd_Write(fd); };
  void Remove_Fd_Read_Handler(int fd) { Handler_Remove_Fd_Read(fd); };
  void Remove_Fd_Write_Handler(int fd) { Handler_Remove_Fd_Write(fd); };
  void Remove_Fd_All_Handlers(int fd) { Handler_Remove_Fd(fd); };
  void Handler_Uninstall() { Uninstall_Handler(); }
  void Timer_Set_Handler(double call_interval, boolean is_timeout = TRUE, boolean call_anyway = TRUE, boolean is_periodic = TRUE) {
    Handler_Set_Timer(call_interval, is_timeout, call_anyway, is_periodic);
  };

  const char *remote_address_name() { return params::server.c_str(); };
  const char *remote_port_name() { return params::port.c_str(); };
  const char *socket_debugging_name() { return params::debug.c_str(); };
  const char *ssl_use_ssl_name() { return params::use_ssl.c_str(); };

  void client_connection_opened(int p_client_id);
  bool add_user_data(int p_client_id);
  int  send_message_on_fd(int p_client_id, const unsigned char *message_buffer, int length_of_message);
  int  send_message_on_nonblocking_fd(int client_id, const unsigned char *message_buffer, int length_of_message);
  int  receive_message_on_fd(int p_client_id);
  void peer_disconnected(int p_client_id);

private: //! \privatesection
  void Handle_Fd_Event(int fd, boolean is_readable, boolean is_writable, boolean is_error);
  void Handle_Timeout(double time_since_last_call);
}; // End of class tcp_layer
