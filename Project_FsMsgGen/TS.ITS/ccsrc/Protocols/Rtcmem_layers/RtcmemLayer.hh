/*!
 * \file      RtcmemLayer.hh
 * \brief     Header file for ITS Rtcmem protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "RtcmemCodec.hh"
#include "t_layer.hh"

namespace LibItsRtcmem__TestSystem {
  class RtcmemPort;
  class RtcmemReq;
  class RtcmemInd;
} // namespace LibItsRtcmem__TestSystem

class RtcmemLayer : public t_layer<LibItsRtcmem__TestSystem::RtcmemPort> {
  params      _params;
  RtcmemCodec _rtcmem_codec;

public:
  RtcmemLayer() : t_layer<LibItsRtcmem__TestSystem::RtcmemPort>(), _params(), _rtcmem_codec(){};
  RtcmemLayer(const std::string &p_type, const std::string &param);
  virtual ~RtcmemLayer(){};

  void sendMsg(const LibItsRtcmem__TypesAndValues::RtcmemReq &, params &params);

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security);

  int disable_secured_mode();

}; // End of class RtcmemLayer
