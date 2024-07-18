/*!
 * \file      UpperTesterRtcmemFactory.hh
 * \brief     Header file for ITS SREM/SSEM Upper Tester protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "UpperTesterRtcmemLayer.hh"

class UpperTesterRtcmemFactory : public layer_factory {
  static UpperTesterRtcmemFactory _f;

public:
  UpperTesterRtcmemFactory() {
    // Register factory
    layer_stack_builder::register_layer_factory("UT_RTCMEM", this);
  };
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new UpperTesterRtcmemLayer(p_type, p_param); };
}; // End of class UpperTesterRtcmemFactory
