/*!
 * \file      uppertester_ivim_layer.hh
 * \brief     Header file for ITS UpperTester IVIM protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "t_layer.hh"
#include "uppertester_ivim_codec.hh"

namespace LibItsIvim__TestSystem {
  class UpperTesterPort;
}

namespace LibItsIvim__TypesAndValues {
  class UtIvimInitialize;
  class UtIvimTrigger;
  class UtIvimUpdate;
  class UtIvimTermination;
} // namespace LibItsIvim__TypesAndValues

class uppertester_ivim_layer : public t_layer<LibItsIvim__TestSystem::UpperTesterPort> {
  params                 _params;
  uppertester_ivim_codec _codec;

public:
  uppertester_ivim_layer() : t_layer<LibItsIvim__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  uppertester_ivim_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_ivim_layer(){};

  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimInitialize &send_par, params &p_params);
  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimTrigger &send_par, params &p_params);
  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimUpdate &send_par, params &p_params);
  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimTermination &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_ivim_layer
