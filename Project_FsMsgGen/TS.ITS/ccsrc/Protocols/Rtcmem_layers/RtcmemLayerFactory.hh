/*!
 * \file      RtcmemLayerFactory.hh
 * \brief     Header file for ITS Rtcmem protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "RtcmemLayer.hh"

class RtcmemLayerFactory : public layer_factory {
  static RtcmemLayerFactory _f;

public:
  RtcmemLayerFactory() {
    // Register factory
    layer_stack_builder::register_layer_factory("Rtcmem", this);
  };
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new RtcmemLayer(p_type, p_param); };
}; // End of class RtcmemLayerFactory
