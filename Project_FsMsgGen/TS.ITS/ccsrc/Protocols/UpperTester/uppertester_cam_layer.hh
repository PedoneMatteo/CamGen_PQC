/*!
 * \file      uppertester_cam_layer.hh
 * \brief     Header file for ITS UpperTester CAM protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <memory>

#include "t_layer.hh"
#include "uppertester_cam_codec.hh"

namespace LibItsCam__TestSystem {
  class UpperTesterPort;
}

namespace LibItsCam__TypesAndValues {
  class UtCamInitialize;
  class UtCamChangePosition;
  class UtCamTrigger;
} // namespace LibItsCam__TypesAndValues

class uppertester_cam_layer : public t_layer<LibItsCam__TestSystem::UpperTesterPort> {
  params                _params;
  uppertester_cam_codec _codec;

public:
  uppertester_cam_layer() : t_layer<LibItsCam__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  uppertester_cam_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_cam_layer(){};

  void sendMsg(const LibItsCam__TypesAndValues::UtCamInitialize &send_par, params &p_params);
  void sendMsg(const LibItsCam__TypesAndValues::UtCamChangePosition &send_par, params &p_params);
  void sendMsg(const LibItsCam__TypesAndValues::UtCamTrigger &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_cam_layer
