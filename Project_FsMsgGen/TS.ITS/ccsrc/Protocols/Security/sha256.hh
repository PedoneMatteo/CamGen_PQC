/*!
 * \file      sha256.hh
 * \brief     Header file for SHA-256 helper methods.
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
 * \class sha256
 * \brief  This class provides description of SHA-256 helper methods
 */
class sha256 {
  SHA256_CTX _ctx; //! SHA context
public:
  /*!
   * \brief Default constructor
   *        Create a new instance of the sha256 class
   */
  explicit sha256() : _ctx{} {};
  /*!
   * \brief Default destructor
   */
  virtual ~sha256(){};

  /*!
   * \fn int generate(const OCTETSTRING& p_buffer, OCTETSTRING& p_hash);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_buffer The data used to generate the SHA-256 hash
   * \param[out] p_hash The SHA-256 hash value based of the provided data
   * \return 0 on success, -1 otherwise
   */
  int generate(const OCTETSTRING &p_buffer, OCTETSTRING &p_hash);

  /*!
   * \fn int generate(const unsigned char* p_buffer, OCTETSTRING& p_hash);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_buffer The data used to generate the SHA-256 hash
   * \param[in] The length of the data buffer
   * \param[out] p_hash The SHA-256 hash value based of the provided data
   * \return 0 on success, -1 otherwise
   */
  int generate(const unsigned char *p_buffer, const size_t p_length, OCTETSTRING &p_hash);

  /*!
   * \fn const OCTETSTRING get_sha256_empty_string() const;
   * \brief Return the SHA-256 of an empty string
   * \return The SHA-256 of an empty string
   */
  inline const OCTETSTRING get_sha256_empty_string() const {
    static unsigned char sha256_empty_string[] = {
      0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
      0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55}; //! SHA-256 of an empty string
    return OCTETSTRING(32, sha256_empty_string);
  };

}; // End of class sha256
