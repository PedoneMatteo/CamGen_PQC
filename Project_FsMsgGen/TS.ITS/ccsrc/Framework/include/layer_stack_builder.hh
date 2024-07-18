/*!
 * \file      layer_stack_builder.hh
 * \brief     Header file for ITS protocol stack builder.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "layer_factory.hh"

/*!
 * \class layer_stack_builder
 * \brief  This class provides a factory class to create Layer class instances
 */
class layer_stack_builder {
private: //! \privatesection
  typedef std::map<std::string, layer_factory *> LayerFactoryMap;

  static layer_stack_builder *           _instance;        //! Smart pointer to the unique instance of the logger framework
  std::map<std::string, layer_factory *> _layer_factories; //! The list of the registered \see t_layer factories

  /*!
   * \brief Default constructor
   *        Create a new instance of the layer_stack_builder class
   * \private
   */
  layer_stack_builder(); // can not be created manually
public:                  //! \publicsection
  /*!
   * \fn layer_stack_builder* get_instance();
   * \brief Accessor for the unique instance of the logger framework
   * \static
   */
  static layer_stack_builder *get_instance();

  /*!
   * \fn void register_layer_factory(const std::string & p_type, layer_factory* p_layer_factory);
   * \brief Add a new layer factory
   * \param[in] p_type          The layer identifier (e.g. GN for the GeoNetworking layer...)
   * \param[in] p_layer_factory A reference to the \see layer_factory
   * \static
   */
  static void register_layer_factory(const std::string &p_type, layer_factory *p_layer_factory);

private: //! \privatesection
  /*!
   * \fn void _register_layer_factory(const std::string & p_type, layer_factory* p_layer_factory);
   * \brief Add a new layer factory
   * \param[in] p_type          The layer identifier (e.g. GN for the GeoNetworking layer...)
   * \param[in] p_layer_factory A reference to the \see layer_factory
   */
  void _register_layer_factory(const std::string &p_type, layer_factory *p_layer_factory);

public: //! \publicsection
  /*!
   * \fn layer* create_layer_stack(const char* p_layer_stack_description);
   * \brief Add a new layer factory
   * \param[in] p_layer_stack_description A textual description of the layer to create
   * \return The created layer object on success, nullptr otherwise
   */
  layer *create_layer_stack(const char *p_layer_stack_description);
}; // End of class layer_stack_builder
