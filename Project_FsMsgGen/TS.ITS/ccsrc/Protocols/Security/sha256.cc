/*!
 * \file      sha256.cc
 * \brief     Source file for SHA-256 helper methods.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <TTCN3.hh>

#include "sha256.hh"

int sha256::generate(const OCTETSTRING& p_buffer, OCTETSTRING& p_hash) {
  // Sanity check
  if (p_buffer.lengthof() == 0) {
    p_hash = get_sha256_empty_string();
    return 0;
  }

  return generate(static_cast<const unsigned char*>(p_buffer), p_buffer.lengthof(), p_hash);
}

int sha256::generate(const unsigned char* p_buffer, const size_t p_length, OCTETSTRING& p_hash) {
  // Sanity check
  if ((p_buffer == nullptr) || (p_length == 0)) {
    p_hash = get_sha256_empty_string();
    return 0;
  }

  // Resize data buffer
  p_hash = int2oct(0, SHA256_DIGEST_LENGTH);
  // Compute the hash value
  ::SHA256_Init(&_ctx);
  ::SHA256_Update(&_ctx, p_buffer, p_length);
  ::SHA256_Final((unsigned char*)static_cast<const unsigned char*>(p_hash), &_ctx);
  return 0;
};
