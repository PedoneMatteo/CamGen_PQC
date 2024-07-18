/*!
 * \file      mapem_spatem_layer.hh
 * \brief     Header file for ITS MapemSpatem protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "mapem_codec.hh"
#include "spatem_codec.hh"
#include "t_layer.hh"

namespace LibItsMapemSpatem__TestSystem {
  class MapemSpatemPort;
  class MapemReq;
  class MapemInd;
  class SpatemReq;
  class SpatemInd;
} // namespace LibItsMapemSpatem__TestSystem

class mapem_spatem_layer : public t_layer<LibItsMapemSpatem__TestSystem::MapemSpatemPort> {
  params       _params;
  mapem_codec  _mapem_codec;
  spatem_codec _spatem_codec;

public:
  mapem_spatem_layer() : t_layer<LibItsMapemSpatem__TestSystem::MapemSpatemPort>(), _params(), _mapem_codec(), _spatem_codec(){};
  mapem_spatem_layer(const std::string &p_type, const std::string &param);
  virtual ~mapem_spatem_layer(){};

  void sendMsg(const LibItsMapemSpatem__TypesAndValues::MapemReq &, params &params);
  void sendMsg(const LibItsMapemSpatem__TypesAndValues::SpatemReq &, params &params);

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security);

  int disable_secured_mode();

}; // End of class mapem_spatem_layer
