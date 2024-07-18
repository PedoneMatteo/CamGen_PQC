/*!
 * \file      security_ecc.cc
 * \brief     Source file for Elliptic Curve Cryptography.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <TTCN3.hh>

#include <openssl/ecdsa.h>
#include <openssl/rand.h>

#include "sha256.hh"

#include "hmac.hh"
#include "security_ecc.hh"

#include "loggers.hh"

#if OPENSSL_VERSION_NUMBER >= 0x10101000L
#define EC_POINT_get_affine_coordinates_GFp EC_POINT_get_affine_coordinates
#define EC_POINT_get_affine_coordinates_GF2m EC_POINT_get_affine_coordinates
#define EC_POINT_set_affine_coordinates_GFp EC_POINT_set_affine_coordinates
#define EC_POINT_set_affine_coordinates_GF2m EC_POINT_set_affine_coordinates
#define EC_POINT_set_compressed_coordinates_GFp EC_POINT_set_compressed_coordinates
#define EC_POINT_set_compressed_coordinates_GF2m EC_POINT_set_compressed_coordinates
#endif

security_ecc::security_ecc(const ec_elliptic_curves p_elliptic_curve)
  : _elliptic_curve(p_elliptic_curve), _encryption_algotithm(encryption_algotithm::aes_128_ccm), _ec_key(nullptr), _ec_group(nullptr), _bn_ctx(nullptr),
    _pri_key(), _pub_key_x(), _pub_key_y(), _pub_key_compressed(), _pub_key_compressed_mode{ecc_compressed_mode::compressed_y_0}, _secret_key(), _enc_key_x(),
    _enc_key_y(), _sym_key(), _nonce(), _tag() {
  loggers::get_instance().log(">>> security_ecc::security_ecc: %d", static_cast<int>(p_elliptic_curve));

  const int result = init();
  if (result == -1) {
    loggers::get_instance().error("security_ecc::security_ecc: Unsupported elliptic_curve %d", _elliptic_curve);
  }
} // End of constructor

security_ecc::security_ecc(const ec_elliptic_curves p_elliptic_curve, const OCTETSTRING &p_private_key)
  : _elliptic_curve(p_elliptic_curve), _encryption_algotithm(encryption_algotithm::aes_128_ccm), _ec_key(nullptr), _ec_group(nullptr), _bn_ctx(nullptr),
    _pri_key(p_private_key), _pub_key_x(), _pub_key_y(), _pub_key_compressed(), _pub_key_compressed_mode{ecc_compressed_mode::compressed_y_0}, _secret_key(),
    _enc_key_x(), _enc_key_y(), _sym_key(), _nonce(), _tag() {
  loggers::get_instance().log(">>> security_ecc::security_ecc (1): %d", static_cast<int>(p_elliptic_curve));

  // Sanity checks
  if ((_elliptic_curve == ec_elliptic_curves::nist_p_256) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_256_r1)) {
    if (p_private_key.lengthof() != 32) {
      loggers::get_instance().error("security_ecc::security_ecc (1): Invalid public keys size");
    }
  } else if ((_elliptic_curve == ec_elliptic_curves::nist_p_384) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_384_r1)) {
    if ((p_private_key.lengthof() != 48)) {
      loggers::get_instance().error("security_ecc::security_ecc (1): Invalid public keys size");
    }
  }

  int result = init();
  if (result == -1) {
    loggers::get_instance().error("security_ecc::security_ecc (1): Unsupported elliptic_curve %d", _elliptic_curve);
  }
  ::EC_KEY_set_conv_form(_ec_key, POINT_CONVERSION_UNCOMPRESSED);

  // Build private key
  BIGNUM *p = ::BN_new();
  ::BN_bin2bn(static_cast<const unsigned char *>(_pri_key), _pri_key.lengthof(), p);
  // Build public keys
  EC_POINT *ec_point = ::EC_POINT_new(_ec_group);
  ::EC_POINT_mul(_ec_group, ec_point, p, nullptr, nullptr, _bn_ctx);
  // Set private key
  ::EC_KEY_set_private_key(_ec_key, p);
  if (::EC_KEY_check_key(_ec_key) != 0) {
    ::BN_clear_free(p);
    loggers::get_instance().error("security_ecc::security_ecc (1): Invalid private key");
  }
  ::BN_clear_free(p);
  p = nullptr;
  // Private key is correct, set public keys
  ::EC_KEY_set_public_key(_ec_key, ec_point);

  BIGNUM *xy = ::BN_new();
  ::EC_POINT_point2bn(_ec_group, ec_point, POINT_CONVERSION_UNCOMPRESSED, xy, _bn_ctx);
  if (BN_num_bytes(xy) == 0) {
    ::BN_clear_free(xy);
    loggers::get_instance().error("security_ecc::security_ecc (1): Failed to generate xy coordinates, check algorithms");
  }
  loggers::get_instance().log("security_ecc::security_ecc (1): xy length: %d", BN_num_bytes(xy));
  OCTETSTRING v = int2oct(0, BN_num_bytes(xy));
  ::BN_bn2bin(xy, (unsigned char *)static_cast<const unsigned char *>(v));
  if ((v.lengthof() % 2) != 0) {
    // Remove first byte
    loggers::get_instance().log_msg("security_ecc::security_ecc (1): Complete xy=", v);
    v = OCTETSTRING(v.lengthof() - 1, 1 + static_cast<const unsigned char *>(v));
  }
  ::BN_clear_free(xy);
  xy = nullptr;

  loggers::get_instance().log_msg("security_ecc::security_ecc (1): xy=", v);
  const int l = v.lengthof() / 2;
  _pub_key_x  = OCTETSTRING(l, static_cast<const unsigned char *>(v));
  _pub_key_y  = OCTETSTRING(l, l + static_cast<const unsigned char *>(v));

  // Compressed
  int len = ::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, nullptr, 0, _bn_ctx);
  if (len == 0) {
    loggers::get_instance().warning("security_ecc::security_ecc (1): Failed to generate x_coordinate compressed key");
    _pub_key_compressed = OCTETSTRING(0, nullptr);
  } else {
    _pub_key_compressed = int2oct(0, len);
    if (::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, (unsigned char *)static_cast<const unsigned char *>(_pub_key_compressed), len,
                             _bn_ctx) == 0) {
      loggers::get_instance().warning("security_ecc::security_ecc (1): Failed to generate x_coordinate compressed key");
      _pub_key_compressed = OCTETSTRING(0, nullptr);
    } else { // Remove first byte
      loggers::get_instance().log_msg("security_ecc::security_ecc (1): Complete _pub_key_compressed=", _pub_key_compressed);
      _pub_key_compressed_mode = ((v[0].get_octet() & 0x01) == 0x00) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1;
      _pub_key_compressed      = OCTETSTRING(_pub_key_compressed.lengthof() - 1, 1 + static_cast<const unsigned char *>(_pub_key_compressed));
    }
  }
  ::EC_POINT_free(ec_point);
  loggers::get_instance().log_msg("security_ecc::security_ecc (1): _pri_key=", _pri_key);
  loggers::get_instance().log_msg("security_ecc::security_ecc (1): _pub_key_x=", _pub_key_x);
  loggers::get_instance().log_msg("security_ecc::security_ecc (1): _pub_key_y=", _pub_key_y);
  loggers::get_instance().log_msg("security_ecc::security_ecc (1): _pub_key_compressed=", _pub_key_compressed);
  loggers::get_instance().log("security_ecc::security_ecc (1): _pub_key_compressed_mode=%d", _pub_key_compressed_mode);
} // End of constructor

security_ecc::security_ecc(const ec_elliptic_curves p_elliptic_curve, const OCTETSTRING &p_public_key_x, const OCTETSTRING &p_public_key_y)
  : _elliptic_curve(p_elliptic_curve), _encryption_algotithm(encryption_algotithm::aes_128_ccm), _ec_key(nullptr), _ec_group(nullptr), _bn_ctx(nullptr),
    _pri_key(), _pub_key_x(p_public_key_x), _pub_key_y(p_public_key_y), _pub_key_compressed(), _pub_key_compressed_mode{ecc_compressed_mode::compressed_y_0},
    _secret_key(), _enc_key_x(), _enc_key_y(), _sym_key(), _nonce(), _tag() {
  loggers::get_instance().log(">>> security_ecc::security_ecc (2): %d", static_cast<int>(p_elliptic_curve));

  // Sanity checks
  if ((_elliptic_curve == ec_elliptic_curves::nist_p_256) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_256_r1)) {
    if ((p_public_key_x.lengthof() != 32) || (p_public_key_y.lengthof() != 32)) {
      loggers::get_instance().error("security_ecc::security_ecc (2): Invalid public keys size");
    }
  } else if ((_elliptic_curve == ec_elliptic_curves::nist_p_384) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_384_r1)) {
    if ((p_public_key_x.lengthof() != 48) || (p_public_key_y.lengthof() != 48)) {
      loggers::get_instance().error("security_ecc::security_ecc (2): Invalid public keys size");
    }
  }

  int result = init();
  if (result == -1) {
    loggers::get_instance().error("security_ecc::security_ecc (2): Unsupported elliptic_curve %d", _elliptic_curve);
  }
  ::EC_KEY_set_conv_form(_ec_key, POINT_CONVERSION_UNCOMPRESSED);

  // Set public key
  BIGNUM *x = ::BN_new();
  ::BN_bin2bn(static_cast<const unsigned char *>(_pub_key_x), _pub_key_x.lengthof(), x);
  BIGNUM *y = ::BN_new();
  ::BN_bin2bn(static_cast<const unsigned char *>(_pub_key_y), _pub_key_y.lengthof(), y);
  EC_POINT *ec_point = ::EC_POINT_new(_ec_group);
  result             = 0;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use primary
                                         // No break;
    case ec_elliptic_curves::nist_p_384:
                                         // No break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      // No break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      result = ::EC_POINT_set_affine_coordinates_GFp(_ec_group, ec_point, x, y, _bn_ctx); // Use primary elliptic curve
      break;
    default:      // Use Binary
      result = 0; // FIXME Does not compile on Windows ::EC_POINT_set_affine_coordinates_GF2m(_ec_group, ec_point, x, y, _bn_ctx);
  }               // End of 'switch' statement
  if (result == 0) {
    ::BN_clear_free(x);
    ::BN_clear_free(y);
    loggers::get_instance().error("security_ecc::security_ecc (2): Failed to get coordinates");
  }
  ::BN_clear_free(x);
  x = nullptr;
  ::BN_clear_free(y);
  y = nullptr;

  ::EC_KEY_set_public_key(_ec_key, ec_point);

  // Compressed
  int len = ::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, nullptr, 0, _bn_ctx);
  if (len == 0) {
    loggers::get_instance().warning("security_ecc::security_ecc (2): Failed to generate x_coordinate compressed key");
  }
  _pub_key_compressed = int2oct(0, len);
  if (::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, (unsigned char *)static_cast<const unsigned char *>(_pub_key_compressed), len,
                           _bn_ctx) == 0) {
    loggers::get_instance().warning("security_ecc::security_ecc (2): Failed to generate x_coordinate compressed key");
    _pub_key_compressed = OCTETSTRING(0, nullptr);
  } else { // Remove first byte
    loggers::get_instance().log_msg("security_ecc::security_ecc (2): Complete _pub_key_compressed=", _pub_key_compressed);
    _pub_key_compressed = OCTETSTRING(_pub_key_compressed.lengthof() - 1, 1 + static_cast<const unsigned char *>(_pub_key_compressed));
  }
  ::EC_POINT_free(ec_point);
  loggers::get_instance().log_msg("security_ecc::security_ecc (2): _pub_key_x=", _pub_key_x);
  loggers::get_instance().log_msg("security_ecc::security_ecc (2): _pub_key_y=", _pub_key_y);
  loggers::get_instance().log_msg("security_ecc::security_ecc (2): _pub_key_compressed=", _pub_key_compressed);
  loggers::get_instance().log("security_ecc::security_ecc (2): _pub_key_compressed_mode=%d", _pub_key_compressed_mode);
} // End of constructor

security_ecc::security_ecc(const ec_elliptic_curves p_elliptic_curve, const OCTETSTRING &p_public_key_compressed, const ecc_compressed_mode p_compressed_mode)
  : _elliptic_curve(p_elliptic_curve), _encryption_algotithm(encryption_algotithm::aes_128_ccm), _ec_key(nullptr), _ec_group(nullptr), _bn_ctx(nullptr),
    _pri_key(), _pub_key_x(), _pub_key_y(), _pub_key_compressed(p_public_key_compressed), _pub_key_compressed_mode{p_compressed_mode}, _secret_key(),
    _enc_key_x(), _enc_key_y(), _sym_key(), _nonce(), _tag() {
  loggers::get_instance().log(">>> security_ecc::security_ecc (3): %d", static_cast<int>(p_elliptic_curve));

  // Sanity checks
  if ((_elliptic_curve == ec_elliptic_curves::nist_p_256) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_256_r1)) {
    if (p_public_key_compressed.lengthof() != 32) {
      loggers::get_instance().error("security_ecc::security_ecc (3): Invalid public keys size");
    }
  } else if ((_elliptic_curve == ec_elliptic_curves::nist_p_384) || (_elliptic_curve == ec_elliptic_curves::brainpool_p_384_r1)) {
    if (p_public_key_compressed.lengthof() != 48) {
      loggers::get_instance().error("security_ecc::security_ecc (3): Invalid public keys size");
    }
  }

  int result = init();
  if (result == -1) {
    loggers::get_instance().error("security_ecc::security_ecc (3): Unsupported elliptic_curve %d", _elliptic_curve);
  }
  ::EC_KEY_set_conv_form(_ec_key, POINT_CONVERSION_UNCOMPRESSED);

  // Set public key
  BIGNUM *compressed_key = ::BN_new();
  ::BN_bin2bn(static_cast<const unsigned char *>(_pub_key_compressed), _pub_key_compressed.lengthof(), compressed_key);
  EC_POINT *ec_point = ::EC_POINT_new(_ec_group);
  result             = 0;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use primary
                                         // No break;
    case ec_elliptic_curves::nist_p_384:
                                         // No break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      // No break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      result = ::EC_POINT_set_compressed_coordinates_GFp(
        _ec_group, ec_point, compressed_key, (p_compressed_mode == ecc_compressed_mode::compressed_y_1) ? 1 : 0, _bn_ctx); // Use primary elliptic curve
      break;
    default:      // Use Binary
      result = 0; // FIXME Does not compile on Windows ::EC_POINT_set_compressed_coordinates_GF2m(_ec_group, ec_point, compressed_key, (p_compressed_mode ==
                  // ecc_compressed_mode::compressed_y_1) ? 1 : 0, _bn_ctx);
  }               // End of 'switch' statement
  BN_clear_free(compressed_key);
  compressed_key = nullptr;

  if (result == 0) {
    loggers::get_instance().error("security_ecc::security_ecc (3): Failed to get coordinates");
  } else if (::EC_POINT_is_on_curve(_ec_group, ec_point, _bn_ctx) == 0) {
    loggers::get_instance().error("security_ecc::security_ecc (3): Point Y0 is not on the curve");
  }

  // Set public keys
  BIGNUM *xy = ::BN_new();
  ::EC_POINT_point2bn(_ec_group, ec_point, POINT_CONVERSION_UNCOMPRESSED, xy, _bn_ctx);
  if (BN_num_bytes(xy) == 0) {
    ::BN_clear_free(xy);
    loggers::get_instance().error("security_ecc::security_ecc (3): Failed to generate xy coordinates, check algorithms");
  }
  loggers::get_instance().log("security_ecc::security_ecc (3): xy length: %d", BN_num_bytes(xy));
  ::EC_KEY_set_public_key(_ec_key, ec_point);
  // Generate X, Y coordinates
  OCTETSTRING v = int2oct(0, BN_num_bytes(xy));
  ::BN_bn2bin(xy, (unsigned char *)static_cast<const unsigned char *>(v));
  ::BN_clear_free(xy);
  xy = nullptr;
  if ((v.lengthof() % 2) != 0) { // TODO Check alse xy[0] == 0x04
    // Remove first byte
    loggers::get_instance().log_msg("security_ecc::security_ecc (3): Complete xy=", v);
    v = OCTETSTRING(v.lengthof() - 1, 1 + static_cast<const unsigned char *>(v));
  }
  loggers::get_instance().log_msg("security_ecc::security_ecc (3): xy=", v);
  const int l = v.lengthof() / 2;
  _pub_key_x  = OCTETSTRING(l, static_cast<const unsigned char *>(v));
  _pub_key_y  = OCTETSTRING(l, l + static_cast<const unsigned char *>(v));
  ::EC_POINT_free(ec_point);

  loggers::get_instance().log_msg("security_ecc::security_ecc (3): _pub_key_x=", _pub_key_x);
  loggers::get_instance().log_msg("security_ecc::security_ecc (3): _pub_key_y=", _pub_key_y);
  loggers::get_instance().log_msg("security_ecc::security_ecc (3): _pub_key_compressed=", _pub_key_compressed);
  loggers::get_instance().log("security_ecc::security_ecc (3): _pub_key_compressed_mode=%d", _pub_key_compressed_mode);
} // End of constructor

security_ecc::~security_ecc() {
  loggers::get_instance().log(">>> security_ecc::~security_ecc");

  if (_ec_key != nullptr) {
    ::EC_KEY_free(_ec_key);
  }
  if (_bn_ctx != nullptr) {
    ::BN_CTX_free(_bn_ctx);
  }

  loggers::get_instance().log("<<< security_ecc::~security_ecc");
} // End of Destructor

int security_ecc::generate() {
  loggers::get_instance().log(">>> security_ecc::generate");

  // Sanity check
  if (!::EC_KEY_generate_key(_ec_key)) { // Generate the private and public keys
    loggers::get_instance().error("security_ecc::generate: Failed to generate private/public keys");
    return -1;
  }

  BIGNUM *        x        = ::BN_new();
  BIGNUM *        y        = ::BN_new();
  const EC_POINT *ec_point = EC_KEY_get0_public_key(_ec_key);
  int             result   = 0;
  int             size     = 0;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use primary
                                         // No break;
    case ec_elliptic_curves::nist_p_384:
                                         // No break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      size   = 32;
      result = ::EC_POINT_get_affine_coordinates_GFp(_ec_group, ec_point, x, y, _bn_ctx); // Use primer on elliptic curve
      break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      size   = 48;
      result = ::EC_POINT_get_affine_coordinates_GFp(_ec_group, ec_point, x, y, _bn_ctx); // Use primer on elliptic curve
      break;
    default:      // Use binary
      result = 0; // FIXME Does not compile on Windows ::EC_POINT_get_affine_coordinates_GF2m(_ec_group, ec_point, x, y, _bn_ctx);
  }               // End of 'switch' statement
  if (result == 0) {
    loggers::get_instance().error("security_ecc::generate: Failed to get coordinates");
  }
  const BIGNUM *p = ::EC_KEY_get0_private_key(_ec_key);

  _pri_key = int2oct(0, size);
  ::BN_bn2bin(p, (unsigned char *)static_cast<const unsigned char *>(_pri_key));
  _pub_key_x = int2oct(0, size);
  ::BN_bn2bin(x, (unsigned char *)static_cast<const unsigned char *>(_pub_key_x));
  _pub_key_y = int2oct(0, size);
  ::BN_bn2bin(y, (unsigned char *)static_cast<const unsigned char *>(_pub_key_y));
  ::BN_clear_free(x);
  x = nullptr;
  ::BN_clear_free(y);
  y = nullptr;

  // Compressed
  int len = ::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, nullptr, 0, _bn_ctx);
  if (len == 0) {
    loggers::get_instance().warning("security_ecc::generate: Failed to generate x_coordinate compressed key");
  }
  _pub_key_compressed = int2oct(0, len);
  if (::EC_POINT_point2oct(_ec_group, ec_point, POINT_CONVERSION_COMPRESSED, (unsigned char *)static_cast<const unsigned char *>(_pub_key_compressed), len,
                           _bn_ctx) == 0) {
    loggers::get_instance().warning("security_ecc::generate: Failed to generate x_coordinate compressed key");
    _pub_key_compressed = OCTETSTRING(0, nullptr);
  } else { // Remove first byte
    loggers::get_instance().log_msg("security_ecc::generate: Complete _pub_key_compressed=", _pub_key_compressed);
    _pub_key_compressed_mode =
      ((_pub_key_compressed[0].get_octet() & 0x01) == 0x00) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1;
    _pub_key_compressed = OCTETSTRING(_pub_key_compressed.lengthof() - 1, 1 + static_cast<const unsigned char *>(_pub_key_compressed));
  }
  loggers::get_instance().log_msg("security_ecc::generate: _pri_key=", _pri_key);
  loggers::get_instance().log_msg("security_ecc::generate: _pub_key_x=", _pub_key_x);
  loggers::get_instance().log_msg("security_ecc::generate: _pub_key_y=", _pub_key_y);
  loggers::get_instance().log_msg("security_ecc::generate: _pub_key_compressed=", _pub_key_compressed);
  loggers::get_instance().log("security_ecc::generate: _pub_key_compressed_mode=%d", _pub_key_compressed_mode);

  return 0;
}

int security_ecc::generate_and_derive_ephemeral_key(const encryption_algotithm p_enc_algorithm, const OCTETSTRING &p_recipients_public_key_x,
                                                    const OCTETSTRING &p_recipients_public_key_y, const OCTETSTRING &p_salt) {
  loggers::get_instance().log(">>> security_ecc::generate_and_derive_ephemeral_key (1): %d", p_enc_algorithm);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (1): p_recipients_public_key_x=", p_recipients_public_key_x);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (1): p_recipients_public_key_y=", p_recipients_public_key_y);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (1): p_salt=", p_salt);

  // Sanity checks
  if (_pri_key.lengthof() == 0) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (1): Key shall be generated");
    return -1;
  }
  if ((_pub_key_x.lengthof() == 0) || (_pub_key_y.lengthof() == 0)) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (1): Keys shall be generated");
    return -1;
  }
  _encryption_algotithm = p_enc_algorithm;

  // Set buffers size
  int len = (EC_GROUP_get_degree(_ec_group) + 7) / 8;
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (1): _secret_key len:%d", len);
  _secret_key = int2oct(0, len);
  unsigned int nonce_length;
  unsigned int sym_key_length;
  unsigned int tag_length;
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      // No break;
    case encryption_algotithm::aes_128_gcm:
      nonce_length   = 12;
      sym_key_length = 16;
      tag_length     = 16;
      break;
    default:
      loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (1): Unsupported encryption algorithm");
      return -1;
  } // End of 'switch' statement
  unsigned int k_enc;
  unsigned int k_mac;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use the ANSI X9.62 Prime 256v1 curve
                                         // No break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      k_enc = 16;
      k_mac = 32;
      break;
    case ec_elliptic_curves::nist_p_384:
                                         // No break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      k_enc = 24; // TODO To be checked
      k_mac = 48;
      break;
    default:
      loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (1): Unsupported encryption algorithm");
      return -1;
  } // End of 'switch' statement

  // Convert the ephemeral public encryption keys to an EC point
  EC_POINT *ec_point = nullptr;
  bin_to_ec_point(p_recipients_public_key_x, p_recipients_public_key_y, &ec_point);
  // Generate the shared secret key (Key Agreement)
  int result = ::ECDH_compute_key((unsigned char *)static_cast<const unsigned char *>(_secret_key), _secret_key.lengthof(),
                                  ec_point, // From recipient's public key
                                  _ec_key,  // From ephemeral's private key
                                  nullptr);
  if (result != _secret_key.lengthof()) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (1): Failed to generate shared secret key");
    ::EC_POINT_free(ec_point);
    return -1;
  }
  ::EC_POINT_free(ec_point);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): _secret_key: ", _secret_key);

  // Derive the shared secret key
  OCTETSTRING digest;
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (1): k_enc size:%d - k_mac size: %d", k_enc, k_mac);
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (1): salt length:%d", p_salt.lengthof());
  if (kdf2(_secret_key, p_salt, 0x00 /*sha256*/, k_enc + k_mac, digest) != 0) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key: Failed to derive shared secret key");
    return -1;
  }
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): digest: ", digest);

  // Extract K1 and generate encrypted symmetric key
  OCTETSTRING k1(k_enc, static_cast<const unsigned char *>(digest));
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): k1: ", k1);
  if (!_sym_key.is_bound()) { // Generate random AES 128 key
    BIGNUM *r = ::BN_new();
    ::BN_pseudo_rand(r, k_enc * 8, -1, 0);
    _sym_key = int2oct(0, k_enc);
    ::BN_bn2bin(r, (unsigned char *)static_cast<const unsigned char *>(_sym_key));
    ::BN_free(r);
  }
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): _sym_key: ", _sym_key);
  _enc_sym_key = k1 ^ _sym_key;
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): Encrypted symmetric key: ", encrypted_symmetric_key());

  // Extract K2 and generate Tag vector
  OCTETSTRING k2(k_enc * 2, k_enc + static_cast<const unsigned char *>(digest));
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): k2: ", k2);
  hmac hmac_256;
  _tag = int2oct(0, tag_length);
  hmac_256.generate(static_cast<const unsigned char *>(_enc_sym_key), _enc_sym_key.lengthof(), static_cast<const unsigned char *>(k2), k2.lengthof(), _tag);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): _tag: ", _tag);

  // Generate random IV (nonce)
  BIGNUM *r = ::BN_new();
  ::BN_pseudo_rand(r, nonce_length * 8, -1, 0);
  _nonce = int2oct(0, nonce_length);
  ::BN_bn2bin(r, (unsigned char *)static_cast<const unsigned char *>(_nonce));
  ::BN_free(r);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (1): _nonce: ", _nonce);

  return 0;
}

int security_ecc::generate_and_derive_ephemeral_key(const encryption_algotithm p_enc_algorithm, const OCTETSTRING &p_ephemeral_public_key_x,
                                                    const OCTETSTRING &p_ephemeral_public_key_y, const OCTETSTRING &p_enc_sym_key,
                                                    const OCTETSTRING &p_expected_nonce, const OCTETSTRING &p_authentication_vector,
                                                    const OCTETSTRING &p_salt) {
  loggers::get_instance().log(">>> security_ecc::generate_and_derive_ephemeral_key (2): %d", p_enc_algorithm);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_ephemeral_public_key_x", p_ephemeral_public_key_x);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_ephemeral_public_key_y", p_ephemeral_public_key_y);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_enc_sym_key", p_enc_sym_key);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_expected_nonce", p_expected_nonce);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_authentication_vector", p_authentication_vector);
  loggers::get_instance().log_msg(">>> security_ecc::generate_and_derive_ephemeral_key (2): p_salt", p_salt);

  // Sanity checks
  if ((_pub_key_x.lengthof() == 0) || (_pub_key_y.lengthof() == 0)) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (2): Key shall be generated");
    return -1;
  }
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): _pub_key_x", _pub_key_x);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): _pub_key_y", _pub_key_y);

  _encryption_algotithm = p_enc_algorithm;
  _nonce                = p_expected_nonce;
  _enc_sym_key          = p_enc_sym_key;
  _tag                  = p_authentication_vector;

  // Set buffers size
  int len = (EC_GROUP_get_degree(_ec_group) + 7) / 8;
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (2): _secret_key len:%d", len);
  _secret_key = int2oct(0, len);
  unsigned int nonce_length;
  unsigned int sym_key_length;
  unsigned int tag_length;
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      // No break;
    case encryption_algotithm::aes_128_gcm:
      nonce_length   = 12;
      sym_key_length = 16;
      tag_length     = 16;
      break;
    default:
      loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (2): Unsupported encryption algorithm");
      return -1;
  } // End of 'switch' statement
  unsigned int k_enc;
  unsigned int k_mac;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use the ANSI X9.62 Prime 256v1 curve
                                         // No break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      k_enc = 16;
      k_mac = 32;
      break;
    case ec_elliptic_curves::nist_p_384:
                                         // No break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      break;
    default:
      loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (2): Unsupported encryption algorithm");
      return -1;
  } // End of 'switch' statement

  // Convert the peer public encryption key to an EC point
  security_ecc ec(ec_elliptic_curves::nist_p_256, p_ephemeral_public_key_x, p_ephemeral_public_key_y);
  EC_POINT *   ec_point = nullptr;
  bin_to_ec_point(ec.public_key_x(), ec.public_key_y(), &ec_point); // EC_POINT from recipient's private key
  // Generate the shared symmetric key
  int result = ::ECDH_compute_key((unsigned char *)static_cast<const unsigned char *>(_secret_key), _secret_key.lengthof(),
                                  ec_point, // From recipient's private key
                                  _ec_key,  // From sender's public key
                                  nullptr);
  if (result != _secret_key.lengthof()) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key (2): Failed to compute shared secret key");
    ::EC_POINT_free(ec_point);
    return -1;
  }
  ::EC_POINT_free(ec_point);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): _secret_key: ", _secret_key);

  // Derive the shared secret key
  OCTETSTRING digest;
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (2): k_enc size:%d - k_mac size: %d", k_enc, k_mac);
  loggers::get_instance().log("security_ecc::generate_and_derive_ephemeral_key (2): salt length:%d", p_salt.lengthof());
  if (kdf2(_secret_key, p_salt, 0x00, k_enc + k_mac, digest) != 0) {
    loggers::get_instance().warning("security_ecc::generate_and_derive_ephemeral_key: Failed to derive shared secret key");
    return -1;
  }
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): digest: ", digest);

  // Extract K2 and generate Tag vector
  OCTETSTRING k2(k_enc * 2, k_enc + static_cast<const unsigned char *>(digest));
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): k2: ", k2);
  hmac hmac_256;
  _tag = int2oct(0, tag_length);
  hmac_256.generate(static_cast<const unsigned char *>(_enc_sym_key), _enc_sym_key.lengthof(), static_cast<const unsigned char *>(k2), k2.lengthof(), _tag);
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): _tag: ", _tag);

  // Extract K1 and generate encrypted symmetric key
  OCTETSTRING k1(k_enc, static_cast<const unsigned char *>(digest));
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): k1: ", k1);
  _sym_key = p_enc_sym_key ^ k1;
  loggers::get_instance().log_msg("security_ecc::generate_and_derive_ephemeral_key (2): Decrypted symmetric key: ", symmetric_encryption_key());

  return 0;
}

int security_ecc::encrypt(const encryption_algotithm p_enc_algorithm, const OCTETSTRING &p_message, OCTETSTRING &p_enc_message) {
  loggers::get_instance().log(">>> security_ecc::encrypt: %d", p_enc_algorithm);
  loggers::get_instance().log_msg(">>> security_ecc::encrypt: p_message=", p_message);
  loggers::get_instance().log_msg(">>> security_ecc::encrypt: p_enc_message=", p_enc_message);

  // Sanity checks
  if ((_pub_key_x.lengthof() != 0) || (_pub_key_y.lengthof() != 0)) {
    loggers::get_instance().warning("security_ecc::encrypt: Constructor format #1 shall be used");
    return -1;
  }
  _encryption_algotithm = p_enc_algorithm;

  // Initialize the context and encryption operation
  EVP_CIPHER_CTX *ctx = ::EVP_CIPHER_CTX_new();
  unsigned int    ctr_set_iv_len, ctr_set_tag, ctr_get_tag;
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_128_ccm(), nullptr, nullptr, nullptr);
      // Allocate buffers size
      _nonce         = int2oct(0, 12);
      _tag           = int2oct(0, 16);
      _sym_key       = int2oct(0, 16);
      p_enc_message  = int2oct(0, p_message.lengthof());
      ctr_set_iv_len = EVP_CTRL_CCM_SET_IVLEN;
      ctr_set_tag    = EVP_CTRL_CCM_SET_TAG;
      ctr_get_tag    = EVP_CTRL_CCM_GET_TAG;
      break;
    case encryption_algotithm::aes_256_ccm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_256_ccm(), nullptr, nullptr, nullptr);
      ctr_set_iv_len = EVP_CTRL_CCM_SET_IVLEN;
      ctr_set_tag    = EVP_CTRL_CCM_SET_TAG;
      ctr_get_tag    = EVP_CTRL_CCM_GET_TAG;
      break;
    case encryption_algotithm::aes_128_gcm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr);
      // Allocate buffers size
      _nonce         = int2oct(0, 12);
      _tag           = int2oct(0, 16);
      _sym_key       = int2oct(0, 16);
      p_enc_message  = int2oct(0, p_message.lengthof());
      ctr_set_iv_len = EVP_CTRL_GCM_SET_IVLEN;
      ctr_set_tag    = EVP_CTRL_GCM_SET_TAG;
      ctr_get_tag    = EVP_CTRL_GCM_GET_TAG;
      break;
    case encryption_algotithm::aes_256_gcm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
      ctr_set_iv_len = EVP_CTRL_GCM_SET_IVLEN;
      ctr_set_tag    = EVP_CTRL_GCM_SET_TAG;
      ctr_get_tag    = EVP_CTRL_GCM_GET_TAG;
      break;
  } // End of 'switch' statement
  // Generate _sym_key
  ::RAND_bytes((unsigned char *)static_cast<const unsigned char *>(_sym_key), _sym_key.lengthof());
  loggers::get_instance().log_msg("security_ecc::encrypt: _sym_key: ", _sym_key);
  // Generate _nonce
  ::RAND_bytes((unsigned char *)static_cast<const unsigned char *>(_nonce), _nonce.lengthof());
  loggers::get_instance().log_msg("security_ecc::encrypt: nonce: ", _nonce);
  // Set nonce length
  ::EVP_CIPHER_CTX_ctrl(ctx, ctr_set_iv_len, _nonce.lengthof(), nullptr);
  // Set tag length
  ::EVP_CIPHER_CTX_ctrl(ctx, ctr_set_tag, _tag.lengthof(), nullptr);
  // Prime the key and nonce
  ::EVP_EncryptInit_ex(ctx, nullptr, nullptr, static_cast<const unsigned char *>(_sym_key), static_cast<const unsigned char *>(_nonce));
  // No authentication data
  // Encrypt the data
  int len = 0;
  ::EVP_EncryptUpdate(ctx, (unsigned char *)static_cast<const unsigned char *>(p_enc_message), &len, static_cast<const unsigned char *>(p_message),
                      p_message.lengthof());
  // Finalize the encryption session
  ::EVP_EncryptFinal_ex(ctx, (unsigned char *)static_cast<const unsigned char *>(p_enc_message) + len, &len);
  // Get the authentication tag(const char*)static_cast<const unsigned char*>(
  ::EVP_CIPHER_CTX_ctrl(ctx, ctr_get_tag, _tag.lengthof(), (unsigned char *)static_cast<const unsigned char *>(_tag));
  loggers::get_instance().log_msg("security_ecc::encrypt: tag: ", _tag);

  ::EVP_CIPHER_CTX_free(ctx);

  return 0;
}

int security_ecc::encrypt(const encryption_algotithm p_enc_algorithm, const OCTETSTRING &p_symmetric_key, const OCTETSTRING &p_nonce,
                          const OCTETSTRING &p_message, OCTETSTRING &p_enc_message) {
  loggers::get_instance().log(">>> security_ecc::encrypt (2): %d", p_enc_algorithm);

  _encryption_algotithm = p_enc_algorithm;
  _sym_key              = p_symmetric_key;
  _nonce                = p_nonce;

  // Initialize the context and encryption operation
  EVP_CIPHER_CTX *ctx = ::EVP_CIPHER_CTX_new();
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_128_ccm(), nullptr, nullptr, nullptr);
      // Allocate buffers size
      _tag          = int2oct(0, 16);
      p_enc_message = int2oct(0, p_message.lengthof());
      break;
    case encryption_algotithm::aes_256_ccm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_256_ccm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_128_gcm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_256_gcm:
      ::EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
      break;
  } // End of 'switch' statement
  loggers::get_instance().log_msg("security_ecc::encrypt: _sym_key: ", _sym_key);
  loggers::get_instance().log_msg("security_ecc::encrypt: nonce: ", _nonce);
  // Set nonce length
  ::EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_IVLEN, _nonce.lengthof(), nullptr);
  // Set tag length
  ::EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, _tag.lengthof(), nullptr);
  // Prime the key and nonce
  ::EVP_EncryptInit_ex(ctx, nullptr, nullptr, static_cast<const unsigned char *>(_sym_key), static_cast<const unsigned char *>(_nonce));
  // No authentication data
  // Encrypt the data
  int len = 0;
  ::EVP_EncryptUpdate(ctx, (unsigned char *)static_cast<const unsigned char *>(p_enc_message), &len, static_cast<const unsigned char *>(p_message),
                      p_message.lengthof());
  // Finalize the encryption session
  ::EVP_EncryptFinal_ex(ctx, (unsigned char *)static_cast<const unsigned char *>(p_enc_message) + len, &len);
  // Get the authentication tag
  ::EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_GET_TAG, _tag.lengthof(), (unsigned char *)static_cast<const unsigned char *>(_tag));
  loggers::get_instance().log_msg("security_ecc::encrypt: tag: ", _tag);

  ::EVP_CIPHER_CTX_free(ctx);

  return 0;
}

int security_ecc::decrypt(const encryption_algotithm p_enc_algorithm, const OCTETSTRING &p_key, const OCTETSTRING &p_nonce, const OCTETSTRING &p_tag,
                          const OCTETSTRING &p_enc_message, OCTETSTRING &p_message) {
  loggers::get_instance().log(">>> security_ecc::decrypt: p_enc_algorithm=%d", p_enc_algorithm);
  loggers::get_instance().log_msg(">>> security_ecc::decrypt: p_key=", p_key);
  loggers::get_instance().log_msg(">>> security_ecc::decrypt: p_nonce", p_nonce);
  loggers::get_instance().log_msg(">>> security_ecc::decrypt: p_tag", p_tag);
  loggers::get_instance().log_msg(">>> security_ecc::decrypt: p_enc_message", p_enc_message);

  _encryption_algotithm = p_enc_algorithm;
  _sym_key              = p_key;
  _nonce                = p_nonce;
  _tag                  = p_tag;

  // Initialize the context and decryption operation
  EVP_CIPHER_CTX *ctx = ::EVP_CIPHER_CTX_new();
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_128_ccm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_256_ccm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_256_ccm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_128_gcm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_256_gcm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
      break;
  } // End of 'switch' statement
  // Set nonce length
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_IVLEN, _nonce.lengthof(), nullptr);
  // Set expected tag value
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, _tag.lengthof(), (unsigned char *)static_cast<const unsigned char *>(_tag));
  // Specify key and IV
  EVP_DecryptInit_ex(ctx, nullptr, nullptr, static_cast<const unsigned char *>(_sym_key), static_cast<const unsigned char *>(_nonce));
  // Decrypt plaintext, verify tag: can only be called once
  p_message  = int2oct(0, p_enc_message.lengthof());
  int len    = 0;
  int result = EVP_DecryptUpdate(ctx, (unsigned char *)static_cast<const unsigned char *>(p_message), &len, static_cast<const unsigned char *>(p_enc_message),
                                 p_enc_message.lengthof());
  loggers::get_instance().log("security_ecc::decrypt: len: %d", len);
  loggers::get_instance().log("security_ecc::decrypt: result: %d", result);
  ::EVP_CIPHER_CTX_free(ctx);

  return (result > 0) ? 0 : -1;
}

int security_ecc::decrypt(const OCTETSTRING &p_tag, const OCTETSTRING &p_enc_message, OCTETSTRING &p_message) {
  loggers::get_instance().log(">>> security_ecc::decrypt: %d", _encryption_algotithm);

  // Sanity checks
  if ((_pri_key.lengthof() == 0) || (_secret_key.lengthof() == 0)) {
    loggers::get_instance().warning("security_ecc::decrypt: Constrictor format #2 shall be used");
    return -1;
  }
  _tag = p_tag;

  // Initialize the context and decryption operation
  EVP_CIPHER_CTX *ctx = ::EVP_CIPHER_CTX_new();
  switch (_encryption_algotithm) {
    case encryption_algotithm::aes_128_ccm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_128_ccm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_256_ccm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_256_ccm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_128_gcm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr);
      break;
    case encryption_algotithm::aes_256_gcm:
      ::EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
      break;
  } // End of 'switch' statement
  // Set nonce length
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_IVLEN, _nonce.lengthof(), nullptr);
  // Set expected tag value
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, _tag.lengthof(), (unsigned char *)static_cast<const unsigned char *>(_tag));
  // Specify key and IV
  EVP_DecryptInit_ex(ctx, nullptr, nullptr, static_cast<const unsigned char *>(_sym_key), static_cast<const unsigned char *>(_nonce));
  // Decrypt plaintext, verify tag: can only be called once
  p_message  = int2oct(0, p_enc_message.lengthof());
  int len    = 0;
  int result = EVP_DecryptUpdate(ctx, (unsigned char *)static_cast<const unsigned char *>(p_message), &len, static_cast<const unsigned char *>(p_enc_message),
                                 p_enc_message.lengthof());
  loggers::get_instance().log("security_ecc::decrypt: len: %d", len);
  loggers::get_instance().log("security_ecc::decrypt: result: %d", result);
  ::EVP_CIPHER_CTX_free(ctx);

  return (result > 0) ? 0 : -1;
}

int security_ecc::sign(const OCTETSTRING &p_data, OCTETSTRING &p_r_sig, OCTETSTRING &p_s_sig) {
  loggers::get_instance().log(">>> security_ecc::sign");
  loggers::get_instance().log_msg(">>> security_ecc::sign: p_data: ", p_data);

  // Sanity checks
  if (_pri_key.lengthof() == 0) { // No private key
    return -1;
  }
  if (p_data.lengthof() == 0) {
    return -1;
  }

  ECDSA_SIG *signature = ::ECDSA_do_sign(static_cast<const unsigned char *>(p_data), p_data.lengthof(), _ec_key);
  if (signature == nullptr) {
    loggers::get_instance().warning("security_ecc::sign: Signature failed");
    return -1;
  }
  loggers::get_instance().log("security_ecc::sign: succeed");

  if (::ECDSA_do_verify(static_cast<const unsigned char *>(p_data), p_data.lengthof(), signature, _ec_key) != 1) {
    loggers::get_instance().warning("security_ecc::sign: Signature not verified");
    return -1;
  }

  const BIGNUM *r = nullptr;
  const BIGNUM *s = nullptr;
  ::ECDSA_SIG_get0(signature, &r, &s);
  loggers::get_instance().log("security_ecc::sign: r size: %d", BN_num_bytes(r));
  p_r_sig = int2oct(0, BN_num_bytes(r));
  ::BN_bn2bin(r, (unsigned char *)static_cast<const unsigned char *>(p_r_sig));
  loggers::get_instance().log_msg("security_ecc::sign: r=", p_r_sig);
  loggers::get_instance().log("security_ecc::sign: s size: %d", BN_num_bytes(s));
  p_s_sig = int2oct(0, BN_num_bytes(s));
  ::BN_bn2bin(s, (unsigned char *)static_cast<const unsigned char *>(p_s_sig));
  loggers::get_instance().log_msg("security_ecc::sign: s=", p_s_sig);

  ::ECDSA_SIG_free(signature);

  return 0;
}

int security_ecc::sign_verif(const OCTETSTRING &p_data, const OCTETSTRING &p_signature) {
  loggers::get_instance().log(">>> security_ecc::sign_verif");
  loggers::get_instance().log_msg(">>> security_ecc::sign_verify: p_data: ", p_data);

  // Sanity checks
  if (p_data.lengthof() == 0) {
    return -1;
  }

  // Build the signature
  BIGNUM *r = ::BN_bin2bn(static_cast<const unsigned char *>(p_signature), p_signature.lengthof() / 2, nullptr);
  loggers::get_instance().log_to_hexa("security_ecc::sign_verify: r=", static_cast<const unsigned char *>(p_signature), p_signature.lengthof() / 2);
  BIGNUM *s = ::BN_bin2bn(static_cast<const unsigned char *>(p_signature) + p_signature.lengthof() / 2, p_signature.lengthof() / 2, nullptr);
  loggers::get_instance().log_to_hexa("security_ecc::sign_verify: s=", static_cast<const unsigned char *>(p_signature) + p_signature.lengthof() / 2,
                                      p_signature.lengthof() / 2);
  ECDSA_SIG *signature = ECDSA_SIG_new();
  ::ECDSA_SIG_set0(signature, r, s);
  // Check the signature
  int result = ::ECDSA_do_verify(static_cast<const unsigned char *>(p_data), p_data.lengthof(), signature, _ec_key);
  ::ECDSA_SIG_free(signature);
  loggers::get_instance().log("security_ecc::sign_verif: %s", (result == 1) ? "succeed" : "failed");
  return (result == 1) ? 0 : -1;
}
const int security_ecc::init() {
  loggers::get_instance().log(">>> security_ecc::init: %d", static_cast<int>(_elliptic_curve));

  ::OpenSSL_add_all_algorithms();
  ::ERR_load_crypto_strings();
  ::ERR_clear_error();

  int result = -1;
  switch (_elliptic_curve) {
    case ec_elliptic_curves::nist_p_256: // Use the ANSI X9.62 Prime 256v1 curve
      result = ::OBJ_txt2nid("prime256v1");
      break;
    case ec_elliptic_curves::nist_p_384:
      result = ::OBJ_txt2nid("secp384r1");
      break;
    case ec_elliptic_curves::brainpool_p_256_r1:
      result = ::OBJ_txt2nid("brainpoolP256r1");
      break;
    case ec_elliptic_curves::brainpool_p_384_r1:
      result = ::OBJ_txt2nid("brainpoolP384r1");
      break;
    default:
      loggers::get_instance().error("security_ecc::init: Unsupported EC elliptic_curve");
  } // End of 'switch' statement
  if (result < 0) {
    loggers::get_instance().warning("security_ecc::init: Unaible to set EC elliptic_curve");
    return -1;
  }
  loggers::get_instance().log("security_ecc::init: Nid=%d", result);

  _ec_key = ::EC_KEY_new_by_curve_name(result);            // Set the elliptic curve
  if (_ec_key == nullptr) {
    loggers::get_instance().error("security_ecc::init: Not supported curve");
  }
  ::EC_KEY_set_asn1_flag(_ec_key, OPENSSL_EC_NAMED_CURVE); // Used to save and retrieve keys
  _ec_group = ::EC_KEY_get0_group(_ec_key);                // Get pointer to the EC_GROUP
  _bn_ctx   = ::BN_CTX_new();

  loggers::get_instance().log("<<< security_ecc::init: 0");
  return 0;
} // End of init

int security_ecc::bin_to_ec_point(const OCTETSTRING &p_public_key_x, const OCTETSTRING &p_public_key_y,
                                  EC_POINT **p_ec_point) { // ec_key_public_key_bin_to_point
  BIGNUM *pubk_bn;

  OCTETSTRING v = int2oct(4, 1);
  v += p_public_key_x;
  v += p_public_key_y;

  pubk_bn     = ::BN_bin2bn(static_cast<const unsigned char *>(v), v.lengthof(), nullptr);
  *p_ec_point = ::EC_POINT_new(_ec_group);
  ::EC_POINT_bn2point(_ec_group, pubk_bn, *p_ec_point, _bn_ctx);
  ::BN_clear_free(pubk_bn);

  // BIO            *bio_out  = nullptr; /* stdout */
  // bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);
  // BIGNUM *x = BN_new();
  // BIGNUM *y = BN_new();

  // if (EC_POINT_get_affine_coordinates_GFp(_ec_group, *p_ec_point, x, y, nullptr)) {
  //   BN_print_fp(stdout, x);
  //   putc('\n', stdout);
  //   BN_print_fp(stdout, y);
  //   putc('\n', stdout);
  // }
  // BN_free(x); BN_free(y);

  return 0;
}

int security_ecc::public_key_to_bin(OCTETSTRING &p_bin_key) { // ec_key_public_key_to_bin
  const EC_GROUP *ec_group = EC_KEY_get0_group(_ec_key);
  const EC_POINT *pub      = EC_KEY_get0_public_key(_ec_key);
  BIGNUM *pub_bn   = BN_new();

  ::EC_POINT_point2bn(ec_group, pub, POINT_CONVERSION_UNCOMPRESSED, pub_bn, _bn_ctx);
  p_bin_key = int2oct(0, BN_num_bytes(pub_bn));
  ::BN_bn2bin(pub_bn, (unsigned char *)static_cast<const unsigned char *>(p_bin_key));

  ::BN_clear_free(pub_bn);

  return 0;
}

int security_ecc::kdf2(const OCTETSTRING &p_secret_key, const OCTETSTRING &p_salt, const unsigned char p_hash_algorithm, const int p_key_length,
                       OCTETSTRING &p_digest) {
  loggers::get_instance().log(">>> security_ecc::kdf2");

  // Sanity checks

  int result = -1;
  switch (p_hash_algorithm) {
    case 0x00: // SHA 256
      result = kdf2_sha256(p_secret_key, p_salt, p_key_length, p_digest);
      break;
  } // End of 'switch' statement

  return result;
}

int security_ecc::kdf2_sha256(const OCTETSTRING &p_secret_key, const OCTETSTRING &p_salt, const int p_key_length, OCTETSTRING &p_digest) {
  loggers::get_instance().log_msg(">>> security_ecc::kdf2_sha256: p_secret_key: ", p_secret_key);
  loggers::get_instance().log_msg(">>> security_ecc::kdf2_sha256: p_salt: ", p_salt);
  loggers::get_instance().log(">>> security_ecc::kdf2_sha256: p_key_length:%d", p_key_length);

  // Sanity checks

  int sha256_blk_len = 32;
  int num_blk_out    = (int)ceil(p_key_length / (float)sha256_blk_len);
  loggers::get_instance().log("security_ecc::kdf2_sha256: num_blk_out= %d", num_blk_out);
  p_digest = OCTETSTRING(0, nullptr);
  sha256 hash;
  for (int i = 1; i < num_blk_out + 1; i++) {
    OCTETSTRING hash_input = p_secret_key + int2oct(i, 4) + p_salt;
    loggers::get_instance().log_msg("security_ecc::kdf2_sha256: hash_input: ", hash_input);
    OCTETSTRING h;
    hash.generate(hash_input, h);
    loggers::get_instance().log_msg("security_ecc::kdf2_sha256: h: ", h);
    p_digest += h;
    loggers::get_instance().log_msg("security_ecc::kdf2_sha256: p_digest: ", p_digest);
    loggers::get_instance().log("security_ecc::kdf2_sha256: p_digest: %d", p_digest.lengthof());
  } // End of 'for' statement
  if (p_digest.lengthof() > p_key_length * 2) {
    p_digest = OCTETSTRING(p_key_length * 2, static_cast<const unsigned char *>(p_digest));
  }

  loggers::get_instance().log_msg("<<< security_ecc::kdf2_sha256: p_secret_key: ", p_digest);
  return 0;
}
