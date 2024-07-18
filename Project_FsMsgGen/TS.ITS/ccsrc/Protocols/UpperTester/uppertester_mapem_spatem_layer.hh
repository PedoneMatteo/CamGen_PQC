/*!
 * \file      uppertester_mapem_spatem_layer.hh
 * \brief     Header file for ITS MapemSpatem protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "t_layer.hh"
#include "uppertester_mapem_spatem_codec.hh"

namespace LibItsMapemSpatem__TestSystem {
  class UpperTesterPort;
}

namespace LibItsMapemSpatem__TypesAndValues {
  class UtMapemSpatemInitialize;
  class UtMapemSpatemChangePosition;
  class UtMapemSpatemTrigger;
} // namespace LibItsMapemSpatem__TypesAndValues

class uppertester_mapem_spatem_layer : public t_layer<LibItsMapemSpatem__TestSystem::UpperTesterPort> {
  params                         _params;
  uppertester_mapem_spatem_codec _codec;

public:
  uppertester_mapem_spatem_layer() : t_layer<LibItsMapemSpatem__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  uppertester_mapem_spatem_layer(const std::string &p_type, const std::string &param);
  virtual ~uppertester_mapem_spatem_layer(){};

  void sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize &send_par, params &p_params);
  void sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class uppertester_mapem_spatem_layer
