/*!
 * \file      codec_stack_builder.hh
 * \brief     Header file for ITS protocol stack builder.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "codec_factory.hh"

class Record_Type; //! TITAN forward declaration

/*!
 * \class codec_stack_builder
 * \brief  This class provides a factory class to create Codec class instances
 */
class codec_stack_builder {
private:                                            //! \privatesection
  static codec_stack_builder *           _instance; //! Smart pointer to the unique instance of the logger framework
  std::map<std::string, codec_factory *> _codecs;   //! The list of the registered \see t_codec factories

  /*!
   * \brief Default constructor
   *        Create a new instance of the codec_stack_builder class
   * \private
   */
  codec_stack_builder(){}; // can not be created manually
public:                    //! \publicsection
  /*!
   * \fn codec_stack_builder* get_instance();
   * \brief Accessor for the unique instance of the logger framework
   * \static
   */
  static codec_stack_builder *get_instance() { return _instance ? _instance : _instance = new codec_stack_builder(); };

  /*!
   * \fn void register_codec_factory(const std::string & p_type, codec_factory<TPDUEnc, TPDUDec>* p_codec_factory);
   * \brief Add a new codec factory
   * \param[in] p_type          The codec identifier (e.g. GN for the GeoNetworking codec...)
   * \param[in] p_codec_factory A reference to the \see codec_factory
   * \static
   */
  static void register_codec_factory(const std::string &p_type, codec_factory *p_codec_factory) {
    codec_stack_builder::get_instance()->_register_codec_factory(p_type, p_codec_factory);
  };

private: //! \privatesection
  /*!
   * \fn void _register_codec_factory(const std::string & p_type, codec_factory<TPDUEnc, TPDUDec>* p_codec_factory);
   * \brief Add a new codec factory
   * \param[in] p_type          The codec identifier (e.g. GN for the GeoNetworking codec...)
   * \param[in] p_codec_factory A reference to the \see codec_factory
   */
  void _register_codec_factory(const std::string &p_type, codec_factory *p_codec_factory) { _codecs[p_type] = p_codec_factory; };

public: //! \publicsection
  /*!
   * \fn codec* get_codec(const char* p_codec_name);
   * \brief Retrieve the specified codec name from the list of the registered codecs
   * \param[in] p_codec_name The codec indentifier
   * \return The pointer to the codec object on success, nullptr otherwise
   */
  inline codec<Record_Type, Record_Type> *get_codec(const char *p_codec_name) { // NOTE A virtual method cannot not be a template ==> polymorphism required here
    typename std::map<std::string, codec_factory *>::const_iterator it = _codecs.find(p_codec_name);
    if (it != _codecs.cend()) {
      return it->second->create_codec();
    }

    return nullptr;
  }
}; // End of class codec_stack_builder
