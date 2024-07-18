/*!
 * \file      codec.hh
 * \brief     Header file for ITS abstract codec definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "params.hh"

class OCTETSTRING; //! Declare TITAN class
class CHARSTRING;  //! Declare TITAN class
class BITSTRING;   //! Declare TITAN class

/*!
 * \class codec
 * \brief  This class provides the interface for all ITS codecs, include UT and AC codecs
 * \abstract
 */
template <typename TPDUEnc, typename TPDUDec> class codec {
protected:
  params *_params; //! Reference to params stack
                   // \todo Use smart pointer std::unique_ptr<params>

public: //! \publicsection
  /*!
   * \fn codec();
   * \brief  Default constructor
   * \todo Remove logs
   */
  explicit codec() : _params(nullptr){};
  /*!
   * \fn ~codec();
   * \brief  Default destructor
   * \virtual
   * \todo Remove logs
   */
  virtual ~codec(){};
  /*!
   * \fn int encode(const TPDUEnc& msg, OCTETSTRING& data);
   * \brief  Encode typed message into an octet string
   * \param[in] p_message The typed message to be encoded
   * \param[out] p_data The encoding result
   * \return 0 on success, -1 otherwise
   * \pure
   */
  virtual int encode(const TPDUEnc &p_message, OCTETSTRING &p_data) = 0;
  /*!
   * \fn int decode(const OCTETSTRING& p_, TPDUDec& p_message, params* p_params = NULL);
   * \brief  Encode typed message into an octet string format
   * \param[in] p_data The message in its octet string
   * \param[out] p_message The decoded typed message
   * \return 0 on success, -1 otherwise
   * \pure
   */
  virtual int decode(const OCTETSTRING &p_, TPDUDec &p_message, params *p_params = NULL) = 0;
}; // End of class codec
