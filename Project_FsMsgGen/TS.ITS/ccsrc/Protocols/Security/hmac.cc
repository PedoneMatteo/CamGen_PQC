/*!
 * \file      hmac.cc
 * \brief     Source file for HMAC helper methods.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <TTCN3.hh>

#include "hmac.hh"

#include "loggers.hh"

int hmac::generate(const OCTETSTRING p_buffer, const OCTETSTRING p_secret_key, OCTETSTRING &p_hmac) {
  // Sanity check
  if (p_buffer.lengthof() == 0) {
    return -1;
  }

  return generate(static_cast<const unsigned char *>(p_buffer), p_buffer.lengthof(), static_cast<const unsigned char *>(p_secret_key), p_secret_key.lengthof(),
                  p_hmac);
}

int hmac::generate(const unsigned char *p_buffer, const size_t p_buffer_length, const unsigned char *p_secret_key, const size_t p_secret_key_length,
                   OCTETSTRING &p_hmac) {
  // Sanity check
  if ((p_buffer == nullptr) || (p_secret_key == nullptr)) {
    return -1;
  }
  ::HMAC_CTX_reset(_ctx);

  p_hmac = int2oct(0, EVP_MAX_MD_SIZE);
  if (_hash_algorithms == hash_algorithms::sha_256) {
    ::HMAC_Init_ex(_ctx, (const void *)p_secret_key, (long unsigned int)p_secret_key_length, EVP_sha256(), NULL);
  } else if (_hash_algorithms == hash_algorithms::sha_384) {
    ::HMAC_Init_ex(_ctx, (const void *)p_secret_key, (long unsigned int)p_secret_key_length, EVP_sha384(), NULL);
  } else { // TODO To be continued
    return -1;
  }
  // Compute the hash value
  ::HMAC_Update(_ctx, p_buffer, p_buffer_length);
  unsigned int length = p_hmac.lengthof();
  ::HMAC_Final(_ctx, (unsigned char *)static_cast<const unsigned char *>(p_hmac), &length);
  loggers::get_instance().log_to_hexa("hmac::generate: ", (unsigned char *)static_cast<const unsigned char *>(p_hmac), length);
  // Resize the hmac
  if (_hash_algorithms == hash_algorithms::sha_256) {
    p_hmac = OCTETSTRING(16, static_cast<const unsigned char *>(p_hmac));
  } // FIXME Check length for the other hash algorithm

  return 0;
}
