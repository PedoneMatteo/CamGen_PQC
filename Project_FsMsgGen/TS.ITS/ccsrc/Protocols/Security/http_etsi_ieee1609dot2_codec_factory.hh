/*!
 * \file      http_etsi_ieee1609dot2_codec_factory.hh
 * \brief     Header file for ITS over HTTP protocols codec factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "codec_stack_builder.hh"

#include "http_etsi_ieee1609dot2_codec.hh"

class Record_Type; //! TITAN forward declaration

/*!
 * \class http_etsi_ieee1609dot2_codec_factory
 * \brief  This class provides a factory class to create an http_etsi_ieee1609dot2_codec class instance
 */
class http_etsi_ieee1609dot2_codec_factory : public codec_factory {
  static http_etsi_ieee1609dot2_codec_factory _f; //! Reference to the unique instance of this class
public:                                           //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the http_etsi_ieee1609dot2_codec_factory class
   * \remark The HELD/IP codec identifier is HELD
   */
  http_etsi_ieee1609dot2_codec_factory() {
    // register factory
    codec_stack_builder::register_codec_factory("http_etsi_ieee1609dot2_codec", this);
  };
  /*!
   * \fn codec* create_codec(const std::string & type, const std::string & param);
   * \brief  Create the codecs stack based on the provided codecs stack description
   * \param[in] p_type The provided codecs stack description
   * \param[in] p_params Optional parameters
   * \return 0 on success, -1 otherwise
   * \inline
   */
  inline virtual codec<Record_Type, Record_Type> *create_codec() { return (codec<Record_Type, Record_Type> *)new http_etsi_ieee1609dot2_codec(); };
}; // End of class http_etsi_ieee1609dot2_codec_factory
