/*!
 * \file      SremSsemLayer.hh
 * \brief     Header file for ITS SremSsem protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "SremCodec.hh"
#include "SsemCodec.hh"
#include "t_layer.hh"

namespace LibItsSremSsem__TestSystem {
  class SremSsemPort;
  class SremReq;
  class SremInd;
  class SsemReq;
  class SsemInd;
} // namespace LibItsSremSsem__TestSystem

class SremSsemLayer : public t_layer<LibItsSremSsem__TestSystem::SremSsemPort> {
  params    _params;
  SremCodec _srem_codec;
  SsemCodec _ssem_codec;

public:
  SremSsemLayer() : t_layer<LibItsSremSsem__TestSystem::SremSsemPort>(), _params(), _srem_codec(), _ssem_codec(){};
  SremSsemLayer(const std::string &p_type, const std::string &param);
  virtual ~SremSsemLayer(){};

  void sendMsg(const LibItsSremSsem__TypesAndValues::SremReq &, params &params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::SsemReq &, params &params);

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security);

  int disable_secured_mode();

}; // End of class SremSsemLayer
