/*!
 * \file      denm_layer.hh
 * \brief     Header file for ITS DENM protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "denm_codec.hh"
#include "t_layer.hh"

namespace LibItsDenm__TestSystem {
  class DenmPort; //! Forward declaration of TITAN class
  class DenmReq;  //! Forward declaration of TITAN class
  class DenmInd;  //! Forward declaration of TITAN class
} // namespace LibItsDenm__TestSystem

/*!
 * \class denm_layer
 * \brief  This class provides description of ITS DENM protocol layer
 */
class denm_layer : public t_layer<LibItsDenm__TestSystem::DenmPort> {
  params     _params; //! Layer parameters
  denm_codec _codec;  //! DENM codec
public:               //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the denm_layer class
   */
  explicit denm_layer() : t_layer<LibItsDenm__TestSystem::DenmPort>(), _params(), _codec(){};
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the denm_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  denm_layer(const std::string &p_type, const std::string &param);
  /*!
   * \brief Default destructor
   */
  virtual ~denm_layer(){};

  /*!
   * \fn void sendMsg(const LibItsDenm__TestSystem::DenmReq& p_denm_req, params& p_params);
   * \brief Send DEN message to the lower layers
   * \param[in] p_denm_req The DEN message to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  void sendMsg(const LibItsDenm__TypesAndValues::DenmReq &p_denm_req, params &params);

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

}; // End of class denm_layer
