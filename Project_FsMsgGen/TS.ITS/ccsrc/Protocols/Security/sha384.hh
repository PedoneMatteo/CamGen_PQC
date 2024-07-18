/*!
 * \file      sha384.hh
 * \brief     Header file for SHA-384 helper methods.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <openssl/objects.h>
#include <openssl/sha.h>

class OCTETSTRING; //! TITAN forward declaration

/*!
 * \class sha384
 * \brief  This class provides description of SHA-384 helper methods
 */
class sha384 {
  SHA512_CTX _ctx; //! SHA context
public:            //! \publicsection
  /*!
   * \brief Default constructor
   *        Create a new instance of the sha384 class
   */
  explicit sha384() : _ctx{} {};
  /*!
   * \brief Default destructor
   */
  virtual ~sha384(){};

  /*!
   * \fn int generate(const OCTETSTRING& p_buffer, OCTETSTRING& p_hash);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_buffer The data used to generate the SHA-384 hash
   * \param[out] p_hash The SHA-384 hash value based of the provided data
   * \return 0 on success, -1 otherwise
   */
  int generate(const OCTETSTRING &p_buffer, OCTETSTRING &p_hash);

  /*!
   * \fn int generate(const OCTETSTRING p_buffer, OCTETSTRING& p_hash);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_buffer The data used to generate the SHA-384 hash
   * \param[in] The length of the data buffer
   * \param[out] p_hash The SHA-384 hash value based of the provided data
   * \return 0 on success, -1 otherwise
   */
  int generate(const unsigned char *p_buffer, const size_t p_length, OCTETSTRING &p_hash);

  /*!
   * \fn const OCTETSTRING get_sha384_empty_string() const;
   * \brief Return the SHA-384 of an empty string
   * \return The SHA-384 of an empty string
   */
  const OCTETSTRING get_sha384_empty_string() const;
}; // End of class sha384
