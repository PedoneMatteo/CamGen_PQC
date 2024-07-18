/*!
 * \file      uppertester_pki_layer.hh
 * \brief     Header file for ITS UpperTester PKI protocol layer definition.
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
#include "uppertester_pki_codec.hh"

namespace LibItsPki__TestSystem {
  class UpperTesterPkiPort;
}

namespace LibItsPki__TypesAndValues {
  class UtPkiInitialize;
  class UtPkiTrigger;
} // namespace LibItsPki__TypesAndValues

class uppertester_pki_layer : public t_layer<LibItsPki__TestSystem::UpperTesterPkiPort> {
  params                _params;
  uppertester_pki_codec _codec;

public:
  uppertester_pki_layer() : t_layer<LibItsPki__TestSystem::UpperTesterPkiPort>(), _params(), _codec(){};
  uppertester_pki_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_pki_layer(){};

  void sendMsg(const LibItsPki__TypesAndValues::UtPkiInitialize &send_par, params &p_params);
  void sendMsg(const LibItsPki__TypesAndValues::UtPkiTrigger &send_par, params &p_params);
  void sendMsg(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_pki_layer
