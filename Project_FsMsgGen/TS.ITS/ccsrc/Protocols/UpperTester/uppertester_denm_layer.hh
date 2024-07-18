/*!
 * \file      uppertester_denm_layer.hh
 * \brief     Header file for ITS UpperTester DENM protocol layer definition.
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
#include "uppertester_denm_codec.hh"

namespace LibItsDenm__TestSystem {
  class UpperTesterPort;
}

namespace LibItsDenm__TypesAndValues {
  class UtDenmInitialize;
  class UtDenmChangePosition;
  class UtDenmChangePseudonym;
  class UtDenmTrigger;
  class UtDenmUpdate;
  class UtDenmTermination;
} // namespace LibItsDenm__TypesAndValues

class uppertester_denm_layer : public t_layer<LibItsDenm__TestSystem::UpperTesterPort> {
  params                 _params;
  uppertester_denm_codec _codec;

public:
  uppertester_denm_layer() : t_layer<LibItsDenm__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  uppertester_denm_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_denm_layer(){};

  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmInitialize &send_par, params &p_params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmChangePosition &send_par, params &p_params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmChangePseudonym &send_par, params &p_params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmTrigger &send_par, params &p_params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmUpdate &send_par, params &p_params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmTermination &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_denm_layer
