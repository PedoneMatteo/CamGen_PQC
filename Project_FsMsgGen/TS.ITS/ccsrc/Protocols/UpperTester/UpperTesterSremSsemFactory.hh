/*!
 * \file      UpperTesterSremSsemFactory.hh
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

#include "UpperTesterSremSsemLayer.hh"

class UpperTesterSremSsemFactory : public layer_factory {
  static UpperTesterSremSsemFactory _f;

public:
  UpperTesterSremSsemFactory() {
    // Register factory
    layer_stack_builder::register_layer_factory("UT_SREMSSEM", this);
  };
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new UpperTesterSremSsemLayer(p_type, p_param); };
}; // End of class UpperTesterSremSsemFactory
