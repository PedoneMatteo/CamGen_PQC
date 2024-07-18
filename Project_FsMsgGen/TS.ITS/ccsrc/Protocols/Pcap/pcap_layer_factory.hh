/*!
 * \file      pcap_layer_factory.hh
 * \brief     Header file for Pcap layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_stack_builder.hh"

#include "pcap_layer.hh"

/*!
 * \class pcap_layer_factory
 * \brief  This class provides a factory class to create an pcap_layer class instance
 */
class pcap_layer_factory : public layer_factory {
  static pcap_layer_factory _f; //! Reference to the unique instance of this class
public:                         //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the udp_layer_factory class
   * \remark The PCAP layer identifier is PCAP
   */
  pcap_layer_factory() {
    // register factory
    layer_stack_builder::register_layer_factory("PCAP", this);
  };
  /*!
   * \fn layer* create_layer(const std::string & type, const std::string & param);
   * \brief  Create the layers stack based on the provided layers stack description
   * \param[in] p_type The provided layers stack description
   * \param[in] p_params Optional parameters
   * \return 0 on success, -1 otherwise
   * \inline
   */
  inline virtual layer *create_layer(const std::string &p_type, const std::string &p_param) { return new pcap_layer(p_type, p_param); };
}; // End of class pcap_layer_factory
