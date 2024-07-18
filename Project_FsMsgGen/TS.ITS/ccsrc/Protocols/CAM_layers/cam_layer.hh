/*!
 * \file      cam_layer.hh
 * \brief     Header file for ITS CAM protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "cam_codec.hh"
#include "t_layer.hh"

namespace LibItsCam__TestSystem {
  class CamPort; //! Forward declaration of TITAN class
  class CamReq;  //! Forward declaration of TITAN class
  class CamInd;  //! Forward declaration of TITAN class
} // namespace LibItsCam__TestSystem

/*!
 * \class cam_layer
 * \brief  This class provides description of ITS CAM protocol layer
 */
class cam_layer : public t_layer<LibItsCam__TestSystem::CamPort> {
  params    _params; //! Layer parameters
  cam_codec _codec;  //! CAM codec
public:              //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the cam_layer class
   */
  explicit cam_layer() : t_layer<LibItsCam__TestSystem::CamPort>(), _params(), _codec(){};
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the cam_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  cam_layer(const std::string &p_type, const std::string &param);
  /*!
   * \brief Default destructor
   */
  virtual ~cam_layer(){};

  /*!
   * \fn void sendMsg(const LibItsCam__TestSystem::CamReq& p_cam_req, params& p_params);
   * \brief Send CA message to the lower layers
   * \param[in] p_cam_req The CA message to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  void sendMsg(const LibItsCam__TypesAndValues::CamReq &p_cam_req, params &p_params);

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

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security);

  int disable_secured_mode();

}; // End of class cam_layer
