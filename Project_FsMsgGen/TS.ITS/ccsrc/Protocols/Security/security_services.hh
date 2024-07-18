#pragma once

#include <memory>

#include "params.hh"

#include "security_db.hh"

#include "security_ecc.hh"

#include "geospacial.hh"

class OCTETSTRING; //! TITAN forward declaration
class CHARSTRING;  //! TITAN forward declaration

namespace Ieee1609Dot2BaseTypes {
  class HashAlgorithm; //! TITAN forward declaration
  class Signature;     //! TITAN forward declaration
} // namespace Ieee1609Dot2BaseTypes

namespace Ieee1609Dot2 {
  class Ieee1609Dot2Data;    //! TITAN forward declaration
  class Ieee1609Dot2Content; //! TITAN forward declaration
  class ToBeSignedData;      //! TITAN forward declaration
  class SignedData;          //! TITAN forward declaration
  class EncryptedData;       //! TITAN forward declaration
  class SignerIdentifier;    //! TITAN forward declaration
} // namespace Ieee1609Dot2

/*!
 * \class security_services
 * \brief This class provides security services for all layers as specified in TSI TS 102 723-8 and ETSI TS 103 097
 * \remark Singleton pattern
 */
class security_services {

  static constexpr unsigned int ProtocolVersion = 11;

  /*!
   * \brief Unique static object reference of this class
   */
  static security_services *instance;

  params                          _params;
  bool                            _setup_done;
  std::unique_ptr<security_ecc>   _ec_keys_enc;
  std::unique_ptr<security_ecc>   _ec_keys_dec;
  std::unique_ptr<security_cache> _security_cache;
  std::unique_ptr<security_db>    _security_db;
  unsigned long long              _last_generation_time;
  OCTETSTRING                     _unknown_certificate;
  std::vector<OCTETSTRING>        _requested_certificate;
  int                             _latitude;
  int                             _longitude;
  int                             _elevation;
  geospacial                      _geospacial;

  /*!
   * \brief Default private ctor
   */
  security_services();
  /*!
   * \brief Default private dtor
   */
  ~security_services() {
    _ec_keys_enc.reset(nullptr);
    _security_db.reset(nullptr);
    _security_cache.reset(nullptr);
    if (instance != NULL) {
      delete instance;
      instance = NULL;
    }
  };

public: /*! \publicsection */
  /*!
   * \brief Public accessor to the single object reference
   */
  inline static security_services &get_instance() {
    if (instance == NULL)
      instance = new security_services();
    return *instance;
  };

  /*!
   * \fn int verify_and_extract_gn_payload(const OCTETSTRING& p_secured_gn_payload, const bool p_verify, OCTETSTRING& p_unsecured_gn_payload, params& p_params);
   * \brief Verify and extract the unsecured payload from the provided secured payload.
   *        The secured payload could signed only, encryted only or signed and encrypted
   * \param[in] p_secured_gn_payload The secured payload to be processed
   * \param[in] p_verify Set to true if security checks shall be applied
   * \param[out] p_unsecured_gn_payload The extracted payload
   * \param[out] p_ieee_1609dot2_data The secured message
   * \param[inout] p_params The Test System parameters
   * \return 0 on success, negative value otherwise
   */
  int verify_and_extract_gn_payload(const OCTETSTRING &p_secured_gn_payload, const bool p_verify, Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data,
                                    OCTETSTRING &p_unsecured_gn_payload, params &p_params);
  /*!
   * \fn int secure_gn_payload(const OCTETSTRING& p_unsecured_gn_payload, OCTETSTRING& p_secured_gn_payload, params& p_params);
   * \brief Apply security to the provided unsecured payload
   * \param[in] p_unsecured_gn_payload The unsecured payload to be processed
   * \param[in] p_secured_gn_payload The secured payload
   * \param[in] p_params The Test System parameters
   * \return 0 on success, negative value otherwise
   */
  int secure_gn_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_secured_gn_payload, params &p_params);

  int setup(params &p_params);

  int store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key, const OCTETSTRING &p_public_key_x,
                        const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_comp_key, const INTEGER &p_public_comp_key_mode,
                        const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashid8, const OCTETSTRING &p_issuer,
                        const OCTETSTRING &p_private_enc_key, const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                        const OCTETSTRING &p_public_enc_compressed_key, const INTEGER &p_public_enc_key_compressed_mode);

  inline void set_position(const int p_latitude, const int p_longitude, const int p_elevation = 0) {
    _latitude  = p_latitude;
    _longitude = p_longitude;
    _elevation = p_elevation;
  };

  int read_certificate(const CHARSTRING &p_certificate_id, OCTETSTRING &p_certificate) const;
  int read_certificate_digest(const CHARSTRING &p_certificate_id, OCTETSTRING &p_digest) const;
  int read_certificate_hash(const CHARSTRING &p_certificate_id, OCTETSTRING &p_hash) const;
  int read_certificate_hash_sha_256(const CHARSTRING &p_certificate_id, OCTETSTRING &p_hash) const;
  int read_certificate_from_digest(const OCTETSTRING &p_digest, CHARSTRING &p_certificate_id) const;
  int read_certificate_from_hashed_id3(const OCTETSTRING &p_digest, CHARSTRING &p_certificate_id) const;
  int read_private_key(const CHARSTRING &p_certificate_id, OCTETSTRING &p_private_key) const;
  int read_private_enc_key(const CHARSTRING &p_certificate_id, OCTETSTRING &p_private_enc_key) const;

  /*!
   * \fn int sign_payload(const OCTETSTRING& p_unsecured_gn_payload, OCTETSTRING& p_signed_gn_payload, params& p_params);
   * \brief Sign the payload according provided parameters
   * \param[in] p_unsecured_gn_payload The payload to be signed
   * \param[in] p_signed_gn_payload The signed payload
   * \param[in] p_params The Test System parameters
   * \return 0 on success, negative value otherwise
   */
  int sign_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_signed_gn_payload, params &p_params);
  /*!
   * \fn int encrypt_gn_payload(const OCTETSTRING& p_unsecured_gn_payload, OCTETSTRING& p_signed_gn_payload, params& p_params);
   * \brief Encrypt the payload according provided parameters
   * \param[in] p_unsecured_gn_payload The payload to be encrypted
   * \param[in] p_enc_gn_payload The encrypted payload
   * \param[in] p_params The Test System parameters
   * \return 0 on success, negative value otherwise
   */
  int encrypt_gn_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_enc_gn_payload, params &p_params);

  int decrypt_gn_payload(const OCTETSTRING &p_enc_gn_payload, OCTETSTRING &p_unsecured_gn_payload, params &p_params);

//  int sign_tbs_data(const Ieee1609Dot2::ToBeSignedData &p_tbs_data, const Ieee1609Dot2BaseTypes::HashAlgorithm &p_hashAlgorithm,
//                    const OCTETSTRING &p_private_key, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);

  const int get_protocol_version() const { return ProtocolVersion; };

private:
  /*!
   * \fn int process_ieee_1609_dot2_content(const Ieee1609Dot2::Ieee1609Dot2Content& p_ieee_1609_dot2_content, const bool p_verify, OCTETSTRING&
   * p_unsecured_payload, params& p_params); \brief Verify and extract the unsecured payload from the Ieee1609Dot2::Ieee1609Dot2Content data structure
   * \param[in] p_content The secured content to be processed
   * \param[in] p_verify Set to true if security checks shall be applied
   * \param[in] p_unsecured_payload The extracted payload
   * \return 0 on success, negative value otherwise
   */
  int process_ieee_1609_dot2_content(const Ieee1609Dot2::Ieee1609Dot2Content &p_ieee_1609_dot2_content, const bool p_verify, OCTETSTRING &p_unsecured_payload,
                                     params &p_params);
  int process_ieee_1609_dot2_signed_data(const Ieee1609Dot2::SignedData &p_signed_data, const bool p_verify, OCTETSTRING &p_unsecured_payload,
                                         params &p_params);
  int process_ieee_1609_dot2_encrypted_data(const Ieee1609Dot2::EncryptedData &p_encrypted_data, const bool p_verify, OCTETSTRING &p_unsecured_payload,
                                            params &p_params);
  int sign_tbs_data(const Ieee1609Dot2::ToBeSignedData &p_tbs_data, const Ieee1609Dot2BaseTypes::HashAlgorithm &p_hashAlgorithm,
                    Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);

  int hash_sha256(const OCTETSTRING &p_data, OCTETSTRING &p_hash_data);
  int hash_sha384(const OCTETSTRING &p_data, OCTETSTRING &p_hash_data);
  int sign_ecdsa_nistp256(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);
  int sign_ecdsa_nistp384(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);
  int sign_ecdsa_brainpoolp256r1(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);
  int sign_ecdsa_brainpoolp384r1(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params);
  int verify_sign_ecdsa_nistp256(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature, const std::string &p_certificate_id,
                                 params &p_params);
  int verify_sign_ecdsa_nistp384(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature, const std::string &p_certificate_id,
                                 params &p_params);
  int verify_sign_ecdsa_brainpoolp256r1(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature, const std::string &p_certificate_id,
                                        params &p_params);
  int verify_sign_ecdsa_brainpoolp384r1(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature, const std::string &p_certificate_id,
                                        params &p_params);

  int extract_verification_keys(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y,
                                OCTETSTRING &p_public_comp_key, INTEGER &p_public_comp_key_mode);
  int extract_encryption_keys(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_public_enc_key_x, OCTETSTRING &p_public_enc_key_y,
                              OCTETSTRING &p_public_enc_comp_key, INTEGER &p_public_enc_comp_key_mode);
  int extract_and_store_certificate(const Ieee1609Dot2::CertificateBase &p_certificate, std::string &p_certificate_id);
}; // End of class security_services
