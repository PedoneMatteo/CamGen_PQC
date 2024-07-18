/*!
 * \file      udp_layer.hh
 * \brief     Header file for ITS UDP/IP protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "layer.hh"

using namespace std; // Required for isnan()
#include "Abstract_Socket.hh"

//class PORT; //! Forward declaration of TITAN class

/*!
 * \class udp_layer
 * \brief  This class provides description of ITS UDP/IP protocol layer
 */
class udp_layer : public layer, public PORT {
  params             _params; //! Layer parameters
  struct sockaddr_in _saddr;  //! Source socket address description
  struct sockaddr_in _daddr;  //! Destination socket address description
  bool               _reuse_incoming_source_adddress;
  //! This flag must be set to true if the UpperTester/UDP layer must act as an IUT, not as a Test System. Default value: Test System
  int         _fd;       // Socket file descriptor
  std::string _time_key; //! \todo

public: //! \publicsection
  //udp_layer(): PORT("UDP") {};
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the udp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  udp_layer(const std::string &p_type, const std::string &p_param);
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the udp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  udp_layer(const std::string &p_type, const params &p_param);
  /*!
   * \brief Default destructor
   */
  virtual ~udp_layer();

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
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

  void Handle_Fd_Event_Readable(int fd);

protected:
  void init();

private:
  unsigned long get_host_id(const std::string &p_host_name);
  void          close();
}; // End of class udp_layer
