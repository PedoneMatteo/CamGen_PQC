/*!
 * \file      ivim_layer_factory.hh
 * \brief     Header file for ITS IVIM protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "ivim_layer.hh"

class ivim_layer_factory : public layer_factory {
  static ivim_layer_factory _f;

public:
  ivim_layer_factory() {
    // Register factory
    layer_stack_builder::register_layer_factory("IVIM", this);
  };
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new ivim_layer(p_type, p_param); };
}; // End of class ivim_layer_factory
