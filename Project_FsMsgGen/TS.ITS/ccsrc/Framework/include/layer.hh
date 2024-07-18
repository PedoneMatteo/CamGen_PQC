/*!
 * \file      layer.hh
 * \brief     Header file for ITS abstract protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "params.hh"

class OCTETSTRING; //! Forward declaration of TITAN class
class BITSTRING;   //! Forward declaration of TITAN class
class CHARSTRING;  //! Forward declaration of TITAN class
class INTEGER;     //! Forward declaration of TITAN class

/*!
 * \class layer
 * \brief  This class provides basic description of an ITS protocol layer
 */
class layer {
  std::vector<layer *> upperLayers; //! List of the upper protocol layers
  std::vector<layer *> lowerLayers; //! List of the lower protocol layers

protected:
  std::string type; //! Type description, it indicates the protocol type (e.g. CAM, DENM, GN, ETH, PCAP...)

public: //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the layer class
   */
  explicit layer() : upperLayers(), lowerLayers(), type(std::string("")){};

  /*!
   * \brief Specialized constructor
   *        Create a new instance of the layer class with its type description
   * \param[in] p_type The port type name (e.g. GN for the GeoNetworking layer)
   * \remark This constructor is called by the layer factory
   * \see layer_factory
   */
  explicit layer(const std::string &p_type) : upperLayers(), lowerLayers(), type(std::string(p_type.begin(), p_type.end())){};

  /*!
   * \brief Default destructor
   * \todo Remove logs
   */
  virtual ~layer() {
    // Double linked list, only remove layers in lowerLayers from the lowest one
    std::for_each(lowerLayers.rbegin(), lowerLayers.rend(), [](layer *it) { delete it; });
    lowerLayers.clear();
    upperLayers.clear();
  };

  /*!
   * \fn void delete_layer();
   * \brief Delete this layer
   * \todo To be implemented
   */
  void delete_layer(){};

public: //! \publicsection
  /*!
   * \inline
   * \fn void add_upper_layer(layer* p_layer);
   * \brief Add a new layer in the list of the upper layer
   * \param[in] p_layer The layer protocol to be removed
   */
  inline void add_upper_layer(layer *p_layer) {
    if (p_layer != NULL) {
      upperLayers.push_back(p_layer);
      p_layer->lowerLayers.push_back(this);
    };
  };

  /*!
   * \fn void remove_upper_layer(layer* p_layer);
   * \brief Remove the specified upper layer protocol from the list of the upper layer
   * \param[in] p_layer The layer protocol to be removed
   * \todo To be implemented
   */
  void remove_upper_layer(layer *p_layer){};

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   * \todo Remove the logs
   * \virtual
   */
  virtual void send_data(OCTETSTRING &p_data, params &p_params){};

  /*!
   * \virtual
   * \fn void receive_data(OCTETSTRING& data, params& params);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_data The bytes formated data received
   * \param[in] p_params Some lower layers parameters values when data was received
   * \todo Remove the logs
   * \virtual
   */
  virtual void receive_data(OCTETSTRING &p_data, params &p_params) {}

  /*!
   * \inline
   * \fn const std::string& to_string();
   * \brief Remove the specified upper layer protocol from the list of the upper layer
   * \param[in] The layer protocol to be removed
   */
  inline const std::string &to_string() const { return type; };

protected: //! \protectedsection
  inline void to_all_layers(std::vector<layer *> &layers, OCTETSTRING &data, params &params) {
    for (std::vector<layer *>::const_iterator it = layers.cbegin(); it != layers.cend(); ++it) {
      layer *p = *it;
      p->receive_data(data, params); // FIXME BUG I
    }                                // End of 'for' statement
  };

  inline void receive_to_all_layers(OCTETSTRING &data, params &params) {
    for (std::vector<layer *>::const_iterator it = upperLayers.cbegin(); it != upperLayers.cend(); ++it) {
      layer *p = *it;
      p->receive_data(data, params);
    } // End of 'for' statement
  };

  inline void send_to_all_layers(OCTETSTRING &data, params &params) {
    for (std::vector<layer *>::const_iterator it = lowerLayers.cbegin(); it != lowerLayers.cend(); ++it) {
      layer *p = *it;
      p->send_data(data, params);
    } // End of 'for' statement
  };
}; // End of class layer
