/*!
 * \file      http_layer.hh
 * \brief     Header file for ITS HTTP protocol layer.
 * \author    ETSI STF549
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <memory>

#include "t_layer.hh"

#include "http_codec.hh"

namespace LibItsHttp__TestSystem {
  class HttpPort;
}

namespace LibItsHttp__TypesAndValues {
  class HttpMessage; //! Forward declaration of TITAN class
}

class OCTETSTRING; //! Forward declaration of TITAN class

/*!
 * \class http_layer
 * \brief  This class provides a factory class to create an tcp_layer class instance
 */
class http_layer : public t_layer<LibItsHttp__TestSystem::HttpPort> {
  params _params;
  http_codec _codec;
  bool _device_mode;

public: //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the http_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  http_layer() : t_layer(), _params(), _device_mode{false} { };
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the http_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  http_layer(const std::string& p_type, const std::string& p_param);
  /*!
   * \brief Default destructor
   */
  virtual ~http_layer() { };

  /*!
   * \fn void sendMsg(const LibItsHttp__TypesAndValues::HttpMessage& p_http_message, params& p_param);
   * \brief Send HTTP message to the lower layers
   * \param[in] p_http_message The GeoNetworking message to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  void sendMsg(const LibItsHttp__TypesAndValues::HttpMessage& p_http_message, params& p_param);
  
  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  virtual void send_data(OCTETSTRING& data, params& params);
  /*!
   * \virtual
   * \fn void receive_data(OCTETSTRING& data, params& params);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_data The bytes formated data received
   * \param[in] p_params Some lower layers parameters values when data was received
   */
  virtual void receive_data(OCTETSTRING& data, params& info);
}; // End of class http_layer

