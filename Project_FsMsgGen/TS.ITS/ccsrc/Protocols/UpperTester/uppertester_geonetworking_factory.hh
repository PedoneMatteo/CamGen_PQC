/*!
 * \file      uppertester_geonetworking_factory.hh
 * \brief     Header file for ITS GeoNetworking Upper Tester protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "uppertester_geonetworking_layer.hh"

class uppertester_geonetworking_factory : public layer_factory {
  static uppertester_geonetworking_factory _f;

public:
  uppertester_geonetworking_factory() {
    // Register factory
    layer_stack_builder::register_layer_factory("UT_GN", this);
  };
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new uppertester_geonetworking_layer(p_type, p_param); };
}; // End of class uppertester_geonetworking_factory
