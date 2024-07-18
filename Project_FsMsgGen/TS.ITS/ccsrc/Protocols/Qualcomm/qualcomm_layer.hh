/*!
 * \file      udp_layer_factory.hh
 * \brief     Header file for ITS Qualcomm port protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "params.hh"
#include "t_layer.hh"

#include "data_event_observer.hh"

class OCTETSTRING; //! Forward declaration of TITAN class

/*!
 * \class qualcomm_layer
 * \brief  This class provides description of ITS Qualcomm port protocol layer
 */
class qualcomm_layer : public layer, public data_event_observer {

  params _params; //! Layer parameters

public:
  //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the qualcomm_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  qualcomm_layer(const std::string &p_type, const std::string &param);
  /*!
   * \brief Default destructor
   * \remark If \see _reconnect_on_send is set to false, the disconnection is done by the destructor
   */
  virtual ~qualcomm_layer(){};

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);

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

}; // End of class qualcomm_layer
