/*!
 * \file      uppertester_geonetworking_layer.hh
 * \brief     Header file for ITS GeoNetworking protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "t_layer.hh"
#include "uppertester_geonetworking_codec.hh"

namespace LibItsGeoNetworking__TestSystem {
  class UpperTesterPort;
}

namespace LibItsGeoNetworking__TypesAndValues {
  class UtGnInitialize;
  class UtGnChangePosition;
  class UtGnTrigger;
  class UtAutoInteropTrigger;
} // namespace LibItsGeoNetworking__TypesAndValues

class uppertester_geonetworking_layer : public t_layer<LibItsGeoNetworking__TestSystem::UpperTesterPort> {
  params                          _params;
  uppertester_geonetworking_codec _codec;

public:
  uppertester_geonetworking_layer() : t_layer<LibItsGeoNetworking__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  uppertester_geonetworking_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_geonetworking_layer(){};

  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &send_par, params &p_params);
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnChangePosition &send_par, params &p_params);
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &send_par, params &p_params);
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtAutoInteropTrigger &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_geonetworking_layer
