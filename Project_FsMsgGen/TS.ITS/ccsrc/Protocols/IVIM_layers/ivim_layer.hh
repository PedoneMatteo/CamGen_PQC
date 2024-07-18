/*!
 * \file      ivim_layer.hh
 * \brief     Header file for ITS IVIM protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "ivim_codec.hh"
#include "t_layer.hh"

namespace LibItsIvim__TestSystem {
  class IvimPort;
  class IvimReq;
  class IvimInd;
} // namespace LibItsIvim__TestSystem

class ivim_layer : public t_layer<LibItsIvim__TestSystem::IvimPort> {
  params     _params;
  ivim_codec _codec;

public:
  ivim_layer() : t_layer<LibItsIvim__TestSystem::IvimPort>(), _params(), _codec(){};
  ivim_layer(const std::string &p_type, const std::string &param);
  virtual ~ivim_layer(){};

  void sendMsg(const LibItsIvim__TypesAndValues::IvimReq &, params &params);
  void sendMsg(const LibItsIvim__TypesAndValues::IvimInd &, params &params);

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security);

  int disable_secured_mode();

}; // End of class ivim_layer
