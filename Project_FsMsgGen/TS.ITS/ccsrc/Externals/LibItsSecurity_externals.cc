/*!
 * \file      LibItsSecurity_externals.cc
 * \brief     Source file for Security externl functions.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <memory>

#include "LibItsCommon_Functions.hh"
#include "LibItsSecurity_Functions.hh"

#include "hmac.hh"
#include "sha256.hh"
#include "sha384.hh"

#include "security_ecc.hh"

#include "security_services.hh"

#include "geospacial.hh"

#include <openssl/ec.h>
#include <openssl/ecdsa.h>

#include "loggers.hh"

namespace LibItsSecurity__Functions {

  static std::unique_ptr<geospacial> g(new geospacial);

  // FIXME Unify code with security_services

  /**
   * \fn OCTETSTRING fx_hashWithSha256(const OCTETSTRING& p__toBeHashedData);
   * \brief Produces a 256-bit (32-bytes) hash value
   * \param[in] p__toBeHashedData The data to be used to calculate the hash value
   * \return  The hash value
   */
  OCTETSTRING fx__hashWithSha256(const OCTETSTRING &p__toBeHashedData) {
    loggers::get_instance().log_msg(">>> fx__hashWithSha256: p__toBeHashedData= ", p__toBeHashedData);

    sha256      hash;
    OCTETSTRING hashData;
    hash.generate(p__toBeHashedData, hashData);
    loggers::get_instance().log_msg("fx__hashWithSha256: hashData= ", hashData);
    return hashData;
  } // End of function fx__hashWithSha256

  /**
   * \fn OCTETSTRING fx_hashWithSha384(const OCTETSTRING& p__toBeHashedData);
   * \brief Produces a 384-bit (48-bytes) hash value
   * \param[in] p__toBeHashedData Data to be used to calculate the hash value
   * \return The hash value
   */
  OCTETSTRING fx__hashWithSha384(const OCTETSTRING &p__toBeHashedData) {
    sha384      hash;
    OCTETSTRING hashData;
    hash.generate(p__toBeHashedData, hashData);
    loggers::get_instance().log_msg("fx__hashWithSha384: hashData= ", hashData);
    return hashData;
  } // End of function fx__hashWithSha384

  /**
   * \fn OCTETSTRING fx__signWithEcdsaNistp256WithSha256(const OCTETSTRING& p__toBeSignedSecuredMessage, const OCTETSTRING& p__privateKey);
   * \brief Produces a Elliptic Curve Digital Signature Algorithm (ECDSA) signature based on standard IEEE 1609.2
   * \param[in] p__toBeSignedSecuredMessage The data to be signed
   * \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate
   * \param[in] p__privateKey The private key
   * \return The signature value
   */
  OCTETSTRING fx__signWithEcdsaNistp256WithSha256(const OCTETSTRING &p__toBeSignedSecuredMessage, const OCTETSTRING &p__certificateIssuer,
                                                  const OCTETSTRING &p__privateKey) {
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaNistp256WithSha256: data=", p__toBeSignedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaNistp256WithSha256: issuer=", p__certificateIssuer);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaNistp256WithSha256: private key=", p__privateKey);

    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__privateKey.lengthof() != 32)) {
      loggers::get_instance().log("fx__signWithEcdsaNistp256WithSha256: Wrong parameters");
      return OCTETSTRING(0, nullptr);
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeSignedSecuredMessage, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__signWithEcdsaNistp256WithSha256: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__signWithEcdsaNistp256WithSha256: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    loggers::get_instance().log_msg("fx__signWithEcdsaNistp256WithSha256: Hash (Data input) || Hash (Signer identifier input)=", hashData1);
    OCTETSTRING hashData; // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__signWithEcdsaNistp256WithSha256: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Calculate the signature
    security_ecc k(ec_elliptic_curves::nist_p_256, p__privateKey);
    OCTETSTRING  r_sig;
    OCTETSTRING  s_sig;
    if (k.sign(hashData, r_sig, s_sig) == 0) {
      OCTETSTRING os = r_sig + s_sig;
      loggers::get_instance().log_msg("r_sig= ", r_sig);
      loggers::get_instance().log_msg("s_sig= ", s_sig);
      loggers::get_instance().log_msg("sig= ", os);
      return os;
    }

    return OCTETSTRING(0, nullptr);
  }

  /**
   * \fn OCTETSTRING fx__signWithEcdsaNistp256WithSha256(const OCTETSTRING& p__toBeSignedSecuredMessage, const OCTETSTRING& p__privateKey);
   * \brief Produces a Elliptic Curve Digital Signature Algorithm (ECDSA) signature based on raw data
   * \param[in] p__toBeSignedSecuredMessage The data to be signed
   * \param[in] p__privateKey The private key
   * \return The signature value
   */
  OCTETSTRING fx__test__signWithEcdsaNistp256WithSha256(const OCTETSTRING &p__toBeSignedSecuredMessage, const OCTETSTRING &p__privateKey) {
    loggers::get_instance().log_msg(">>> fx__test__signWithEcdsaNistp256WithSha256: data=", p__toBeSignedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__test__signWithEcdsaNistp256WithSha256: private key=", p__privateKey);

    // Sanity checks
    if (p__privateKey.lengthof() != 32) {
      loggers::get_instance().log("fx__test__signWithEcdsaNistp256WithSha256: Wrong parameters");
      return OCTETSTRING(0, nullptr);
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeSignedSecuredMessage, hashData1);
    loggers::get_instance().log_msg("fx__test__signWithEcdsaNistp256WithSha256: Hash (Data input)=", hashData1);
    // Calculate the signature
    security_ecc k(ec_elliptic_curves::nist_p_256, p__privateKey);
    OCTETSTRING  r_sig;
    OCTETSTRING  s_sig;
    if (k.sign(hashData1, r_sig, s_sig) == 0) {
      OCTETSTRING os = r_sig + s_sig;
      loggers::get_instance().log_msg("r_sig= ", r_sig);
      loggers::get_instance().log_msg("s_sig= ", s_sig);
      loggers::get_instance().log_msg("sig= ", os);
      return os;
    }

    return OCTETSTRING(0, nullptr);
  }

  /**
   * \fn OCTETSTRING fx__signWithEcdsaBrainpoolp256r1WithSha256(const OCTETSTRING& p__toBeSignedSecuredMessage, const OCTETSTRING& p__privateKey);
   * \brief Produces a Elliptic Curve Digital Signature Algorithm (ECDSA) signature based on standard IEEE 1609.2
   * \param[in] p__toBeSignedSecuredMessage The data to be signed
   * \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate
   * \param[in] p__privateKey The private key
   * \return The signature value
   */
  OCTETSTRING fx__signWithEcdsaBrainpoolp256r1WithSha256(const OCTETSTRING &p__toBeSignedSecuredMessage, const OCTETSTRING &p__certificateIssuer,
                                                         const OCTETSTRING &p__privateKey) {
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp256r1WithSha256: data=", p__toBeSignedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp256r1WithSha256: issuer=", p__certificateIssuer);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp256r1WithSha256: private key=", p__privateKey);

    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__privateKey.lengthof() != 32)) {
      loggers::get_instance().log("fx__signWithEcdsaBrainpoolp256r1WithSha256: Wrong parameters");
      return OCTETSTRING(0, nullptr);
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeSignedSecuredMessage, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp256r1WithSha256: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp256r1WithSha256: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp256r1WithSha256: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Calculate the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, p__privateKey);
    OCTETSTRING  r_sig;
    OCTETSTRING  s_sig;
    if (k.sign(hashData, r_sig, s_sig) == 0) {
      OCTETSTRING os = r_sig + s_sig;
      loggers::get_instance().log_msg("r_sig= ", r_sig);
      loggers::get_instance().log_msg("s_sig= ", s_sig);
      loggers::get_instance().log_msg("sig= ", os);
      return os;
    }

    return OCTETSTRING(0, nullptr);
  }

  /**
   * \fn OCTETSTRING fx__signWithEcdsaBrainpoolp384r1WithSha384(const OCTETSTRING& p__toBeSignedSecuredMessage, const OCTETSTRING& p__privateKey);
   * \brief Produces a Elliptic Curve Digital Signature Algorithm (ECDSA) signature based on standard IEEE 1609.2
   * \param[in] p__toBeSignedSecuredMessage The data to be signed
   * \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate
   * \param[in] p__privateKey The private key
   * \return The signature value
   */
  OCTETSTRING fx__signWithEcdsaBrainpoolp384r1WithSha384(const OCTETSTRING &p__toBeSignedSecuredMessage, const OCTETSTRING &p__certificateIssuer,
                                                         const OCTETSTRING &p__privateKey) {
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp384r1WithSha384: data=", p__toBeSignedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp384r1WithSha384: issuer=", p__certificateIssuer);
    loggers::get_instance().log_msg(">>> fx__signWithEcdsaBrainpoolp384r1WithSha384: private key=", p__privateKey);

    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 48) || (p__privateKey.lengthof() != 48)) {
      loggers::get_instance().log("fx__signWithEcdsaBrainpoolp384r1WithSha384: Wrong parameters");
      return OCTETSTRING(0, nullptr);
    }

    // Calculate the SHA384 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha384      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeSignedSecuredMessage, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 48)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha384_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Calculate the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, p__privateKey);
    OCTETSTRING  r_sig;
    OCTETSTRING  s_sig;
    if (k.sign(hashData, r_sig, s_sig) == 0) {
      OCTETSTRING os = r_sig + s_sig;
      loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: r_sig= ", r_sig);
      loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: s_sig= ", s_sig);
      loggers::get_instance().log_msg("fx__signWithEcdsaBrainpoolp384r1WithSha384: sig= ", os);
      return os;
    }

    return OCTETSTRING(0, nullptr);
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaNistp256WithSha256(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaNistp256PublicKeyCompressed); \brief Verify the signature of the specified data based on standard IEEE 1609.2 \param[in] p__toBeVerifiedData The
   * data to be verified \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate \param[in]
   * p__signature The signature \param[in] p__ecdsaNistp256PublicKeyCompressed The compressed public key (x coordinate only) \return true on success, false
   * otherwise
   */
  BOOLEAN fx__verifyWithEcdsaNistp256WithSha256(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaNistp256PublicKeyCompressed,
                                                const INTEGER &p__compressedMode) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__signature.lengthof() != 64) || (p__ecdsaNistp256PublicKeyCompressed.lengthof() != 32)) {
      loggers::get_instance().log("fx__verifyWithEcdsaNistp256WithSha256: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::nist_p_256, p__ecdsaNistp256PublicKeyCompressed,
                   (p__compressedMode == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaNistp256WithSha256(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__ecdsaNistp256PublicKeyCompressed);
   * \brief Verify the signature of the specified data based on raw data
   * \param[in] p__toBeVerifiedData The data to be verified
   * \param[in] p__signature The signature
   * \param[in] p__ecdsaNistp256PublicKeyCompressed The compressed public key (x coordinate only)
   * \return true on success, false otherwise
   */
  BOOLEAN fx__test__verifyWithEcdsaNistp256WithSha256(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__signature,
                                                      const OCTETSTRING &p__ecdsaNistp256PublicKeyCompressed, const INTEGER &p__compressedMode) {
    // Sanity checks
    if ((p__signature.lengthof() != 64) || (p__ecdsaNistp256PublicKeyCompressed.lengthof() != 32)) {
      loggers::get_instance().log("fx__test__verifyWithEcdsaNistp256WithSha256: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    loggers::get_instance().log_msg("fx__test__verifyWithEcdsaNistp256WithSha256: Hash (Data input)=", hashData1);
    // Check the signature
    security_ecc k(ec_elliptic_curves::nist_p_256, p__ecdsaNistp256PublicKeyCompressed,
                   (p__compressedMode == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (k.sign_verif(hashData1, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaNistp256WithSha256_1(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaNistp256PublicKeyX, const OCTETSTRING& p__ecdsaNistp256PublicKeyY); \brief Verify the signature of the specified data based on standard IEEE 1609.2
   * \param[in] p__toBeVerifiedData The data to be verified
   * \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate
   * \param[in] p__signature The signature
   * \param[in] p__ecdsaNistp256PublicKeyX The public key (x coordinate)
   * \param[in] p__ecdsaNistp256PublicKeyY The public key (y coordinate)
   * \return true on success, false otherwise
   */
  BOOLEAN fx__verifyWithEcdsaNistp256WithSha256__1(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                   const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaNistp256PublicKeyX,
                                                   const OCTETSTRING &p__ecdsaNistp256PublicKeyY) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__signature.lengthof() != 64) || (p__ecdsaNistp256PublicKeyX.lengthof() != 32) ||
        (p__ecdsaNistp256PublicKeyY.lengthof() != 32)) {
      loggers::get_instance().log("fx__verifyWithEcdsaNistp256WithSha256__1: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256__1: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256__1: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaNistp256WithSha256__1: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::nist_p_256, p__ecdsaNistp256PublicKeyX, p__ecdsaNistp256PublicKeyY);
    // security_ecc k(ec_elliptic_curves::nist_p_256);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaBrainpoolp256r1WithSha256(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaBrainpoolp256PublicKeyCompressed); \brief Verify the signature of the specified data based on standard IEEE 1609.2 \param[in] p__toBeVerifiedData
   * The data to be verified \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate \param[in]
   * p__signature The signature \param[in] p__ecdsaBrainpoolp256PublicKeyCompressed The compressed public key (x coordinate only) \return true on success, false
   * otherwise
   */
  BOOLEAN fx__verifyWithEcdsaBrainpoolp256r1WithSha256(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                       const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaBrainpoolp256PublicKeyCompressed,
                                                       const INTEGER &p__compressedMode) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__signature.lengthof() != 64) || (p__ecdsaBrainpoolp256PublicKeyCompressed.lengthof() != 32)) {
      loggers::get_instance().log("fx__verifyWithEcdsaBrainpoolp256r1WithSha256: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, p__ecdsaBrainpoolp256PublicKeyCompressed,
                   (p__compressedMode == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaBrainpoolp256r1WithSha256_1(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaBrainpoolp256PublicKeyX, const OCTETSTRING& p__ecdsaBrainpoolp256PublicKeyY); \brief Verify the signature of the specified data based on standard
   * IEEE 1609.2 \param[in] p__toBeVerifiedData The data to be verified \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in
   * case of self signed certificate \param[in] p__signature The signature \param[in] p__ecdsaBrainpoolp256PublicKeyX The public key (x coordinate) \param[in]
   * p__ecdsaBrainpoolp256PublicKeyY The public key (y coordinate) \return true on success, false otherwise
   */
  BOOLEAN fx__verifyWithEcdsaBrainpoolp256r1WithSha256__1(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                          const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaBrainpoolp256PublicKeyX,
                                                          const OCTETSTRING &p__ecdsaBrainpoolp256PublicKeyY) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 32) || (p__signature.lengthof() != 64) || (p__ecdsaBrainpoolp256PublicKeyX.lengthof() != 32) ||
        (p__ecdsaBrainpoolp256PublicKeyY.lengthof() != 32)) {
      loggers::get_instance().log("fx__verifyWithEcdsaBrainpoolp256r1WithSha256__1: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA256 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha256      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha256_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256__1: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256__1: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp256r1WithSha256__1: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, p__ecdsaBrainpoolp256PublicKeyX, p__ecdsaBrainpoolp256PublicKeyY);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaBrainpoolp384r1WithSha384(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaBrainpoolp384PublicKeyCompressed); \brief Verify the signature of the specified data based on standard IEEE 1609.2 \param[in] p__toBeVerifiedData
   * The data to be verified \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in case of self signed certificate \param[in]
   * p__signature The signature \param[in] p__ecdsaBrainpoolp384PublicKeyCompressed The compressed public key (x coordinate only) \return true on success, false
   * otherwise
   */
  BOOLEAN fx__verifyWithEcdsaBrainpoolp384r1WithSha384(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                       const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaBrainpoolp384PublicKeyCompressed,
                                                       const INTEGER &p__compressedMode) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 48) || (p__signature.lengthof() != 96) || (p__ecdsaBrainpoolp384PublicKeyCompressed.lengthof() != 48)) {
      loggers::get_instance().log("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA384 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha384      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 48)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha384_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, p__ecdsaBrainpoolp384PublicKeyCompressed,
                   (p__compressedMode == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn BOOLEAN fx__verifyWithEcdsaBrainpoolp384r1WithSha384_1(const OCTETSTRING& p__toBeVerifiedData, const OCTETSTRING& p__signature, const OCTETSTRING&
   * p__ecdsaBrainpoolp384PublicKeyX, const OCTETSTRING& p__ecdsaBrainpoolp384PublicKeyY); \brief Verify the signature of the specified data based on standard
   * IEEE 1609.2 \param[in] p__toBeVerifiedData The data to be verified \param[in] p__certificateIssuer The whole-hash issuer certificate or int2oct(0, 32) in
   * case of self signed certificate \param[in] p__signature The signature \param[in] p__ecdsaBrainpoolp384PublicKeyX The public key (x coordinate) \param[in]
   * p__ecdsaBrainpoolp384PublicKeyY The public key (y coordinate) \return true on success, false otherwise
   */
  BOOLEAN fx__verifyWithEcdsaBrainpoolp384r1WithSha384__1(const OCTETSTRING &p__toBeVerifiedData, const OCTETSTRING &p__certificateIssuer,
                                                          const OCTETSTRING &p__signature, const OCTETSTRING &p__ecdsaBrainpoolp384PublicKeyX,
                                                          const OCTETSTRING &p__ecdsaBrainpoolp384PublicKeyY) {
    // Sanity checks
    if ((p__certificateIssuer.lengthof() != 48) || (p__signature.lengthof() != 96) || (p__ecdsaBrainpoolp384PublicKeyX.lengthof() != 48) ||
        (p__ecdsaBrainpoolp384PublicKeyY.lengthof() != 48)) {
      loggers::get_instance().log("fx__verifyWithEcdsaBrainpoolp384r1WithSha384__1: Wrong parameters");
      return FALSE;
    }

    // Calculate the SHA384 of the hashed data for signing: Hash ( Hash (Data input) || Hash (Signer identifier input) )
    sha384      hash;
    OCTETSTRING hashData1; // Hash (Data input)
    hash.generate(p__toBeVerifiedData, hashData1);
    OCTETSTRING hashData2;                        // Hash (Signer identifier input)
    if (p__certificateIssuer != int2oct(0, 32)) { // || Hash (Signer identifier input)
      hashData2 = p__certificateIssuer;
    } else {
      hashData2 = hash.get_sha384_empty_string(); // Hash of empty string
    }
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash (Data input)=", hashData1);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash (Signer identifier input)=", hashData2);
    hashData1 += hashData2; // Hash (Data input) || Hash (Signer identifier input)
    OCTETSTRING hashData;   // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    hash.generate(hashData1, hashData);
    loggers::get_instance().log_msg("fx__verifyWithEcdsaBrainpoolp384r1WithSha384: Hash ( Hash (Data input) || Hash (Signer identifier input) )=", hashData);
    // Check the signature
    security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, p__ecdsaBrainpoolp384PublicKeyX, p__ecdsaBrainpoolp384PublicKeyY);
    if (k.sign_verif(hashData, p__signature) == 0) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * \fn OCTETSTRING fx__hmac__sha256(const OCTETSTRING& p__k, const OCTETSTRING& p__m);
   * \brief Generate a HMAC-SHA256 value based on the provided secret key
   * \param[in] p__k The secret key used for the HMAC calculation
   * \param[in] p__m The message
   * \return The HMAC value resized to 16-byte
   */
  OCTETSTRING fx__hmac__sha256(const OCTETSTRING &p__k, const OCTETSTRING &p__m) {
    loggers::get_instance().log_msg(">>> fx__hmac__sha256: p__k=", p__k);
    loggers::get_instance().log_msg(">>> fx__hmac__sha256: p__m=", p__m);

    hmac        h(hash_algorithms::sha_256); // TODO Use ec_encryption_algorithm
    OCTETSTRING t;
    if (h.generate(p__m, p__k, t) == -1) {
      loggers::get_instance().warning("fx__hmac__sha256: Failed to generate HMAC");
      return OCTETSTRING(0, nullptr);
    }

    loggers::get_instance().log_msg("<<< fx__hmac__sha256: HMAC: ", t);
    return t;
  }

  /**
   * \fn OCTETSTRING fx__encrypt__aes__128__ccm__test(const OCTETSTRING& p__k, const OCTETSTRING& p__n, const OCTETSTRING& p__pt);
   * \brief Encrypt the message using AES 128 CCM algorithm
   * \param[in] p__k The symmetric encryption key
   * \param[in] p__n The initial vector, nonce vector
   * \param[in] p__pt The message to encrypt
   * \return The encrypted message concatenated to the AES 128 CCM tag
   */
  OCTETSTRING fx__encrypt__aes__128__ccm__test(const OCTETSTRING &p__k, const OCTETSTRING &p__n, const OCTETSTRING &p__pt) {
    loggers::get_instance().log(">>> fx__encrypt__aes__128__ccm__test");

    security_ecc ec(ec_elliptic_curves::nist_p_256);
    OCTETSTRING  enc_message;
    if (ec.encrypt(encryption_algotithm::aes_128_ccm, p__k, p__n, p__pt, enc_message) == -1) {
      loggers::get_instance().warning("fx__encrypt__aes__128__ccm__test: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    OCTETSTRING os(enc_message + ec.tag());
    loggers::get_instance().log_msg("fx__encrypt__aes__128__ccm__test: encrypted message: ", os);

    return os;
  }

  /**
   * \fn OCTETSTRING fx__encrypt__aes__128__gcm__test(const OCTETSTRING& p__k, const OCTETSTRING& p__n, const OCTETSTRING& p__pt);
   * \brief Encrypt the message using AES 128 GCM algorithm
   * \param[in] p__k The symmetric encryption key
   * \param[in] p__n The initial vector, nonce vector
   * \param[in] p__pt The message to encrypt
   * \return The encrypted message concatenated to the AES 128 CCM tag
   */
  OCTETSTRING fx__encrypt__aes__128__gcm__test(const OCTETSTRING &p__k, const OCTETSTRING &p__n, const OCTETSTRING &p__pt) {
    loggers::get_instance().log(">>> fx__encrypt__aes__128__gcm__test");

    security_ecc ec(ec_elliptic_curves::nist_p_256);
    OCTETSTRING  enc_message;
    if (ec.encrypt(encryption_algotithm::aes_128_gcm, p__k, p__n, p__pt, enc_message) == -1) {
      loggers::get_instance().warning("fx__encrypt__aes__128__gcm__test: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    OCTETSTRING os(enc_message + ec.tag());
    loggers::get_instance().log_msg("fx__encrypt__aes__128__gcm__test: encrypted message: ", os);

    return os;
  }

  /**
   * \fn OCTETSTRING fx__decrypt__aes__128__ccm__test(const OCTETSTRING& p__k, const OCTETSTRING& p__n, const OCTETSTRING& p__ct);
   * \brief Encrypt the message using AES 128 CCM algorithm
   * \param[in] p__k The symmetric encryption key
   * \param[in] p__n The initial vector, nonce vector
   * \param[in] p__ct The encrypted message concatenated to the AES 128 CCM tag
   * \return The original message
   */
  OCTETSTRING fx__decrypt__aes__128__ccm__test(const OCTETSTRING &p__k, const OCTETSTRING &p__n, const OCTETSTRING &p__ct) {
    loggers::get_instance().log_msg(">>> fx__decrypt__aes__128__ccm__test: p__k: ", p__k);
    loggers::get_instance().log_msg(">>> fx__decrypt__aes__128__ccm__test: p__n: ", p__n);
    loggers::get_instance().log_msg(">>> fx__decrypt__aes__128__ccm__test: p__ct: ", p__ct);

    security_ecc ec(ec_elliptic_curves::nist_p_256);
    // Extract the tag
    OCTETSTRING tag(16, p__ct.lengthof() - 16 + static_cast<const unsigned char *>(p__ct));
    loggers::get_instance().log_msg("fx__decrypt__aes__128__ccm__test: tag: ", tag);
    // Remove the tag from the end of the encrypted message
    OCTETSTRING ct(p__ct.lengthof() - 16, static_cast<const unsigned char *>(p__ct));
    loggers::get_instance().log_msg("fx__decrypt__aes__128__ccm__test: ct: ", ct);

    OCTETSTRING message;
    if (ec.decrypt(encryption_algotithm::aes_128_ccm, p__k, p__n, tag, ct, message) == -1) {
      loggers::get_instance().warning("fx__decrypt__aes__128__ccm__test: Failed to decrypt message");
      return OCTETSTRING(0, nullptr);
    }
    loggers::get_instance().log_to_hexa("fx__decrypt__aes__128__ccm__test: decrypted message: ", message);

    return message;
  }

  /**
   * \fn OCTETSTRING fx__encryptWithEciesNistp256WithSha256(const OCTETSTRING& p__toBeEncryptedSecuredMessage, const OCTETSTRING& p__recipientsPublicKeyX, const
   * OCTETSTRING& p__recipientsPublicKeyY, OCTETSTRING& p__publicEphemeralKeyX, OCTETSTRING& p__publicEphemeralKeyY, OCTETSTRING& p__encrypted__sym__key,
   * OCTETSTRING& p__authentication__vector, OCTETSTRING& p__nonce); \brief Encrypt the message using ECIES algorithm to encrypt AES 128 CCM symmetric key, as
   * defined in IEEE Std 1609.2-2017 \param[in] p__toBeEncryptedSecuredMessage The message to be encrypted \param[in] p__recipientsPublicKeyCompressed The
   * Recipient's compressed public key \param[in] p__compressedMode The compressed mode, 0 if the latest bit of Y-coordinate is 0, 1 otherwise \param[out]
   * p__publicEphemeralKeyCompressed The public ephemeral compressed key \param[out] p__ephemeralCompressedMode The compressed mode, 0 if the latest bit of
   * Y-coordinate is 0, 1 otherwise \param[out] p__encrypted__sym__key The encrypted AES 128 symmetric key \param[out] p__authentication__vector The tag of the
   * encrypted AES 128 symmetric key \param[out] p__nonce The nonce vector \return The original message \see IEEE Std 1609.2-2017 Clause 5.3.5 Public key
   * encryption algorithms: ECIES \see https://www.nominet.uk/researchblog/how-elliptic-curve-cryptography-encryption-works/ \see
   * http://digital.csic.es/bitstream/10261/32671/1/V2-I2-P7-13.pdf
   */
  // TODO Use common function for both fx__encryptWithEciesxxx and fx__decryptWithEciesxxx function
  OCTETSTRING fx__encryptWithEciesNistp256WithSha256(const OCTETSTRING &p__toBeEncryptedSecuredMessage, const OCTETSTRING &p__recipientsPublicKeyCompressed,
                                                     const INTEGER &p__compressedMode, const OCTETSTRING &p__salt, OCTETSTRING &p__publicEphemeralKeyCompressed,
                                                     INTEGER &p__ephemeralCompressedMode, OCTETSTRING &p__aes__sym__key, OCTETSTRING &p__encrypted__sym__key,
                                                     OCTETSTRING &p__authentication__vector, OCTETSTRING &p__nonce) {
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesNistp256WithSha256: p__toBeEncryptedSecuredMessage: ", p__toBeEncryptedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesNistp256WithSha256: p__recipientsPublicKeyCompressed: ", p__recipientsPublicKeyCompressed);
    loggers::get_instance().log(">>> fx__encryptWithEciesNistp256WithSha256: p__compressedMode: %d", static_cast<int>(p__compressedMode));
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesNistp256WithSha256: p__salt: ", p__salt);

    // 1. Generate new Private/Public Ephemeral key
    std::unique_ptr<security_ecc> ec;
    ec.reset(new security_ecc(ec_elliptic_curves::nist_p_256));
    if (ec->generate() == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesNistp256WithSha256: Failed to generate ephemeral keys");
      return OCTETSTRING(0, nullptr);
    }
    // 2. Generate and derive shared secret based on recipient's private keys
    security_ecc ec_comp(ec_elliptic_curves::nist_p_256, p__recipientsPublicKeyCompressed,
                         (static_cast<int>(p__compressedMode) == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (ec->generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__salt) == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesNistp256WithSha256: Failed to generate and derive secret key");
      return OCTETSTRING(0, nullptr);
    }

    // Set the AES symmetric key
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: AES symmetric key: ", ec->symmetric_encryption_key());
    p__aes__sym__key = ec->symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: p__aes__sym__key: ", p__aes__sym__key);
    // Set the encrypted symmetric key
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: Encrypted symmetric key: ", ec->encrypted_symmetric_key());
    p__encrypted__sym__key = ec->encrypted_symmetric_key();
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    // Set the tag of the symmetric key encryption
    p__authentication__vector = ec->tag();
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: p__authentication__vector: ", p__authentication__vector);
    // Set ephemeral public keys
    p__publicEphemeralKeyCompressed = ec->public_key_compressed();
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: Ephemeral public compressed key: ", p__publicEphemeralKeyCompressed);
    p__ephemeralCompressedMode = (ec->public_key_compressed_mode() == ecc_compressed_mode::compressed_y_0) ? 0 : 1;
    loggers::get_instance().log("fx__encryptWithEciesNistp256WithSha256: Ephemeral public compressed mode: %d: ", (int)p__ephemeralCompressedMode);
    // 3. Retrieve AES 128 parameters
    p__nonce = ec->nonce();
    loggers::get_instance().log_msg("fx__encryptWithEciesNistp256WithSha256: p__nonce: ", p__nonce);
    // 4. Encrypt the data using AES-128 CCM
    OCTETSTRING enc_message;
    if (ec->encrypt(encryption_algotithm::aes_128_ccm, ec->symmetric_encryption_key(), ec->nonce(), p__toBeEncryptedSecuredMessage, enc_message) == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesNistp256WithSha256: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    enc_message += ec->tag();
    loggers::get_instance().log_to_hexa("fx__encryptWithEciesNistp256WithSha256: enc message||Tag: ", enc_message);

    return enc_message;
  }

  /**
   * @desc Test function for ECIES NIST P-256 Encryption with SHA-256
   * @remark For the purpose of testing, the content of p__toBeEncryptedSecuredMessage is the AES 128 symmetric key to be encrypted
   */
  OCTETSTRING fx__test__encryptWithEciesNistp256WithSha256(const OCTETSTRING &p__toBeEncryptedSecuredMessage, const OCTETSTRING &p__privateEphemeralKey,
                                                           const OCTETSTRING &p__recipientPublicKeyX, const OCTETSTRING &p__recipientPublicKeyY,
                                                           const OCTETSTRING &p__salt, OCTETSTRING &p__publicEphemeralKeyX, OCTETSTRING &p__publicEphemeralKeyY,
                                                           OCTETSTRING &p__aes__sym__key, OCTETSTRING &p__encrypted__sym__key,
                                                           OCTETSTRING &p__authentication__vector, OCTETSTRING &p__nonce) {

    // 1. Generate new ephemeral Private/Public keys
    security_ecc ec(ec_elliptic_curves::nist_p_256, p__privateEphemeralKey);
    p__publicEphemeralKeyX = ec.public_key_x();
    p__publicEphemeralKeyY = ec.public_key_y();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: Vx=", p__publicEphemeralKeyX);
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: Vy=", p__publicEphemeralKeyY);

    // 2. Generate and derive shared secret
    security_ecc ec_comp(ec_elliptic_curves::nist_p_256, p__recipientPublicKeyX, p__recipientPublicKeyY);
    ec.symmetric_encryption_key(p__toBeEncryptedSecuredMessage);
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: ", ec.encrypted_symmetric_key());
    if (ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__salt) == -1) {
      loggers::get_instance().warning("fx__test__encryptWithEciesNistp256WithSha256: Failed to generate and derive secret key");
      return OCTETSTRING(0, nullptr);
    }

    // Set the AES symmetric key
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: AES symmetric key: ", ec.symmetric_encryption_key());
    p__aes__sym__key = ec.symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: p__aes__sym__key: ", p__aes__sym__key);
    // Set the encrypted symmetric key
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: Encrypted symmetric key: ", ec.encrypted_symmetric_key());
    p__encrypted__sym__key = ec.encrypted_symmetric_key();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    // Set the tag of the symmetric key encryption
    p__authentication__vector = ec.tag();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: p__authentication__vector: ", p__authentication__vector);
    // 3. Retrieve AES 128 parameters
    p__nonce = ec.nonce();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesNistp256WithSha256: p__nonce: ", p__nonce);
    // 4. Encrypt the data using AES-128 CCM
    OCTETSTRING enc_message;
    if (ec.encrypt(encryption_algotithm::aes_128_ccm, ec.symmetric_encryption_key(), ec.nonce(), p__toBeEncryptedSecuredMessage, enc_message) == -1) {
      loggers::get_instance().warning("fx__test__encryptWithEciesNistp256WithSha256: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    enc_message += ec.tag();
    loggers::get_instance().log_to_hexa("fx__test__encryptWithEciesNistp256WithSha256: enc message||Tag: ", enc_message);

    return enc_message;
  }

  /**
   * \fn OCTETSTRING fx__decryptWithEciesNistp256WithSha256(const OCTETSTRING& p__encryptedSecuredMessage, const OCTETSTRING& p__privateEncKey, const
   * OCTETSTRING& p__publicEphemeralKeyX, const OCTETSTRING& p__publicEphemeralKeyY, const OCTETSTRING& p__encrypted__sym__key, const OCTETSTRING&
   * p__authentication__vector, const OCTETSTRING& p__nonce); \brief Decrypt the message using ECIES algorithm to decrypt AES 128 CCM symmetric key, as defined
   * in IEEE Std 1609.2-2017 \param[in] p__encryptedSecuredMessage The encrypted message \param[in] p__privateEncKey The private encryption key \param[in]
   * p__publicEphemeralKeyCompressed The public ephemeral compressed key \param[in] p__ephemeralCompressedMode The compressed mode, 0 if the latest bit of
   * Y-coordinate is 0, 1 otherwise \param[in] p__encrypted__sym__key The encrypted AES 128 symmetric key \param[in] p__authentication__vector The tag of the
   * encrypted AES 128 symmetric key \param[in] p__nonce The nonce vector \return The original message \see IEEE Std 1609.2-2017 Clause 5.3.5 Public key
   * encryption algorithms: ECIES \see https://www.nominet.uk/researchblog/how-elliptic-curve-cryptography-encryption-works/ \see
   * http://digital.csic.es/bitstream/10261/32671/1/V2-I2-P7-13.pdf
   */
  // TODO Use common function for both fx__encryptWithEciesxxx and fx__decryptWithEciesxxx function
  OCTETSTRING fx__decryptWithEciesNistp256WithSha256(const OCTETSTRING &p__encryptedSecuredMessage, const OCTETSTRING &p__privateEncKey,
                                                     const OCTETSTRING &p__publicEphemeralKeyCompressed, const INTEGER &p__ephemeralCompressedMode,
                                                     const OCTETSTRING &p__encrypted__sym__key, const OCTETSTRING &p__authentication__vector,
                                                     const OCTETSTRING &p__nonce, const OCTETSTRING &p__salt, OCTETSTRING &p__aes__sym__enc__key) {
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__toBeEncryptedSecuredMessage: ", p__encryptedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__privateEncKey: ", p__privateEncKey);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__publicEphemeralKeyCompressed: ", p__publicEphemeralKeyCompressed);
    loggers::get_instance().log(">>> fx__decryptWithEciesNistp256WithSha256: p__ephemeralCompressedMode: %d", static_cast<int>(p__ephemeralCompressedMode));
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__nonce: ", p__nonce);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__authentication__vector: ", p__authentication__vector);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesNistp256WithSha256: p__salt", p__salt);

    // 1. Create security_ecc instance based on recipient's private key
    security_ecc ec(ec_elliptic_curves::nist_p_256, p__privateEncKey);
    security_ecc ec_comp(ec_elliptic_curves::nist_p_256, p__publicEphemeralKeyCompressed,
                         (static_cast<int>(p__ephemeralCompressedMode) == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);

    // 2. Generate the shared secret value based on public ephemeral keys will be required
    if (ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__encrypted__sym__key,
                                             p__nonce, p__authentication__vector, p__salt) == -1) {
      loggers::get_instance().warning("fx__decryptWithEciesNistp256WithSha256: Failed to generate shared secret");
      return OCTETSTRING(0, nullptr);
    }

    // Decrypt the message
    OCTETSTRING enc_message(p__encryptedSecuredMessage.lengthof() - ec.tag().lengthof(),
                            static_cast<const unsigned char *>(p__encryptedSecuredMessage));               // Extract the encrypted message
    loggers::get_instance().log_msg("fx__decryptWithEciesNistp256WithSha256: enc_message: ", enc_message); // Extract the ctag value
    OCTETSTRING tag(ec.tag().lengthof(),
                    static_cast<const unsigned char *>(p__encryptedSecuredMessage) + p__encryptedSecuredMessage.lengthof() - ec.tag().lengthof());
    loggers::get_instance().log_msg("fx__decryptWithEciesNistp256WithSha256: tag: ", tag);
    OCTETSTRING message;
    if (ec.decrypt(tag, enc_message, message) == -1) {
      loggers::get_instance().warning("fx__decryptWithEciesNistp256WithSha256: Failed to generate shared secret");
      return OCTETSTRING(0, nullptr);
    }
    loggers::get_instance().log_msg("fx__decryptWithEciesNistp256WithSha256: dec message: ", message);
    p__aes__sym__enc__key = ec.symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__decryptWithEciesNistp256WithSha256: AES 128 encryption key: ", p__aes__sym__enc__key);

    return message;
  }

  OCTETSTRING fx__encryptWithEciesBrainpoolp256r1WithSha256(const OCTETSTRING &p__toBeEncryptedSecuredMessage,
                                                            const OCTETSTRING &p__recipientsPublicKeyCompressed, const INTEGER &p__compressedMode,
                                                            const OCTETSTRING &p__salt, OCTETSTRING &p__publicEphemeralKeyCompressed,
                                                            INTEGER &p__ephemeralCompressedMode, OCTETSTRING &p__aes__sym__key,
                                                            OCTETSTRING &p__encrypted__sym__key, OCTETSTRING &p__authentication__vector,
                                                            OCTETSTRING &p__nonce) {
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesBrainpoolp256r1WithSha256: p__toBeEncryptedSecuredMessage: ", p__toBeEncryptedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesBrainpoolp256r1WithSha256: p__recipientsPublicKeyCompressed: ", p__recipientsPublicKeyCompressed);
    loggers::get_instance().log(">>> fx__encryptWithEciesBrainpoolp256r1WithSha256: p__compressedMode: %d", static_cast<int>(p__compressedMode));
    loggers::get_instance().log_msg(">>> fx__encryptWithEciesBrainpoolp256r1WithSha256: p__salt: ", p__salt);

    // 1. Generate new Private/Public Ephemeral key
    std::unique_ptr<security_ecc> ec;
    ec.reset(new security_ecc(ec_elliptic_curves::brainpool_p_256_r1));
    if (ec->generate() == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesBrainpoolp256r1WithSha256: Failed to generate ephemeral keys");
      return OCTETSTRING(0, nullptr);
    }
    // 2. Generate and derive shared secret based on recipient's private keys
    security_ecc ec_comp(ec_elliptic_curves::brainpool_p_256_r1, p__recipientsPublicKeyCompressed,
                         (static_cast<int>(p__compressedMode) == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);
    if (ec->generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__salt) == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesBrainpoolp256r1WithSha256: Failed to generate and derive secret key");
      return OCTETSTRING(0, nullptr);
    }

    // Set the AES symmetric key
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: AES symmetric key: ", ec->symmetric_encryption_key());
    p__aes__sym__key = ec->symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: p__aes__sym__key: ", p__aes__sym__key);
    // Set the encrypted symmetric key
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: Encrypted symmetric key: ", ec->encrypted_symmetric_key());
    p__encrypted__sym__key = ec->encrypted_symmetric_key();
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    // Set the tag of the symmetric key encryption
    p__authentication__vector = ec->tag();
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: p__authentication__vector: ", p__authentication__vector);
    // Set ephemeral public keys
    p__publicEphemeralKeyCompressed = ec->public_key_compressed();
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: Ephemeral public compressed key: ", p__publicEphemeralKeyCompressed);
    p__ephemeralCompressedMode = (ec->public_key_compressed_mode() == ecc_compressed_mode::compressed_y_0) ? 0 : 1;
    loggers::get_instance().log("fx__encryptWithEciesBrainpoolp256r1WithSha256: Ephemeral public compressed mode: %d: ", (int)p__ephemeralCompressedMode);
    // 3. Retrieve AES 128 parameters
    p__nonce = ec->nonce();
    loggers::get_instance().log_msg("fx__encryptWithEciesBrainpoolp256r1WithSha256: p__nonce: ", p__nonce);
    // 4. Encrypt the data using AES-128 CCM
    OCTETSTRING enc_message;
    if (ec->encrypt(encryption_algotithm::aes_128_ccm, ec->symmetric_encryption_key(), ec->nonce(), p__toBeEncryptedSecuredMessage, enc_message) == -1) {
      loggers::get_instance().warning("fx__encryptWithEciesBrainpoolp256r1WithSha256: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    enc_message += ec->tag();
    loggers::get_instance().log_to_hexa("fx__encryptWithEciesBrainpoolp256r1WithSha256: enc message||Tag: ", enc_message);

    return enc_message;
  }

  /**
   * @desc Test function for ECIES BRAINPOOL P-256r1 Encryption with SHA-256
   * @remark For the purpose of testing, the content of p__toBeEncryptedSecuredMessage is the AES 128 symmetric key to be encrypted
   */
  OCTETSTRING fx__test__encryptWithEciesBrainpoolp256r1WithSha256(const OCTETSTRING &p__toBeEncryptedSecuredMessage, const OCTETSTRING &p__privateEphemeralKey,
                                                                  const OCTETSTRING &p__recipientPublicKeyX, const OCTETSTRING &p__recipientPublicKeyY,
                                                                  const OCTETSTRING &p__salt, OCTETSTRING &p__publicEphemeralKeyX,
                                                                  OCTETSTRING &p__publicEphemeralKeyY, OCTETSTRING &p__aes__sym__key,
                                                                  OCTETSTRING &p__encrypted__sym__key, OCTETSTRING &p__authentication__vector,
                                                                  OCTETSTRING &p__nonce) {

    // 1. Generate new ephemeral Private/Public keys
    security_ecc ec(ec_elliptic_curves::brainpool_p_256_r1, p__privateEphemeralKey);
    p__publicEphemeralKeyX = ec.public_key_x();
    p__publicEphemeralKeyY = ec.public_key_y();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: Vx=", p__publicEphemeralKeyX);
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: Vy=", p__publicEphemeralKeyY);

    // 2. Generate and derive shared secret
    security_ecc ec_comp(ec_elliptic_curves::brainpool_p_256_r1, p__recipientPublicKeyX, p__recipientPublicKeyY);
    ec.symmetric_encryption_key(p__toBeEncryptedSecuredMessage);
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: ", ec.encrypted_symmetric_key());
    if (ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__salt) == -1) {
      loggers::get_instance().warning("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: Failed to generate and derive secret key");
      return OCTETSTRING(0, nullptr);
    }

    // Set the AES symmetric key
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: AES symmetric key: ", ec.symmetric_encryption_key());
    p__aes__sym__key = ec.symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: p__aes__sym__key: ", p__aes__sym__key);
    // Set the encrypted symmetric key
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: Encrypted symmetric key: ", ec.encrypted_symmetric_key());
    p__encrypted__sym__key = ec.encrypted_symmetric_key();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    // Set the tag of the symmetric key encryption
    p__authentication__vector = ec.tag();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: p__authentication__vector: ", p__authentication__vector);
    // 3. Retrieve AES 128 parameters
    p__nonce = ec.nonce();
    loggers::get_instance().log_msg("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: p__nonce: ", p__nonce);
    // 4. Encrypt the data using AES-128 CCM
    OCTETSTRING enc_message;
    if (ec.encrypt(encryption_algotithm::aes_128_ccm, ec.symmetric_encryption_key(), ec.nonce(), p__toBeEncryptedSecuredMessage, enc_message) == -1) {
      loggers::get_instance().warning("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: Failed to encrypt message");
      return OCTETSTRING(0, nullptr);
    }
    enc_message += ec.tag();
    loggers::get_instance().log_to_hexa("fx__test__encryptWithEciesBrainpoolp256r1WithSha256: enc message||Tag: ", enc_message);

    return enc_message;
  }

  OCTETSTRING fx__decryptWithEciesBrainpoolp256r1WithSha256(const OCTETSTRING &p__encryptedSecuredMessage, const OCTETSTRING &p__privateEncKey,
                                                            const OCTETSTRING &p__publicEphemeralKeyCompressed, const INTEGER &p__ephemeralCompressedMode,
                                                            const OCTETSTRING &p__encrypted__sym__key, const OCTETSTRING &p__authentication__vector,
                                                            const OCTETSTRING &p__nonce, const OCTETSTRING &p__salt, OCTETSTRING &p__aes__sym__enc__key) {
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__toBeEncryptedSecuredMessage: ", p__encryptedSecuredMessage);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__privateEncKey: ", p__privateEncKey);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__publicEphemeralKeyCompressed: ", p__publicEphemeralKeyCompressed);
    loggers::get_instance().log(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__ephemeralCompressedMode: %d",
                                static_cast<int>(p__ephemeralCompressedMode));
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__nonce: ", p__nonce);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__authentication__vector: ", p__authentication__vector);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__encrypted__sym__key: ", p__encrypted__sym__key);
    loggers::get_instance().log_msg(">>> fx__decryptWithEciesBrainpoolp256r1WithSha256: p__salt", p__salt);

    // 1. Create security_ecc instance based on public ephemeral keys
    security_ecc ec(ec_elliptic_curves::brainpool_p_256_r1, p__privateEncKey);
    security_ecc ec_comp(ec_elliptic_curves::brainpool_p_256_r1, p__publicEphemeralKeyCompressed,
                         (static_cast<int>(p__ephemeralCompressedMode) == 0) ? ecc_compressed_mode::compressed_y_0 : ecc_compressed_mode::compressed_y_1);

    // 2. Generate the shared secret value based on public ephemeral keys will be required
    if (ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), p__encrypted__sym__key,
                                             p__nonce, p__authentication__vector, OCTETSTRING(0, nullptr)) == -1) {
      loggers::get_instance().warning("fx__decryptWithEciesBrainpoolp256r1WithSha256: Failed to generate shared secret");
      return OCTETSTRING(0, nullptr);
    }

    // Decrypt the message
    OCTETSTRING enc_message(p__encryptedSecuredMessage.lengthof() - ec.tag().lengthof(),
                            static_cast<const unsigned char *>(p__encryptedSecuredMessage));                      // Extract the encrypted message
    loggers::get_instance().log_msg("fx__decryptWithEciesBrainpoolp256r1WithSha256: enc_message: ", enc_message); // Extract the ctag value
    OCTETSTRING tag(ec.tag().lengthof(),
                    static_cast<const unsigned char *>(p__encryptedSecuredMessage) + p__encryptedSecuredMessage.lengthof() - ec.tag().lengthof());
    loggers::get_instance().log_msg("fx__decryptWithEciesBrainpoolp256r1WithSha256: tag: ", tag);
    OCTETSTRING message;
    if (ec.decrypt(tag, enc_message, message) == -1) {
      loggers::get_instance().warning("fx__decryptWithEciesBrainpoolp256r1WithSha256: Failed to generate shared secret");
      return OCTETSTRING(0, nullptr);
    }
    loggers::get_instance().log_msg("fx__decryptWithEciesBrainpoolp256r1WithSha256: dec message: ", message);
    p__aes__sym__enc__key = ec.symmetric_encryption_key();
    loggers::get_instance().log_msg("fx__decryptWithEciesBrainpoolp256r1WithSha256: AES 128 encryption key: ", p__aes__sym__enc__key);

    return message;
  }

  /**
   * \brief    Produce a new public/private key pair based on Elliptic Curve Digital Signature Algorithm (ECDSA) algorithm.
   *          This function should not be used by the ATS
   * \param   p_privateKey    The new private key value
   * \param   p_publicKeyX    The new public key value (x coordinate)
   * \param   p_publicKeyX    The new public key value (y coordinate)
   * \return  true on success, false otherwise
   fx_generateKeyPair_nistp256(out octetstring<UInt64> p_privateKey, out octetstring p_publicKeyX, out octetstring p_publicKeyY) return boolean;
  */
  BOOLEAN fx__generateKeyPair__nistp256(OCTETSTRING &p__privateKey, OCTETSTRING &p__publicKeyX, OCTETSTRING &p__publicKeyY, OCTETSTRING &p__publicKeyCompressed,
                                        INTEGER &p__compressedMode) {
    security_ecc k(ec_elliptic_curves::nist_p_256);
    if (k.generate() != 0) {
      p__privateKey          = OCTETSTRING(0, nullptr);
      p__publicKeyX          = OCTETSTRING(0, nullptr);
      p__publicKeyY          = OCTETSTRING(0, nullptr);
      p__publicKeyCompressed = OCTETSTRING(0, nullptr);
      return FALSE;
    }
    // Sanity checks
    if (k.private_key().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__nistp256: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__nistp256: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__nistp256: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__nistp256: Invalid public compressed key size");
      return FALSE;
    }
    p__privateKey          = k.private_key();
    p__publicKeyX          = k.public_key_x();
    p__publicKeyY          = k.public_key_y();
    p__publicKeyCompressed = k.public_key_compressed();
    p__compressedMode      = INTEGER((int)k.public_key_compressed_mode());

    return TRUE;
  }

  /**
   * \brief    Produce a new public/private key pair based on Elliptic Curve Digital Signature Algorithm (ECDSA) algorithm.
   *          This function should not be used by the ATS
   * \param   p_privateKey    The new private key value
   * \param   p_publicKeyX    The new public key value (x coordinate)
   * \param   p_publicKeyX    The new public key value (y coordinate)
   * \return  true on success, false otherwise
   fx_generateKeyPair_nistp256(out octetstring<UInt64> p_privateKey, out octetstring p_publicKeyX, out octetstring p_publicKeyY) return boolean;
  */
  BOOLEAN fx__generateKeyPair__brainpoolp256r1(OCTETSTRING &p__privateKey, OCTETSTRING &p__publicKeyX, OCTETSTRING &p__publicKeyY,
                                               OCTETSTRING &p__publicKeyCompressed, INTEGER &p__compressedMode) {
    security_ecc k(ec_elliptic_curves::brainpool_p_256_r1);
    if (k.generate() != 0) {
      p__privateKey          = OCTETSTRING(0, nullptr);
      p__publicKeyX          = OCTETSTRING(0, nullptr);
      p__publicKeyY          = OCTETSTRING(0, nullptr);
      p__publicKeyCompressed = OCTETSTRING(0, nullptr);
      return FALSE;
    }

    // Sanity checks
    if (k.private_key().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp256r1: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp256r1: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp256r1: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 32) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp256r1: Invalid public compressed key size");
      return FALSE;
    }
    p__privateKey          = k.private_key();
    p__publicKeyX          = k.public_key_x();
    p__publicKeyY          = k.public_key_y();
    p__publicKeyCompressed = k.public_key_compressed();
    p__compressedMode      = INTEGER((int)k.public_key_compressed_mode());

    return TRUE;
  }

  /**
   * \brief    Produce a new public/private key pair based on Elliptic Curve Digital Signature Algorithm (ECDSA) algorithm.
   *          This function should not be used by the ATS
   * \param   p_privateKey    The new private key value
   * \param   p_publicKeyX    The new public key value (x coordinate)
   * \param   p_publicKeyX    The new public key value (y coordinate)
   * \return  true on success, false otherwise
   fx_generateKeyPair_nistp256(out octetstring<UInt64> p_privateKey, out octetstring p_publicKeyX, out octetstring p_publicKeyY) return boolean;
  */
  BOOLEAN fx__generateKeyPair__brainpoolp384r1(OCTETSTRING &p__privateKey, OCTETSTRING &p__publicKeyX, OCTETSTRING &p__publicKeyY,
                                               OCTETSTRING &p__publicKeyCompressed, INTEGER &p__compressedMode) {
    security_ecc k(ec_elliptic_curves::brainpool_p_384_r1);
    if (k.generate() != 0) {
      p__privateKey          = OCTETSTRING(0, nullptr);
      p__publicKeyX          = OCTETSTRING(0, nullptr);
      p__publicKeyY          = OCTETSTRING(0, nullptr);
      p__publicKeyCompressed = OCTETSTRING(0, nullptr);
      return FALSE;
    }

    // Sanity checks
    if (k.private_key().lengthof() != 48) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp384r1: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 48) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp384r1: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 48) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp384r1: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 48) {
      loggers::get_instance().error("fx__generateKeyPair__brainpoolp384r1: Invalid public compressed key size");
      return FALSE;
    }
    p__privateKey          = k.private_key();
    p__publicKeyX          = k.public_key_x();
    p__publicKeyY          = k.public_key_y();
    p__publicKeyCompressed = k.public_key_compressed();
    p__compressedMode      = INTEGER((int)k.public_key_compressed_mode());

    return TRUE;
  }

  BOOLEAN fx__get__uncompressed__key__nistp256(const OCTETSTRING &p__privateKey, const OCTETSTRING &p__publicKeyCompressed, const INTEGER &p__compressedMode,
                                               OCTETSTRING &p__publicKeyY) {
    security_ecc k(ec_elliptic_curves::nist_p_256, p__privateKey);

    // Sanity checks
    if (k.private_key().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid public compressed key size");
      return FALSE;
    }
    // if (p__compressedMode != INTEGER((int)k.public_key_compressed_mode())) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid public compressed mode");
    //   return FALSE;
    // }
    // if (p__publicKeyCompressed != k.public_key_compressed()) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__nistp256: Invalid public compressed key value");
    //   return FALSE;
    // }

    p__publicKeyY = k.public_key_y();

    return TRUE;
  }

  BOOLEAN fx__get__uncompressed__key__brainpoolp256r1(const OCTETSTRING &p__privateKey, const OCTETSTRING &p__publicKeyCompressed,
                                                      const INTEGER &p__compressedMode, OCTETSTRING &p__publicKeyY) {
    security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, p__privateKey);

    // Sanity checks
    if (k.private_key().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 32) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid public compressed key size");
      return FALSE;
    }
    // if (p__compressedMode != INTEGER((int)k.public_key_compressed_mode())) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid public compressed mode");
    //   return FALSE;
    // }
    // if (p__publicKeyCompressed != k.public_key_compressed()) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp256r1: Invalid public compressed key value");
    //   return FALSE;
    // }

    p__publicKeyY = k.public_key_y();

    return TRUE;
  }

  BOOLEAN fx__get__uncompressed__key__brainpoolp384r1(const OCTETSTRING &p__privateKey, const OCTETSTRING &p__publicKeyCompressed,
                                                      const INTEGER &p__compressedMode, OCTETSTRING &p__publicKeyY) {
    security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, p__privateKey);

    // Sanity checks
    if (k.private_key().lengthof() != 48) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid private key size");
      return FALSE;
    }
    if (k.public_key_x().lengthof() != 48) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid public key X-coordonate size");
      return FALSE;
    }
    if (k.public_key_y().lengthof() != 48) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid public key Y-coordonate size");
      return FALSE;
    }
    if (k.public_key_compressed().lengthof() != 48) {
      loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid public compressed key size");
      return FALSE;
    }
    // if (p__compressedMode != INTEGER((int)k.public_key_compressed_mode())) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid public compressed mode");
    //   return FALSE;
    // }
    // if (p__publicKeyCompressed != k.public_key_compressed()) {
    //   loggers::get_instance().error("fx__get__uncompressed__key__brainpoolp384r1: Invalid public compressed key value");
    //   return FALSE;
    // }

    p__publicKeyY = k.public_key_y();

    return TRUE;
  }

  //        group encryption

  //        group certificatesLoader

  /**
   * \brief    Load in memory cache the certificates available in the specified directory
   * \param   p_rootDirectory Root directory to access to the certificates identified by the certificate ID
   * \param   p_configId      A configuration identifier
   * @remark  This method SHALL be call before any usage of certificates
   * \return  true on success, false otherwise
   fx_loadCertificates(in charstring p_rootDirectory, in charstring p_configId) return boolean;
  */
  BOOLEAN fx__loadCertificates(const CHARSTRING &p__rootDirectory, const CHARSTRING &p__configId) {
    loggers::get_instance().log(">>> fx__loadCertificates: '%s', '%s'", static_cast<const char *>(p__rootDirectory), static_cast<const char *>(p__configId));

    std::string str(static_cast<const char *>(p__rootDirectory));
    if (p__configId.lengthof() != 0) {
      str += "/";
      str += std::string(static_cast<const char *>(p__configId));
    }
    params params;
    params.insert(std::pair<std::string, std::string>(std::string("sec_db_path"), str));
    if (security_services::get_instance().setup(params) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  BOOLEAN fx__store__certificate(const CHARSTRING &p__cert__id, const OCTETSTRING &p__cert, const OCTETSTRING &p__private__key,
                                 const OCTETSTRING &p__public__key__x, const OCTETSTRING &p__public__key__y, const OCTETSTRING &p__public__key__compressed,
                                 const INTEGER &p__public__key__compressed__mode, const OCTETSTRING &p__hash, const OCTETSTRING &p__hash__256,
                                 const OCTETSTRING &p__hashid8, const OCTETSTRING &p__issuer, const OCTETSTRING_template &p__private__enc__key,
                                 const OCTETSTRING_template &p__public__enc__key__x, const OCTETSTRING_template &p__public__enc__key__y,
                                 const OCTETSTRING_template &p__public__enc__compressed__key, const INTEGER_template &p__public__enc__key__compressed__mode) {
    loggers::get_instance().log(">>> fx__store__certificate: '%s'", static_cast<const char *>(p__cert__id));

    int result;
    if (!p__private__enc__key.is_omit()) {
      const OCTETSTRING private_enc_key  = p__private__enc__key.valueof();
      const OCTETSTRING public_enc_key_x = p__public__enc__key__x.valueof();
      const OCTETSTRING public_enc_key_y = p__public__enc__key__y.valueof();
      result                             = security_services::get_instance().store_certificate(
        p__cert__id, p__cert, p__private__key, p__public__key__x, p__public__key__y, p__public__key__compressed, p__public__key__compressed__mode, p__hash,
        p__hash__256, p__hashid8, p__issuer, p__private__enc__key.valueof(), p__public__enc__key__x.valueof(), p__public__enc__key__y.valueof(),
        p__public__enc__compressed__key.valueof(), p__public__enc__key__compressed__mode.valueof());
    } else {
      result = security_services::get_instance().store_certificate(
        p__cert__id, p__cert, p__private__key, p__public__key__x, p__public__key__y, p__public__key__compressed, p__public__key__compressed__mode, p__hash,
        p__hash__256, p__hashid8, p__issuer, OCTETSTRING(0, nullptr), OCTETSTRING(0, nullptr), OCTETSTRING(0, nullptr), OCTETSTRING(0, nullptr), INTEGER(-1));
    }

    return (result == 0);
  }

  /**
   * \brief    Unload from memory cache the certificates
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__unloadCertificates() { return TRUE; }

  /**
   * \brief    Read the specified certificate
   * \param   p_certificate_id the certificate identifier
   * \param   p_certificate   the expected certificate
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__readCertificate(const CHARSTRING &p__certificateId, OCTETSTRING &p__certificate) {
    loggers::get_instance().log(">>> fx__readCertificate: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_certificate(p__certificateId, p__certificate) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  BOOLEAN fx__readCertificateFromDigest(const OCTETSTRING &p__digest, CHARSTRING &p__certificateId) {
    loggers::get_instance().log_msg(">>> fx__readCertificateFromDigest: ", p__digest);

    if (security_services::get_instance().read_certificate_from_digest(p__digest, p__certificateId) == -1) {
      return FALSE;
    }
    loggers::get_instance().log_msg("fx__readCertificateFromDigest: ", p__certificateId);

    return TRUE;
  }

  BOOLEAN fx__readCertificateFromHashedId3(const OCTETSTRING &p__digest, CHARSTRING &p__certificateId) {
    loggers::get_instance().log_msg(">>> fx__readCertificateFromHashedId3: ", p__digest);

    if (security_services::get_instance().read_certificate_from_hashed_id3(p__digest, p__certificateId) == -1) {
      return FALSE;
    }
    loggers::get_instance().log_msg("fx__readCertificateFromHashedId3: ", p__certificateId);

    return TRUE;
  }

  /**
   * \brief    Read the specified certificate digest
   * \param   p_certificate_id the certificate identifier
   * \param   p_digest   the expected certificate
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__readCertificateDigest(const CHARSTRING &p__certificateId, OCTETSTRING &p__digest) {
    loggers::get_instance().log(">>> fx__readCertificateDigest: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_certificate_digest(p__certificateId, p__digest) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * \brief   Read the whole-hash of the certificate
   * \param   p_certificate_id the certificate identifier
   * \param   p_hash   the expected certificate
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__readCertificateHash(const CHARSTRING &p__certificateId, OCTETSTRING &p__hash) {
    loggers::get_instance().log(">>> fx__readCertificateHash: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_certificate_hash(p__certificateId, p__hash) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * \brief   Read the whole-hash of the certificate using SHA 256
   * \param   p_certificate_id the certificate identifier
   * \param   p_hash   the expected certificate
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__readCertificateHash256(const CHARSTRING &p__certificateId, OCTETSTRING &p__hash) {
    loggers::get_instance().log(">>> fx__readCertificateHash256: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_certificate_hash_sha_256(p__certificateId, p__hash) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * \brief    Read the private keys for the specified certificate
   * \param   p_certificate_id     the keys identifier
   * \param   p_signingPrivateKey the signing private key
   * \return  true on success, false otherwise
   */
  BOOLEAN fx__readSigningKey(const CHARSTRING &p__certificateId, OCTETSTRING &p__signingPrivateKey) {
    loggers::get_instance().log(">>> fx__readSigningKey: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_private_key(p__certificateId, p__signingPrivateKey) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * \brief    Read the private keys for the specified certificate
   * \param   p_keysId            the keys identifier
   * \param   p_encryptPrivateKey the encrypt private key
   * \return  true on success, false otherwise
   fx_readEncryptingKey(in charstring p_keysId, out Oct32 p_encryptingPrivateKey) return boolean;
  */
  BOOLEAN fx__readEncryptingKey(const CHARSTRING &p__certificateId, OCTETSTRING &p__encryptingPrivateKey) {
    loggers::get_instance().log(">>> fx__readSigningKey: '%s'", static_cast<const char *>(p__certificateId));

    if (security_services::get_instance().read_private_enc_key(p__certificateId, p__encryptingPrivateKey) == -1) {
      return FALSE;
    }

    return TRUE;
  }

  //        group geospacial

  /*          * \brief    Check that given polygon doesn't have neither self-intersections nor holes.
   * \param   p_region   Polygonal Region
   * \return  true on success, false otherwise
   * @verdict Unchanged
   fx_isValidPolygonalRegion(in PolygonalRegion p_region) return boolean;
  */
  BOOLEAN fx__isValidPolygonalRegion(const Ieee1609Dot2BaseTypes::PolygonalRegion &p__region) { return TRUE; }

  /*          * \brief Check if a polygonal region is inside another one
   * \param p_parent  The main polygonal region
   * \param p_region  The polygonal region to be included
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isPolygonalRegionInside(in PolygonalRegion p_parent, in PolygonalRegion p_region) return boolean;
  */
  BOOLEAN fx__isPolygonalRegionInside(const Ieee1609Dot2BaseTypes::PolygonalRegion &p__parent, const Ieee1609Dot2BaseTypes::PolygonalRegion &p__region) {
    return TRUE;
  }

  /*          * \brief Check that the location is inside a circular region
   * \param p_region      The circular region to consider
   * \param p_location    The device location
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isLocationInsideCircularRegion(in CircularRegion p_region, in ThreeDLocation p_location) return boolean;
  */
  BOOLEAN fx__isLocationInsideCircularRegion(const Ieee1609Dot2BaseTypes::CircularRegion &p__region, const Ieee1609Dot2BaseTypes::ThreeDLocation &p__location) {
    return TRUE;
  }

  /*          * \brief Check that the location is inside a rectangular region
   * \param p_region      The rectangular region to consider
   * \param p_location    The device location
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isLocationInsideRectangularRegion(in SequenceOfRectangularRegion p_region, in ThreeDLocation p_location) return boolean;
  */
  BOOLEAN fx__isLocationInsideRectangularRegion(const Ieee1609Dot2BaseTypes::SequenceOfRectangularRegion &p__region,
                                                const Ieee1609Dot2BaseTypes::ThreeDLocation &             p__location) {
    return TRUE;
  }

  /*          * \brief Check that the location is inside a polygonal region
   * \param p_region      The polygonal region to consider
   * \param p_location    The device location
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isLocationInsidePolygonalRegion(in PolygonalRegion p_region, in ThreeDLocation p_location) return boolean;
  */
  BOOLEAN fx__isLocationInsidePolygonalRegion(const Ieee1609Dot2BaseTypes::PolygonalRegion &p__region,
                                              const Ieee1609Dot2BaseTypes::ThreeDLocation & p__location) {
    return TRUE;
  }

  /*          * \brief Check if the location is inside an identified region
   * \param p_region      The identified region to consider
   * \param p_location    The device location
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isLocationInsideIdentifiedRegion(in IdentifiedRegion p_region, in ThreeDLocation p_location) return boolean;
  */
  BOOLEAN fx__isLocationInsideIdentifiedRegion(const Ieee1609Dot2BaseTypes::IdentifiedRegion &p__region,
                                               const Ieee1609Dot2BaseTypes::ThreeDLocation &  p__location) {
    return TRUE;
  }

  /*          * \brief Check if the location is inside an undefined region
   * \param p_region      The identified region to consider
   * \param p_location    The device location
   * \return true on success, false otherwise
   * @verdict Unchanged
   fx_isLocationInsideOtherRegion(in octetstring p_region, in ThreeDLocation p_location) return boolean;
  */
  BOOLEAN fx__isLocationInsideOtherRegion(const OCTETSTRING &p_region, const Ieee1609Dot2BaseTypes::ThreeDLocation &p_location) { return TRUE; }

  /**
   * \brief    Check that p_circular_region_1 circular region is included into p_circular_region_2 circular region
   * \param   p_circular_region_1    Circular region 1
   * \param   p_circular_region_2    Circular region 2
   * \return  true on success, false otherwise
   fx_areCirclesInside(in CircularRegion p_circular_region_1, in CircularRegion p_circular_region_2) return boolean;
  */
  BOOLEAN fx__areCirclesInside(const Ieee1609Dot2BaseTypes::CircularRegion &p_circular_region_1,
                               const Ieee1609Dot2BaseTypes::CircularRegion &p_circular_region_2) {
    // Compute distance between the 2 radius
    FLOAT d = LibItsCommon__Functions::fx__computeDistance(p_circular_region_1.center().latitude(), p_circular_region_1.center().longitude(),
                                                           p_circular_region_2.center().latitude(), p_circular_region_2.center().longitude());
    if ((float)abs(p_circular_region_1.radius() - p_circular_region_2.radius()) > (float)d) {
      return FALSE;
    }
    return TRUE;
  }

  /*           * \brief    Check that p_rectanglar_region_1 rectangular region is included into p_rectanglar_region_2 rectangular region
   * \param   p_rectanglar_region_1    Rectangular region 1
   * \param   p_rectanglar_region_2    Rectangular region 2
   * \return  true on success, false otherwise
   fx_areRectanglesInside(in SequenceOfRectangularRegion p_rectanglar_region_1, in SequenceOfRectangularRegion p_rectanglar_region_2) return boolean;
  */
  BOOLEAN fx__areRectanglesInside(const Ieee1609Dot2BaseTypes::SequenceOfRectangularRegion &p_rectanglar_region_1,
                                  const Ieee1609Dot2BaseTypes::SequenceOfRectangularRegion &p_rectanglar_region_2) {

    return TRUE;
  }

  /*           * \brief    Check that p_polygonal_region_1 polygonal region is included into p_polygonal_region_2 polygonal region
   * \param   p_polygonal_region_1    Polygonal region 1
   * \param   p_polygonal_region_2    Polygonal region 2
   * \return  true on success, false otherwise
   fx_arePolygonsInside(in PolygonalRegion p_polygonal_region_1, in PolygonalRegion p_polygonal_region_2) return boolean;
  */
  BOOLEAN fx__arePolygonsInside(const Ieee1609Dot2BaseTypes::PolygonalRegion &p_polygonal_region_1,
                                const Ieee1609Dot2BaseTypes::PolygonalRegion &p_polygonal_region_2) {
    return TRUE;
  }

  /**
   * \brief Convert a spacial coordinate from DMS to DMS
   * \param p_degrees The degrees (D)
   * \param p_minutes The minutes (M)
   * \param p_seconds The seconds (S)
   * \param p_latlon  The latitude/longitude: (N|S|E|W)
   * \return The decimal coordinate on success, 0.0, otherwise
   * @verdict Unchanged
   fx_dms2dd(in Int p_degrees, in Int p_minutes, in float p_seconds, in Oct1 p_latlon) return float;
  */
  FLOAT fx__dms2dd(const INTEGER &p__degrees, const INTEGER &p__minutes, const FLOAT &p__seconds, const OCTETSTRING &p__latlon) { return 0.0; }

  FLOAT fx__int2ddlat(const INTEGER &p__latitude) { return (float)((float)p__latitude / 10000000.0); }

  FLOAT fx__int2ddlon(const INTEGER &p__longitude) { return (float)((float)p__longitude / 1000000.0); }

} // namespace LibItsSecurity__Functions
