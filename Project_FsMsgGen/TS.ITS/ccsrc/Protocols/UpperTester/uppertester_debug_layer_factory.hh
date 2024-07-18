/*!
 * \file      uppertester_debug_layer_factory.hh
 * \brief     Header file for ITS UpperTester Debug protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.o
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "uppertester_debug_layer.hh"

/*!
 * \class uppertester_debug_layer_factory
 * \brief  This class provides a factory class to create an uppertester_debug_layer class instance
 */
class uppertester_debug_layer_factory : public layer_factory {
  static uppertester_debug_layer_factory _f; //! Reference to the unique instance of this class
public:                                      //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the uppertester_debug_layer_factory class
   * \remark The UPPERTESTER_DEBUG/IP layer identifier is UPPERTESTER_DEBUG
   */
  uppertester_debug_layer_factory() {
    // register factory
    layer_stack_builder::register_layer_factory("DEBUG", this);
  };
  /*!
   * \fn layer* create_layer(const std::string & type, const std::string & param);
   * \brief  Create the layers stack based on the provided layers stack description
   * \param[in] p_type The provided layers stack description
   * \param[in] p_params Optional parameters
   * \return 0 on success, -1 otherwise
   * \inline
   */
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new uppertester_debug_layer(p_type, p_param); };
}; // End of class uppertester_debug_layer_factory
