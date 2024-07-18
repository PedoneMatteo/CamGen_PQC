/*!
 * \file      lte_layer.hh
 * \brief     Header file for 3GPP V2X protocol layer definition (PC5 & Uu).
 * \author    ETSI TTF T011
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "ethernet_layer.hh"
#include "qualcomm_layer.hh"

#include "data_event_notifier.hh"

/*!
 * \class lte_layer
 * \brief  This class provides description of ITS UDP/IP protocol layer
 */
class lte_layer : public layer, public data_event_notifier {
  params _params; //! Layer parameters
  std::string _uu_transport;
  std::string _uu_protocol;
  std::string _pc5_layer_id;
  uu_interface* _uu_layer;
  layer* _pc5_layer;
  qualcomm_layer* _pc5;
  std::string _time_key; //! \todo

  public: //! \publicsection
    /*!
     * \brief Specialised constructor
     *        Create a new instance of the lte_layer class
     * \param[in] p_type \todo
     * \param[in] p_param \todo
     */
    lte_layer(const std::string &p_type, const std::string &p_param);
    /*!
     * \brief Default destructor
     */
    virtual ~lte_layer();

    /*!
     * \virtual
     * \fn void send_data(OCTETSTRING& data, params& params);
     * \brief Send bytes formated data to the lower layers
     * \param[in] p_data The data to be sent
     * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
     */
    virtual void send_data(OCTETSTRING &data, params &params);
    /*!
     * \virtual
     * \fn void receive_data(OCTETSTRING& data, params& params);
     * \brief Receive bytes formated data from the lower layers
     * \param[in] p_data The bytes formated data received
     * \param[in] p_params Some lower layers parameters values when data was received
     */
    virtual void receive_data(OCTETSTRING &data, params &info);

    virtual void update(OCTETSTRING &p_data, params &p_params);

    void Handle_Fd_Event_Readable(int fd);

  private:
    void          close();
    void          setup_raw_v2x();
    void          setup_mqtt_v2x();
    void          setup_amqp_v2x();
}; // End of class lte_layer
