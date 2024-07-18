/*!
 * \file      sha384.cc
 * \brief     Sorce file for SHA-384 helper methods.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <TTCN3.hh>

#include <TTCN3.hh>

#include "sha384.hh"

int sha384::generate(const OCTETSTRING &p_buffer, OCTETSTRING &p_hash) {
  // Sanity check
  if (p_buffer.lengthof() == 0) {
    p_hash = get_sha384_empty_string();
    return 0;
  }

  return generate(static_cast<const unsigned char *>(p_buffer), p_buffer.lengthof(), p_hash);
}

int sha384::generate(const unsigned char *p_buffer, const size_t p_length, OCTETSTRING &p_hash) {
  // Sanity check
  if ((p_buffer == nullptr) || (p_length == 0)) {
    p_hash = get_sha384_empty_string();
    return 0;
  }

  // Resize data buffer
  p_hash = int2oct(0, SHA384_DIGEST_LENGTH);
  // Compute the hash value
  ::SHA384_Init(&_ctx);
  ::SHA384_Update(&_ctx, p_buffer, p_length);
  ::SHA384_Final((unsigned char *)static_cast<const unsigned char *>(p_hash), &_ctx);
  return 0;
}

const OCTETSTRING sha384::get_sha384_empty_string() const {
  static unsigned char
    sha384_empty_string[] = {0x38, 0xb0, 0x60, 0xa7, 0x51, 0xac, 0x96, 0x38, 0x4c, 0xd9, 0x32, 0x7e, 0xb1, 0xb1, 0xe3, 0x6a,
                             0x21, 0xfd, 0xb7, 0x11, 0x14, 0xbe, 0x07, 0x43, 0x4c, 0x0c, 0xc7, 0xbf, 0x63, 0xf6, 0xe1, 0xda,
                             0x27, 0x4e, 0xde, 0xbf, 0xe7, 0x6f, 0x65, 0xfb, 0xd5, 0x1a, 0xd2, 0xf1, 0x48, 0x98, 0xb9, 0x5b}; //! SHA-384 of an empty string
  return OCTETSTRING(48, sha384_empty_string);
}
