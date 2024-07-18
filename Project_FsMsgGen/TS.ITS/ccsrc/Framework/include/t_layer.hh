/*!
 * \file      t_layer.hh
 * \brief     Header file for ITS abstract protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer.hh"

/*!
 * \class t_layer
 * \brief  This class provides basic description of an ITS port protocol layer.
 *         A port protocol layer is the final layer which provides the access to the physical communication channel
 *         A port protocol layer derives from both a layer class and a template port class
 */
template <typename TPort> class t_layer : public layer {
  typedef std::vector<TPort *>                    TPortList;
  typedef typename std::vector<TPort *>::iterator TPortListIterator;

  TPortList upperPorts; //! The list of the upper ports

public: //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the t_layer class
   * \todo Remove logs
   */
  explicit t_layer() : layer(), upperPorts(){};

  /*!
   * \brief Specialized constructor
   *        Create a new instance of the layer class with its type description
   * \param[in] p_type The port type name (e.g. TCP for the TCP sockect based layer)
   * \remark This constructor is called by the layer factory
   * \see layer_factory
   */
  explicit t_layer(const std::string &p_type) : layer(p_type), upperPorts(){};
  /*!
   * \inline
   * \fn void add_upper_port(TPort * p_port);
   * \brief Add a new upper port layer
   * \todo To be done
   */
  inline void add_upper_port(TPort *p_port) { upperPorts.push_back(p_port); };
  /*!
   * \fn void remove_upper_port(TPort*);
   * \brief Remove the specified upper layer port protocol from the list of the upper layers
   * \param[in] p_layer The layer protocol to be removed
   */
  void remove_upper_port(TPort *);

protected: //! \protectedsection
  /*!
   * \inline
   * \fn void to_all_upper_ports(const TMessage& m, const params& param);
   * \brief Forward the message to all available upper port layers
   * \param[in] p_message The message to be forwarded
   * \param[in] p_params Some lower layers parameters values when data was received
   */
  template <typename TMessage> inline void to_all_upper_ports(const TMessage &p_message, const params &p_params) {
    for (TPortListIterator it = upperPorts.begin(); it < upperPorts.end(); ++it) {
      (*it)->receiveMsg(p_message, p_params);
    }
  }
}; // End of class t_layer
