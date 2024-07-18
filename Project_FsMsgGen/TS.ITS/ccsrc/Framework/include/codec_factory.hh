/*!
 * \file      codec_factory.hh
 * \brief     Header file for ITS abstract protocol codec definition.
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

#include "codec.hh"

class Record_Type; //! TITAN forward declaration

/*!
 * \class codec_factory
 * \brief  This class provides a factory class to create codec class instances
 * \abstract
 */
class codec_factory {
public: //! \publicsection
  /*!
   * \fn codec();
   * \brief  Default constructor
   */
  codec_factory(){};
  /*!
   * \fn codec* create_codec(const std::string & type, const std::string & param);
   * \brief  Create the codecs stack based on the provided codecs stack description (cf. remark)
   * \param[in] p_type The provided codecs stack description
   * \param[in] p_params Optional parameters
   * \return 0 on success, -1 otherwise
   * \remark The description below introduces codecs stack in case of ITS project:
   *     HTTP(codecs=xml:held_codec;html:html_codec,json:json_codec)/TCP(debug=1,server=httpbin.org,port=80,use_ssl=0)
   * \pure
   */
  virtual codec<Record_Type, Record_Type> *create_codec() = 0;
}; // End of class codec_factory
