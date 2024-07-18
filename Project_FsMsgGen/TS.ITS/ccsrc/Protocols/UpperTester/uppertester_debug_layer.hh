/*!
 * \file      uppertester_debug_layer.hh
 * \brief     Header file for ITS UPPERTESTER_DEBUG/IP protocol layer definition.
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

#include "params.hh"
#include "t_layer.hh"

class PORT; //! Forward declaration of TITAN class

/*!
 * \class uppertester_debug_layer
 * \brief  This class provides description of ITS UPPERTESTER_DEBUG/IP protocol layer
 */
class uppertester_debug_layer : public layer, public PORT {
  params _params; //! Layer parameters

public: //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the uppertester_debug_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  uppertester_debug_layer(const std::string &p_type, const std::string &p_param);
  /*!
   * \brief Default destructor
   */
  virtual ~uppertester_debug_layer();

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  virtual void send_data(OCTETSTRING &p_data, params &p_params);
  /*!
   * \virtual
   * \fn void receive_data(OCTETSTRING& data, params& params);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_data The bytes formated data received
   * \param[in] p_params Some lower layers parameters values when data was received
   */
  virtual void receive_data(OCTETSTRING &p_data, params &p_params);

}; // End of class uppertester_debug_layer
