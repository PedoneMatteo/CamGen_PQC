/*!
 * \file      udp_layer.hh
 * \brief     Header file for ITS UDP/IP protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "udp_layer.hh"

#include "data_event_observer.hh"

/*!
 * \class lte_udp_layer
 * \brief  This class provides description of ITS UDP/IP protocol layer
 */
class lte_udp_layer : public udp_layer, public data_event_observer {
  using udp_layer::udp_layer; // Required for ctor/dtor inheritance mechanism

public: //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the lte_udp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  lte_udp_layer(const std::string &p_type, const std::string &p_param);
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the lte_udp_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  lte_udp_layer(const std::string &p_type, const params &p_param);
  /*!
   * \brief Default destructor
   */
  virtual ~lte_udp_layer() {};

  void receive_data(OCTETSTRING &data, params &params);

  inline void incoming_packet_observer_attach(data_event_notifier* p_observer) {
    _observers.push_back(p_observer);
  };

  inline void incoming_packet_observer_detach(data_event_notifier* p_observer) {
    _observers.erase(std::remove(_observers.begin(), _observers.end(), p_observer), _observers.end());
  };

  inline void incoming_packet_notify(OCTETSTRING &p_data, params &p_params) {
    for (auto o : _observers) {
      o->update(p_data, p_params);
    }
  };

}; // End of class lte_udp_layer
