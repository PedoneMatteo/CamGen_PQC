/*!
 * \file      btp_layer.hh
 * \brief     Header file for ITS BTP protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "btp_codec.hh"
#include "t_layer.hh"

namespace LibItsBtp__TestSystem {
  class BtpPort; //! Forward declaration of TITAN class
  class BtpReq;  //! Forward declaration of TITAN class
  class BtpInd;  //! Forward declaration of TITAN class
} // namespace LibItsBtp__TestSystem

/*!
 * \class btp_layer
 * \brief  This class provides description of ITS BTP protocol layer
 */
class btp_layer : public t_layer<LibItsBtp__TestSystem::BtpPort> {
  params    _params;      //! Layer parameters
  btp_codec _codec;       //! BTP codec
  bool      _device_mode; //! Set to true if the BTP layer shall encapsulate the upper layer PDU
public:                   //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the btp_layer class
   */
  explicit btp_layer() : t_layer<LibItsBtp__TestSystem::BtpPort>(), _params(), _codec(), _device_mode{false} {};
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the btp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  btp_layer(const std::string &p_type, const std::string &param);
  virtual ~btp_layer(){};

  /*!
   * \fn void sendMsg(const LibItsBtp__TestSystem::BtpReq& p_btp_req, params& p_param);
   * \brief Send BTP message to the lower layers
   * \param[in] p_btp_req The BTP message to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  void sendMsg(const LibItsBtp__TypesAndValues::BtpReq &p_btp_req, params &p_param);

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
}; // End of class btp_layer
