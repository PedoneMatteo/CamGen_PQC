/*!
 * \file      hmac.hh
 * \brief     Header file for HMAC helper methods.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <vector>

#include <openssl/hmac.h>

class OCTETSTRING; //! TITAN forward declaration

/*!
 * \enum Supported hash algorithms
 */
enum class hash_algorithms : unsigned char {
  sha_256, /*!< HMAC with SHA-256 */
  sha_384  /*!< HMAC with SHA-384 */
};         // End of class hash_algorithms

/*!
 * \class hmac
 * \brief  This class provides description of HMAC helper methods
 */
class hmac {
  HMAC_CTX *      _ctx;             //! HMAC context
  hash_algorithms _hash_algorithms; //! HMAC hash algorithm to use
public:
  /*!
   * \brief Default constructor
   *        Create a new instance of the hmac class
   * \param[in] p_hash_algorithms The hash algorithm to be used to compute the HMAC. Default: sha_256
   */
  hmac(const hash_algorithms p_hash_algorithms = hash_algorithms::sha_256) : _ctx{::HMAC_CTX_new()}, _hash_algorithms(p_hash_algorithms){};
  /*!
   * \brief Default destructor
   */
  virtual ~hmac() {
    if (_ctx != nullptr) {
      ::HMAC_CTX_free(_ctx);
    };
  };

  /*!
   * \fn int generate(const OCTETSTRING p_buffer, const OCTETSTRING p_secret_key, OCTETSTRING& p_hmac);
   * \brief Generate the HMAC of data using a secret key
   * \Param[in] p_buffer The data tobe hashed
   * \param[in] p_secret_key The secret key to be used to generate the HMAC
   * \param[out] p_hmac The HMAC value based of the provided data
   * \return 0 on success, -1 otherwise
   */
  int generate(const OCTETSTRING p_buffer, const OCTETSTRING p_secret_key, OCTETSTRING &p_hmac); // TODO Use reference &

  /*!
   * \fn int generate(const unsigned char* p_buffer, const size_t p_buffer_length, const unsigned char* p_secret_key, const size_t p_secret_key_length,
   * OCTETSTRING& p_hmac); \brief Generate the HMAC of data using a secret key \param[in] p_buffer The data to be hashed \param[in] p_buffer_length The size of
   * the data \param[in] p_secret_key The secret key to be used to generate the HMAC \param[in] p_secret_key_length The size of the secret key \param[out]
   * p_hmac The HMAC value based of the provided data \return 0 on success, -1 otherwise
   */
  int generate(const unsigned char *p_buffer, const size_t p_buffer_length, const unsigned char *p_secret_key, const size_t p_secret_key_length,
               OCTETSTRING &p_hmac);

}; // End of class hmac
