#include <TTCN3.hh>

#include <cmath>

#include "security_services.hh"

using namespace std; // Required for isnan()
#include "etsi_ts103097_certificate_codec.hh"
#include "etsi_ts103097_data_codec.hh"
#include "etsi_ts103097_tobesigned_data_codec.hh"

#include "security_ecc.hh"
#include "sha256.hh"
#include "sha384.hh"

#include "base_time.hh"

#include "params.hh"

#include "loggers.hh"

#include "converter.hh"

security_services *security_services::instance = nullptr;

security_services::security_services()
  : _setup_done{false}, _ec_keys_enc(nullptr), _security_cache(new security_cache), _security_db(nullptr), _last_generation_time(0),
    _unknown_certificate(0, nullptr), _requested_certificate(), _latitude(0), _longitude(0), _elevation(0), _geospacial() {
  loggers::get_instance().log(">>> security_services::security_services");

  //_geospacial.load_countries_map("/home/yann/dev/TTF0002_Its/ne-countries-50m.json"); // FIXME Use parameter

} // End of ctor

int security_services::setup(params &p_params) { // FIXME Rename this method
  loggers::get_instance().log(">>> security_services::setup");
  _params = p_params;
  _params.log();

  if (_setup_done) {
    loggers::get_instance().warning("security_services::setup: Already done");
    return 0;
  }

  // Build the certificate caching
  try {
    _security_db.reset(new security_db(_params[params::sec_db_path]));
    if (_security_db.get() == nullptr) { // Memory allocation issue
      loggers::get_instance().warning("security_services::setup: _security_db pointer is NULL");
      return -1;
    }
    _setup_done = true;
  } catch (...) {
    loggers::get_instance().error("security_services::setup: Filesystem access error, terminate test suite on TTCN-3 error. Please check user name and paths "
                                  "in the test suite configuration file.");
    return -1;
  }

  // Initialise encryption mechanism
  params::const_iterator it = _params.find(params::cypher);
  if (it == _params.cend()) {
    _ec_keys_enc.reset(new security_ecc(ec_elliptic_curves::nist_p_256));
    _params.insert(std::pair<std::string, std::string>(params::cypher, std::string("NISTP-256")));
    p_params.insert(std::pair<std::string, std::string>(params::cypher, std::string("NISTP-256")));
  } else if (it->second.compare("NISTP-256")) {
    _ec_keys_enc.reset(new security_ecc(ec_elliptic_curves::nist_p_256));
  } else if (it->second.compare("BP-256")) {
    _ec_keys_enc.reset(new security_ecc(ec_elliptic_curves::brainpool_p_256_r1));
  } else {
    loggers::get_instance().warning("security_services::setup: Failed to encode ToBeSignedData");
    return -1;
  }

  return 0;
}

int security_services::store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key,
                                         const OCTETSTRING &p_public_key_x, const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_comp_key,
                                         const INTEGER &p_public_comp_key_mode, const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256,
                                         const OCTETSTRING &p_hashid8, const OCTETSTRING &p_issuer, const OCTETSTRING &p_private_enc_key,
                                         const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                                         const OCTETSTRING &p_public_enc_compressed_key, const INTEGER &p_public_enc_key_compressed_mode) {
  loggers::get_instance().log_msg(">>> security_services::store_certificate: ", p_cert_id);

  // Sanity checks
  if (_security_db.get() == nullptr) { // Setup not called
    loggers::get_instance().warning("security_services::store_certificate: Not initialised");
    return -1;
  }
  return _security_db.get()->store_certificate(p_cert_id, p_cert, p_private_key, p_public_key_x, p_public_key_y, p_public_comp_key, p_public_comp_key_mode,
                                               p_hash, p_hash_sha_256, p_hashid8, p_issuer, p_private_enc_key, p_public_enc_key_x, p_public_enc_key_y,
                                               p_public_enc_compressed_key, p_public_enc_key_compressed_mode);
}

int security_services::verify_and_extract_gn_payload(const OCTETSTRING &p_secured_gn_payload, const bool p_verify,
                                                     Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data, OCTETSTRING &p_unsecured_gn_payload,
                                                     params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::verify_and_extract_gn_payload: ", p_secured_gn_payload);

  // Sanity checks
  if (p_secured_gn_payload.lengthof() == 0) {
    return -1;
  }

  // Decode the secured message (OER encoding)
  etsi_ts103097_data_codec codec;
  codec.decode(p_secured_gn_payload, p_ieee_1609dot2_data, &p_params);
  // Sanity checks
  if (!p_ieee_1609dot2_data.is_bound()) {
    loggers::get_instance().warning("security_services::verify_and_extract_gn_payload: Unbound value, discard it");
    return -1;
  }
  if (p_verify && ((unsigned int)(int)p_ieee_1609dot2_data.protocolVersion() != security_services::ProtocolVersion)) {
    loggers::get_instance().warning("security_services::verify_and_extract_gn_payload: Wrong version protocol, discard it");
    return -1;
  }

  return process_ieee_1609_dot2_content(p_ieee_1609dot2_data.content(), p_verify, p_unsecured_gn_payload, p_params);
} // End of method verify_and_extract_gn_payload

int security_services::process_ieee_1609_dot2_content(const Ieee1609Dot2::Ieee1609Dot2Content &p_ieee_1609_dot2_content, const bool p_verify,
                                                      OCTETSTRING &p_unsecured_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::process_ieee_1609_dot2_content: ", p_ieee_1609_dot2_content);

  if (p_ieee_1609_dot2_content.ischosen(Ieee1609Dot2::Ieee1609Dot2Content::ALT_unsecuredData)) { // Unsecured packet, End of recursivity
    p_unsecured_payload = p_ieee_1609_dot2_content.unsecuredData();
  } else if (p_ieee_1609_dot2_content.ischosen(Ieee1609Dot2::Ieee1609Dot2Content::ALT_signedData)) {
    const Ieee1609Dot2::SignedData &signedData = p_ieee_1609_dot2_content.signedData();
    if (process_ieee_1609_dot2_signed_data(signedData, p_verify, p_unsecured_payload, p_params) != 0) {
      if (p_verify) {
        return -1;
      }
    }
  } else if (p_ieee_1609_dot2_content.ischosen(Ieee1609Dot2::Ieee1609Dot2Content::ALT_encryptedData)) {
    const Ieee1609Dot2::EncryptedData &encrypted_data = p_ieee_1609_dot2_content.encryptedData();
    OCTETSTRING                        signed_payload;
    if (process_ieee_1609_dot2_encrypted_data(encrypted_data, p_verify, signed_payload, p_params) != 0) {
      return -1;
    }
    loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_content: Decrypted payload: ", signed_payload);
    Ieee1609Dot2::Ieee1609Dot2Data ieee_1609dot2_data; // TODO Check if it could be reused
    if (verify_and_extract_gn_payload(signed_payload, p_verify, ieee_1609dot2_data, p_unsecured_payload, p_params) != 0) {
      if (p_verify) {
        return -1;
      }
    }
  } else if (p_ieee_1609_dot2_content.ischosen(Ieee1609Dot2::Ieee1609Dot2Content::ALT_signedCertificateRequest)) {
    // Reset certificate timer
    loggers::get_instance().log("security_services::process_ieee_1609_dot2_content: Set Certificate re-transmission flag and reset timer");
    _last_generation_time = 0;
    return 0;
  } else { // Shall never be reached
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_content: Undefined IEEE 1609.2 Content, discard it");
    if (p_verify) {
      return -1;
    }
  }

  loggers::get_instance().log_msg("<<< security_services::process_ieee_1609_dot2_content: ", p_unsecured_payload);
  return 0;
} // End of method process_ieee_1609_dot2_content

int security_services::process_ieee_1609_dot2_signed_data(const Ieee1609Dot2::SignedData &p_signed_data, const bool p_verify, OCTETSTRING &p_unsecured_payload,
                                                          params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::process_ieee_1609_dot2_signed_data: ", p_signed_data);

  // Check the headerInfo content
  const Ieee1609Dot2::HeaderInfo &header_info = p_signed_data.tbsData().headerInfo();
  p_params[params::its_aid]                   = std::to_string(header_info.psid().get_long_long_val());
  if (!header_info.generationTime().is_present()) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: HeaderInfo::GenerationTime field is missing");
    if (p_verify) {
      return -1;
    }
  } else {
    const OPTIONAL<INTEGER> &v  = dynamic_cast<const OPTIONAL<INTEGER> &>(header_info.generationTime()); // in microsecond
    unsigned long long       gt = ((INTEGER &)(*v.get_opt_value())).get_long_long_val();
    // Get current time timestamp
    unsigned long long us = base_time::get_instance().get_its_current_time_us(); // in microsecond
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: generation time check %ld / %ld, delta = %f", gt, us,
                                    abs((double)gt - (double)us));
    if (abs((double)gt - (double)us) >= 5000000.0) { // TODO Use a params for generation_time_epsilon, 5s differences
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Invalid generation time, discard it");
      if (p_verify) {
        return -1;
      }
    }
  }

  // Check p2pcdLearningRequest keys if present
  if (header_info.p2pcdLearningRequest().is_present()) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: p2pcdLearningRequest not supported yet");
    // TODO Not supported by C-ITS Protocol
  }

  // Check missingCrlIdentifier keys if present
  if (header_info.missingCrlIdentifier().is_present()) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: missingCrlIdentifier not supported yet");
    // TODO Not supported by C-ITS Protocol
  }

  // Check encryption keys if present
  if (header_info.encryptionKey().is_present()) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: encryptionKey not supported yet");
    // TODO Not supported by C-ITS Protocol
  }

  // Check request certificate
  if (header_info.inlineP2pcdRequest().is_present()) {
    loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: inlineP2pcdRequest: ", header_info.inlineP2pcdRequest());
    const Ieee1609Dot2BaseTypes::SequenceOfHashedId3 &s =
      static_cast<const Ieee1609Dot2BaseTypes::SequenceOfHashedId3 &>(*header_info.inlineP2pcdRequest().get_opt_value());
    _requested_certificate.clear();
    for (int i = 0; i < s.lengthof(); i++) {
      loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: Add requested certificate= ", s[i]);
      _requested_certificate.push_back(s[i]);
    } // End of 'for' statement
  }

  // Check requested certificate
  if (header_info.requestedCertificate().is_present()) {
    Ieee1609Dot2::CertificateBase requested_cert = header_info.requestedCertificate();
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: ", requested_cert);
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: requestedCertificate not supported yet");
    // TODO Not supported by C-ITS Protocol
  }

  // Check and extract unsecured payload
  if (p_signed_data.tbsData().payload().data().is_present()) {
    // Check protocol version
    const OPTIONAL<Ieee1609Dot2::Ieee1609Dot2Data> &v =
      dynamic_cast<const OPTIONAL<Ieee1609Dot2::Ieee1609Dot2Data> &>(p_signed_data.tbsData().payload().data());
    loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: SignedDataPayload.data=", v);
    const Ieee1609Dot2::Ieee1609Dot2Data &ieee_1609dot2_data = static_cast<const Ieee1609Dot2::Ieee1609Dot2Data &>(*v.get_opt_value());
    if (p_verify && ((unsigned int)(int)ieee_1609dot2_data.protocolVersion() != security_services::ProtocolVersion)) {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Wrong version protocol, discard it");
      if (p_verify) {
        return -1;
      }
    }
    if (process_ieee_1609_dot2_content(ieee_1609dot2_data.content(), p_verify, p_unsecured_payload, p_params) != 0) {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Failed to process SignedData, discard it");
      if (p_verify) {
        return -1;
      }
    }
  } else if (p_signed_data.tbsData().payload().extDataHash().is_present()) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Unsupported extDataHash, discard it");
    if (p_verify) {
      return -1;
    }
  } else { // Shall not be reached
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Unsupported SignedDataPayload, discard it");
    return -1;
  }

  // Encode the ToBeSignedData
  etsi_ts103097_tobesigned_data_codec tbs_data_codec;
  OCTETSTRING                         os;
  tbs_data_codec.encode(p_signed_data.tbsData(), os);
  if (os.lengthof() == 0) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Failed to encode ToBeSignedData");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: encoded tbs_data=", os);
  // Calculate the hash according to the hashId
  OCTETSTRING hashed_data;
  if (p_signed_data.hashId() == Ieee1609Dot2BaseTypes::HashAlgorithm::sha256) {
    hash_sha256(os, hashed_data);
  } else {
    hash_sha384(os, hashed_data);
  }
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: hashed_data=", hashed_data);
  // Retrieve certificate identifier
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: signer=", p_signed_data.signer());
  std::string certificate_id;
  int         result = -1;
  if (p_signed_data.signer().ischosen(Ieee1609Dot2::SignerIdentifier::ALT_digest)) {
    // Retrieve the certificate identifier from digest
    loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: Retrieve the certificate identifier from digest");
    result = _security_db.get()->get_certificate_id(p_signed_data.signer().digest(), certificate_id);
    if (result == -1) {
      // Check in the cache
      if (_security_cache.get()->get_certificate_id(p_signed_data.signer().digest(), certificate_id) == -1) {
        // Unknown certificate, request it
        const OCTETSTRING &os = p_signed_data.signer().digest();
        _unknown_certificate  = OCTETSTRING(3, static_cast<const unsigned char *>(os) + os.lengthof() - 3);
        loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Unknown certificate, request it");
        loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: HashedId3: ", _unknown_certificate);
      }
      // Reset certificate timer
      loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: Set Certificate re-transmission flag and reset timer");
      _last_generation_time = 0;

      return -1;
    }
  } else if (p_signed_data.signer().ischosen(Ieee1609Dot2::SignerIdentifier::ALT_certificate) && (p_signed_data.signer().certificate().size_of() != 0)) {
    // Extract the certificates
    std::vector<std::string> certificate_ids;
    for (int i = 0; i < p_signed_data.signer().certificate().size_of(); i++) {
      Ieee1609Dot2::CertificateBase cert = p_signed_data.signer().certificate()[i];
      // Retrieve ssps
      OPTIONAL<Ieee1609Dot2BaseTypes::SequenceOfPsidSsp> &v = cert.toBeSigned().appPermissions();
      if (v.is_present()) {
        Ieee1609Dot2BaseTypes::SequenceOfPsidSsp psid_ssps = static_cast<const Ieee1609Dot2BaseTypes::SequenceOfPsidSsp &>(*v.get_opt_value());
        loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: psid_ssps size: %d", psid_ssps.lengthof());
        for (int i = 0; i < psid_ssps.lengthof(); i++) {
          const Ieee1609Dot2BaseTypes::PsidSsp &psid_ssp = psid_ssps[i];
          loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: Processing psid_ssp ", psid_ssp);
          const OPTIONAL<Ieee1609Dot2BaseTypes::ServiceSpecificPermissions> &s = psid_ssp.ssp();
          if (s.is_present()) {
            const Ieee1609Dot2BaseTypes::ServiceSpecificPermissions &ssp = static_cast<const Ieee1609Dot2BaseTypes::ServiceSpecificPermissions>(s);
            loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: Processing ssp ", ssp);
            params::const_iterator it = p_params.find(std::to_string(psid_ssp.psid()));
            if (it == p_params.cend()) {
              OCTETSTRING os;
              if (ssp.ischosen(Ieee1609Dot2BaseTypes::ServiceSpecificPermissions::ALT_opaque)) {
                os = ssp.opaque();
              } else {
                os = ssp.bitmapSsp();
              }
              p_params[params::ssp] = std::string(static_cast<const char *>(oct2str(os)));
            }
          }
        } // End of 'for' statement
      }

      std::string certificate_id;
      if (extract_and_store_certificate(cert, certificate_id) != 0) {
        loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Failed to store certificate");
        if (p_verify) {
          return -1;
        }
      }
      loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: certificate_id: '%s'", certificate_id.c_str());
      certificate_ids.push_back(certificate_id);
      loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: certificate_ids size: %d", certificate_ids.size());
    } // End of 'for' statement
    certificate_id = certificate_ids[0];
    loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: After extract_and_store_certificate, certificate_id: '%s'",
                                certificate_id.c_str());
  } else {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Unsupported SignerIdentifier");
    return -1;
  }
  loggers::get_instance().log("security_services::process_ieee_1609_dot2_signed_data: certificate id = '%s'", certificate_id.c_str());
  // Verify the signature of the ToBeSignedData
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_signed_data: signature=", p_signed_data.signature__());
  result = -1;
  if (p_signed_data.signature__().ischosen(Ieee1609Dot2BaseTypes::Signature::ALT_ecdsaNistP256Signature)) {
    result = verify_sign_ecdsa_nistp256(hashed_data, p_signed_data.signature__(), certificate_id, p_params);
  } else if (p_signed_data.signature__().ischosen(Ieee1609Dot2BaseTypes::Signature::ALT_ecdsaBrainpoolP256r1Signature)) {
    result = verify_sign_ecdsa_brainpoolp256r1(hashed_data, p_signed_data.signature__(), certificate_id, p_params);
  } else if (p_signed_data.signature__().ischosen(Ieee1609Dot2BaseTypes::Signature::ALT_ecdsaBrainpoolP384r1Signature)) {
    result = verify_sign_ecdsa_brainpoolp384r1(hashed_data, p_signed_data.signature__(), certificate_id, p_params);
  } else {
    // TODO
    loggers::get_instance().error("security_services::process_ieee_1609_dot2_signed_data: TODO");
  }
  if (result != 0) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_signed_data: Failed to verify signature");
    return -1;
  }

  loggers::get_instance().log_msg("<<< security_services::process_ieee_1609_dot2_signed_data: ", p_unsecured_payload);
  return 0;
} // End of method process_ieee_1609_dot2_signed_data

int security_services::process_ieee_1609_dot2_encrypted_data(const Ieee1609Dot2::EncryptedData &p_encrypted_data, const bool p_verify,
                                                             OCTETSTRING &p_unsecured_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::process_ieee_1609_dot2_encrypted_data: ", p_encrypted_data);

  // 1. Retrieve the RecipientId
  const Ieee1609Dot2::RecipientInfo &                 r            = p_encrypted_data.recipients()[0]; // TODO Add multiple support of recipients
  const Ieee1609Dot2BaseTypes::EciesP256EncryptedKey *ecies        = nullptr;                          // TODO Use smart pointer
  const OCTETSTRING *                                 recipient_id = nullptr;                          // TODO Use smart pointer
  if (r.ischosen(Ieee1609Dot2::RecipientInfo::ALT_certRecipInfo)) {
    recipient_id = &r.certRecipInfo().recipientId();
    if (r.certRecipInfo().encKey().ischosen(Ieee1609Dot2::EncryptedDataEncryptionKey::ALT_eciesNistP256)) {
      ecies = &r.certRecipInfo().encKey().eciesNistP256();
    } else if (r.certRecipInfo().encKey().ischosen(Ieee1609Dot2::EncryptedDataEncryptionKey::ALT_eciesBrainpoolP256r1)) {
      ecies = &r.certRecipInfo().encKey().eciesBrainpoolP256r1();
    } else {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Unsupported encryption algorithm");
      return -1;
    }
  } else if (r.ischosen(Ieee1609Dot2::RecipientInfo::ALT_signedDataRecipInfo)) {
    recipient_id = &r.signedDataRecipInfo().recipientId();
    if (r.signedDataRecipInfo().encKey().ischosen(Ieee1609Dot2::EncryptedDataEncryptionKey::ALT_eciesNistP256)) {
      ecies = &r.signedDataRecipInfo().encKey().eciesNistP256();
    } else if (r.signedDataRecipInfo().encKey().ischosen(Ieee1609Dot2::EncryptedDataEncryptionKey::ALT_eciesBrainpoolP256r1)) {
      ecies = &r.signedDataRecipInfo().encKey().eciesBrainpoolP256r1();
    } else {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Unsupported encryption algorithm");
      return -1;
    }
  } else {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Unsupported RecipientInfo variant");
    return -1;
  }
  if (!p_encrypted_data.ciphertext().ischosen(Ieee1609Dot2::SymmetricCiphertext::ALT_aes128ccm)) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Unsupported AES 128 algorithm");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: RecipientId= ", *recipient_id);

  // 2. Retrieve the certificate if present
  std::string certificate_id;
  OCTETSTRING p_enc_key; // The private encryption key
  if (_security_db.get()->get_certificate_id(*recipient_id, certificate_id) == -1) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Unknown certificate");
    // Check if RecipientId is the hashed_id8 of the symetric AES keys
    loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: AES Symmetric Keys= ", ecies->c());
    OCTETSTRING hashed_data;
    hash_sha256(ecies->c(), hashed_data);
    loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: Hash (AES Symmetric Keys)= ", hashed_data);
    if (substr(hashed_data, hashed_data.lengthof() - 8, 8) != *recipient_id) {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: RecipientId does not match HashedId8 of the symmetric key");
      return -1;
    } else {
      if (_security_db.get()->get_private_enc_key(p_params[params::certificate], p_enc_key) == -1) {
        loggers::get_instance().warning(
          "security_services::process_ieee_1609_dot2_encrypted_data: Failed to retrieve private encryption key for certificate %s",
          p_params[params::certificate].c_str());
        return -1;
      }
    }
  } else {
    if (_security_db.get()->get_private_enc_key(certificate_id, p_enc_key) == -1) {
      loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Failed to retrieve private encryption key");
      return -1;
    }
  }

  // 3. Generate the shared secret value based on recipient's public ephemeral keys will be required
  int          result;
  security_ecc ec(ec_elliptic_curves::nist_p_256, p_enc_key);
  if (ecies->v().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
    result = ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ecies->v().uncompressedP256().x(), ecies->v().uncompressedP256().y(),
                                                  ecies->c(), p_encrypted_data.ciphertext().aes128ccm().nonce(), ecies->t(), OCTETSTRING(0, nullptr));
  } else if (ecies->v().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
    security_ecc ec_comp(ec_elliptic_curves::nist_p_256, ecies->v().compressed__y__0(), ecc_compressed_mode::compressed_y_0);
    result = ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), ecies->c(),
                                                  p_encrypted_data.ciphertext().aes128ccm().nonce(), ecies->t(), OCTETSTRING(0, nullptr));
  } else if (ecies->v().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
    security_ecc ec_comp(ec_elliptic_curves::nist_p_256, ecies->v().compressed__y__1(), ecc_compressed_mode::compressed_y_1);
    result = ec.generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, ec_comp.public_key_x(), ec_comp.public_key_y(), ecies->c(),
                                                  p_encrypted_data.ciphertext().aes128ccm().nonce(), ecies->t(), OCTETSTRING(0, nullptr));
  } else {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Failed to decode Decrypt Ieee1609Dot2Data-Encrypted");
    return -1;
  }
  if (result == -1) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Failed to generate shared secret");
    return -1;
  }
  // 4. Decrypt the message
  OCTETSTRING enc_message(p_encrypted_data.ciphertext().aes128ccm().ccmCiphertext().lengthof() - ec.tag().lengthof(),
                          static_cast<const unsigned char *>(p_encrypted_data.ciphertext().aes128ccm().ccmCiphertext()));
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: enc_message: ", enc_message);
  OCTETSTRING tag(ec.tag().lengthof(), enc_message.lengthof() + static_cast<const unsigned char *>(p_encrypted_data.ciphertext().aes128ccm().ccmCiphertext()));
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: tag: ", tag);
  if (ec.decrypt(tag, enc_message, p_unsecured_payload) == -1) {
    loggers::get_instance().warning("security_services::process_ieee_1609_dot2_encrypted_data: Failed to generate shared secret");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::process_ieee_1609_dot2_encrypted_data: ", p_unsecured_payload);

  return 0;
} // End of method process_ieee_1609_dot2_encrypted_data

int security_services::secure_gn_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_secured_gn_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::secure_gn_payload: ", p_unsecured_gn_payload);
  p_params.log();

  OCTETSTRING signed_payload;
  if (sign_payload(p_unsecured_gn_payload, signed_payload, p_params) != 0) {
    p_secured_gn_payload = p_unsecured_gn_payload;
    loggers::get_instance().warning("security_services::secure_gn_payload: Failed to signed payload");
    return -1;
  }

  if (_params[params::encrypted_mode].compare("1") == 0) {
    if (encrypt_gn_payload(signed_payload, p_secured_gn_payload, p_params) != 0) {
      p_secured_gn_payload = signed_payload;
      loggers::get_instance().warning("security_services::secure_gn_payload: Failed to encrypt payload");
      return -1;
    }
  } else { // No encryption required
    loggers::get_instance().log("security_services::secure_gn_payload: Encryption mode not set");
    p_secured_gn_payload = signed_payload;
  }

  return 0;
}

int security_services::sign_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_signed_gn_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_payload: ", p_unsecured_gn_payload);

  // Set unsecured data
  Ieee1609Dot2::Ieee1609Dot2Content unsecured_data_content;
  unsecured_data_content.unsecuredData() = p_unsecured_gn_payload;
  Ieee1609Dot2::Ieee1609Dot2Data unsecured_data(ProtocolVersion, unsecured_data_content);
  // Set hash algorithm
  Ieee1609Dot2BaseTypes::HashAlgorithm hashId(Ieee1609Dot2BaseTypes::HashAlgorithm::sha256);
  if (p_params[params::hash].compare("SHA-384") == 0) {
    hashId = Ieee1609Dot2BaseTypes::HashAlgorithm::sha384;
  }
  // Set SignedDataPayload
  Ieee1609Dot2::SignedDataPayload payload;
  payload.data() = unsecured_data;
  payload.extDataHash().set_to_omit();
  Ieee1609Dot2::HeaderInfo header_info;
  // Set secured field according to the payload!
  header_info.psid() = converter::get_instance().string_to_int(p_params[params::its_aid]);
  header_info.expiryTime().set_to_omit();
  // Force certificate?
  boolean force_certificate = false;
  params::const_iterator it = p_params.find(params::force_certificate);
  if (it != p_params.cend()) {
    force_certificate = true;
  }
  if (p_params[params::its_aid].compare("37") == 0) { // Only used by AtsRSUSimulator to add location header
    // Mandatory for DENM payload
    OPTIONAL<Ieee1609Dot2BaseTypes::ThreeDLocation> location(Ieee1609Dot2BaseTypes::ThreeDLocation(_latitude, _longitude, _elevation));
    header_info.generationLocation() = location;
    loggers::get_instance().log_msg("security_services::sign_payload: generationLocation: ", header_info.generationLocation());
  } else {
    header_info.generationLocation().set_to_omit();
  }
  header_info.p2pcdLearningRequest().set_to_omit();
  header_info.missingCrlIdentifier().set_to_omit();
  if (_params[params::encrypted_mode].compare("1") == 0) {
    // TODO Set the encrytion key. Not supported yet, need to clarify mechanism, see IEEE Std 1609.2-20XX Clause 6.3.9 HeaderInfo
    header_info.encryptionKey().set_to_omit();
  } else {
    header_info.encryptionKey().set_to_omit();
  }
  it = p_params.find(params::payload_type);
  if (it != p_params.cend()) {
    loggers::get_instance().log("security_services::sign_payload: Payload type: %s", it->second.c_str());
    if (it->second.compare("1") == 0) { // DENM
      OPTIONAL<Ieee1609Dot2BaseTypes::ThreeDLocation> location(Ieee1609Dot2BaseTypes::ThreeDLocation(_latitude, _longitude, _elevation));
      loggers::get_instance().log_msg("security_services::sign_payload: generationLocation: ", location);
      header_info.generationLocation() = location;
      loggers::get_instance().log_msg("security_services::sign_payload: generationLocation: ", header_info.generationLocation());
    } else if (it->second.compare("2") == 0) { // CAM
      // Noting to do
    } else {
      // Noting to do
    }
  } else { // Process it as a GeoNetworking payload
    loggers::get_instance().log("security_services::sign_payload: Payload type not set");
    // Noting to do
  }
  unsigned long long us = base_time::get_instance().get_its_current_time_us();
  loggers::get_instance().log("security_services::sign_payload: HeaderInfo timestamp: %ld", us);
  INTEGER i;
  i.set_long_long_val(us);
  header_info.generationTime() = OPTIONAL<INTEGER>(i);
  loggers::get_instance().log("security_services::sign_payload: Final HeaderInfo timestamp: %ld", us);
  // Check if a certificate shall be requested
  if (_unknown_certificate.lengthof() == 3) { // HashedId3 of a requested certificate
    Ieee1609Dot2BaseTypes::SequenceOfHashedId3 s;
    s[0]                             = _unknown_certificate;
    header_info.inlineP2pcdRequest() = OPTIONAL<Ieee1609Dot2BaseTypes::SequenceOfHashedId3>(s);
    _unknown_certificate             = OCTETSTRING(0, nullptr);
  } else {
    header_info.inlineP2pcdRequest().set_to_omit();
  }
  // Check if a certificate shall be provided
  if (_requested_certificate.size() != 0) {
    Ieee1609Dot2::CertificateBase           c;
    const Ieee1609Dot2BaseTypes::HashedId3 &hashed_id3 = _requested_certificate[0];
    _security_db->get_certificate(hashed_id3, c);
    header_info.requestedCertificate() = OPTIONAL<Ieee1609Dot2::CertificateBase>(c);
  } else {
    header_info.requestedCertificate().set_to_omit();
  }
  header_info.pduFunctionalType().set_to_omit();
  header_info.contributedExtensions().set_to_omit();

  Ieee1609Dot2::ToBeSignedData tbs_data;
  tbs_data.payload()    = payload;
  tbs_data.headerInfo() = header_info;
  loggers::get_instance().log_msg("security_services::sign_payload: tbs_data=", tbs_data);
  // Sign the ToBeSignedData data structure
  Ieee1609Dot2BaseTypes::Signature signature;
  if (sign_tbs_data(tbs_data, hashId, signature, p_params) != 0) {
    loggers::get_instance().warning("security_services::sign_payload: Failed to secure payload");
    return -1;
  }
  Ieee1609Dot2::SignerIdentifier signer;
  loggers::get_instance().log("security_services::sign_payload: us = %ld - _last_generation_time = %ld - us - _last_generation_time = %ld", us,
                              _last_generation_time, us - _last_generation_time);
  std::string certificate_id = p_params[params::certificate];
  loggers::get_instance().log("security_services::sign_payload: certificate_id = %s", certificate_id.c_str());
  if (((unsigned int)(us - _last_generation_time) >= 1000000 * 0.95) || force_certificate) { // Need to add certificate every 1s
    loggers::get_instance().log("security_services::sign_payload: Need to add certificate");
    Ieee1609Dot2::CertificateBase cert;
    if (_security_db->get_certificate(certificate_id, cert) != 0) {
      loggers::get_instance().warning("security_services:sign_payload: Failed to secure payload");
      return -1;
    }
    loggers::get_instance().log_msg("security_services::sign_payload: cert= ", cert);
    Ieee1609Dot2::SequenceOfCertificate sequenceOfCertificate;
    sequenceOfCertificate[0] = cert;
    signer.certificate()     = sequenceOfCertificate;
    // Reset send certificate timer
    _last_generation_time = us;
    loggers::get_instance().log("security_services::sign_payload: Reset send certificate timer, signer= ", signer);
  } else {
    loggers::get_instance().log("security_services::sign_payload: Add digest");
    OCTETSTRING digest;
    if (_security_db->get_hashed_id(certificate_id, digest) != 0) {
      loggers::get_instance().warning("security_services::sign_payload: Failed to secure payload");
      return -1;
    }
    signer.digest() = digest;
  }
  Ieee1609Dot2::SignedData signed_data(hashId, tbs_data, signer, signature);
  loggers::get_instance().log_msg("security_services::sign_payload: signed_data=", signed_data);
  Ieee1609Dot2::Ieee1609Dot2Content ieee_dot2_content;
  ieee_dot2_content.signedData() = signed_data;
  Ieee1609Dot2::Ieee1609Dot2Data ieee_1609dot2_data(security_services::ProtocolVersion, ieee_dot2_content);
  loggers::get_instance().log_msg("security_services::sign_payload: ieee_1609dot2_data=", ieee_1609dot2_data);
  etsi_ts103097_data_codec codec;
  codec.encode(ieee_1609dot2_data, p_signed_gn_payload);
  if (!p_signed_gn_payload.is_bound()) {
    loggers::get_instance().warning("security_services::sign_payload: Failed to encode Ieee1609Dot2Data");
    return -1;
  }

  return 0;
}

int security_services::encrypt_gn_payload(const OCTETSTRING &p_unsecured_gn_payload, OCTETSTRING &p_enc_gn_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::encrypt_gn_payload: ", p_unsecured_gn_payload);

  // Sanity checks
  if (_ec_keys_enc.get() == nullptr) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Encryption not initialised");
    return -1;
  }
  params::const_iterator it = p_params.find(params::peer_certificate);
  if (it == p_params.cend()) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Encryption impossible without a peer_certificte indication in parameters");
    return -1;
  }
  std::string certificate_id = it->second;
  loggers::get_instance().log("security_services::encrypt_gn_payload: Peer CertificateId=%s", certificate_id.c_str());

  // 1. Retrieve recipient's public keys
  OCTETSTRING r_public_key_x;
  OCTETSTRING r_public_key_y;
  if (_security_db.get()->get_public_enc_keys(certificate_id, r_public_key_x, r_public_key_y) == -1) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Failed to retrieve recipient's public keys");
    // TODO Setup request certificate mechanism
    return -1;
  }
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: r_public_key_x=", r_public_key_x);
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: r_public_key_y=", r_public_key_y);

  // 2. Generate new Private/Public ephemeral keys
  if (_ec_keys_enc.get()->generate() == -1) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Failed to generate ephemeral keys");
    return -1;
  }

  // 3. Generate and derive shared secret
  if (_ec_keys_enc.get()->generate_and_derive_ephemeral_key(encryption_algotithm::aes_128_ccm, r_public_key_x, r_public_key_y, OCTETSTRING(0, nullptr)) == -1) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Failed to generate and derive secret key");
    return -1;
  }

  // 4. Buil curve data structure in canonical form
  Ieee1609Dot2BaseTypes::EccP256CurvePoint eccP256CurvePoint;
  if (_ec_keys_enc.get()->public_key_compressed_mode() == ecc_compressed_mode::compressed_y_0) {
    eccP256CurvePoint.compressed__y__0() = _ec_keys_enc.get()->public_key_compressed();
  } else if (_ec_keys_enc.get()->public_key_compressed_mode() == ecc_compressed_mode::compressed_y_1) {
    eccP256CurvePoint.compressed__y__1() = _ec_keys_enc.get()->public_key_compressed();
  } else {
    eccP256CurvePoint.uncompressedP256().x() = _ec_keys_enc.get()->public_key_x();
    eccP256CurvePoint.uncompressedP256().y() = _ec_keys_enc.get()->public_key_y();
  }
  Ieee1609Dot2BaseTypes::EciesP256EncryptedKey ecies_key(eccP256CurvePoint, _ec_keys_enc.get()->encrypted_symmetric_key(), _ec_keys_enc.get()->tag());
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: ecies_key=", ecies_key);

  // 5. AES-128 encryption of the data
  OCTETSTRING enc_message;
  if (_ec_keys_enc.get()->encrypt(encryption_algotithm::aes_128_ccm, _ec_keys_enc.get()->symmetric_encryption_key(), _ec_keys_enc.get()->nonce(),
                                  p_unsecured_gn_payload, enc_message) == -1) {
    loggers::get_instance().warning("fx__encryptWithEciesNistp256WithSha256: Failed to encrypt message");
    return -1;
  }
  OCTETSTRING                    nonce = _ec_keys_enc.get()->nonce();
  OCTETSTRING                    tag   = _ec_keys_enc.get()->tag();
  Ieee1609Dot2::AesCcmCiphertext aes_128_ccm(nonce, enc_message + tag); // Add tag at the end of the ciphered text
  // 6. Build SymmetricCiphertext
  Ieee1609Dot2::SymmetricCiphertext cipher_text;
  cipher_text.aes128ccm() = aes_128_ccm;
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: aes_128_ccm=", cipher_text);

  // 7. Build the recipient_id
  OCTETSTRING recipient_id;
  _security_db.get()->get_hashed_id(certificate_id, recipient_id); // SHA-256 of the certificate which contain the recipient's public keys

  // 8. Build the encryption data
  Ieee1609Dot2::EncryptedDataEncryptionKey enc_data_key;
  if (_params[params::cypher].compare("NISTP-256") == 0) {
    enc_data_key.eciesNistP256() = ecies_key;
  } else if (_params[params::cypher].compare("BP-256") == 0) {
    enc_data_key.eciesBrainpoolP256r1() = ecies_key;
  }
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: enc_data_key=", enc_data_key);

  // 9. Finalise the encryption
  Ieee1609Dot2::PKRecipientInfo cert_recipient_info(recipient_id, enc_data_key);
  Ieee1609Dot2::RecipientInfo   recipient_info;
  recipient_info.certRecipInfo() = cert_recipient_info;
  Ieee1609Dot2::SequenceOfRecipientInfo recipients;
  recipients[0] = recipient_info;
  Ieee1609Dot2::EncryptedData encrypted_data(recipients, cipher_text);
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: encrypted_data=", encrypted_data);

  // 10. Encode it
  Ieee1609Dot2::Ieee1609Dot2Content ieee_dot2_content;
  ieee_dot2_content.encryptedData() = encrypted_data;
  Ieee1609Dot2::Ieee1609Dot2Data ieee_1609dot2_data(security_services::ProtocolVersion, ieee_dot2_content);
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: ieee_1609dot2_data=", ieee_1609dot2_data);
  etsi_ts103097_data_codec codec;
  codec.encode(ieee_1609dot2_data, p_enc_gn_payload);
  if (!p_enc_gn_payload.is_bound()) {
    loggers::get_instance().warning("security_services::encrypt_gn_payload: Failed to encode Ieee1609Dot2Data");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::encrypt_gn_payload: Encoded ieee_1609dot2_data=", p_enc_gn_payload);

  return 0;
}

int security_services::decrypt_gn_payload(const OCTETSTRING &p_enc_gn_payload, OCTETSTRING &p_unsecured_gn_payload, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::decrypt_gn_payload: ", p_enc_gn_payload);

  // Sanity checks
  if (_ec_keys_enc.get() == nullptr) {
    loggers::get_instance().warning("security_services::decrypt_gn_payload: Encryption not initialised");
    return -1;
  }

  // 1. Decode the IEEE 1609dot2Data-Encrypted
  Ieee1609Dot2::Ieee1609Dot2Data ieee_1609dot2_data;
  etsi_ts103097_data_codec       codec;
  codec.decode(p_enc_gn_payload, ieee_1609dot2_data);
  if (!ieee_1609dot2_data.is_bound()) {
    loggers::get_instance().warning("security_services::decrypt_gn_payload: Failed to decode Ieee1609Dot2Data-Encrypted");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::decrypt_gn_payload: Ieee1609Dot2Data-Encrypted=", ieee_1609dot2_data);

  if (!ieee_1609dot2_data.content().ischosen(Ieee1609Dot2::Ieee1609Dot2Content::ALT_encryptedData)) {
    loggers::get_instance().warning("security_services::decrypt_gn_payload: Failed to decode Decrypt Ieee1609Dot2Data-Encrypted");
    return -1;
  }
  if (process_ieee_1609_dot2_encrypted_data(ieee_1609dot2_data.content().encryptedData(), true, p_unsecured_gn_payload, p_params) == -1) {
    loggers::get_instance().warning("security_services::decrypt_gn_payload: Failed to decode Decrypt Ieee1609Dot2Data-Encrypted");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::decrypt_gn_payload: Encoded ieee_1609dot2_data=", p_unsecured_gn_payload);

  return 0;
}

/*int security_services::sign_tbs_data(const Ieee1609Dot2::ToBeSignedData &p_tbs_data, const Ieee1609Dot2BaseTypes::HashAlgorithm &p_hashAlgorithm,
                                     const OCTETSTRING &p_private_key, Ieee1609Dot2BaseTypes::Signature &p_signature,
                                     params &p_params) { // TODO Refine function
  loggers::get_instance().log_msg(">>> security_services::sign_tbs_data: ", p_tbs_data);

  // Encode the ToBeSignedData
  etsi_ts103097_tobesigned_data_codec tbs_data_codec;
  OCTETSTRING os;
  tbs_data_codec.encode(p_tbs_data, os);
  if (os.lengthof() == 0) {
    loggers::get_instance().warning("security_services::sign_tbs_data: Failed to encode ToBeSignedData");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_tbs_data: encoded tbs_data=", os);
  // Hash ToBeSignedData
  OCTETSTRING hashed_data;
  OCTETSTRING hash_issuer;
  if (p_hashAlgorithm == Ieee1609Dot2BaseTypes::HashAlgorithm::sha256) {
    sha256 hash;
    hash.generate(os, hashed_data);
    hash_issuer = hash.get_sha256_empty_string();
  } else {
    sha384 hash;
    hash.generate(os, hashed_data);
    hash_issuer = hash.get_sha384_empty_string();
  }
  loggers::get_instance().log_msg("security_services::sign_tbs_data: encoded hashed_data=", hashed_data);
  loggers::get_instance().log_msg("security_services::sign_tbs_data: encoded hashed_issuer=", hash_issuer);
  // Sign ToBeSignedData
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::signature = '%s'",
                              p_params[params::signature].c_str()); // TODO this parameter is useless, use content of the certificate
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::certificate = '%s'", p_params[params::certificate].c_str());
  if (p_params[params::signature].compare("NISTP-256") == 0) {
    // Hash ( Hash (Data input) || Hash ("") )
    OCTETSTRING os = hashed_data + hash_issuer; // Hash (Data input) || Hash (Signer identifier input)
    loggers::get_instance().log_msg("security_services::sign_tbs_data: hash: ", os);
    OCTETSTRING hashed_data;
    hash_sha256(os, hashed_data); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
    security_ecc k(ec_elliptic_curves::nist_p_256, p_private_key);
    OCTETSTRING  r_sig;
    OCTETSTRING  s_sig;
    if (k.sign(hashed_data, r_sig, s_sig) != 0) {
      loggers::get_instance().warning("security_services::sign_tbs_data: Failed to sign payload");
      return -1;
    }
    Ieee1609Dot2BaseTypes::EccP256CurvePoint ep;
    ep.x__only()                         = r_sig;
    p_signature.ecdsaNistP256Signature() = Ieee1609Dot2BaseTypes::EcdsaP256Signature(ep, s_sig);
    loggers::get_instance().log_msg("security_services::sign_tbs_data: signature=", p_signature);
  } // TODO To be done

  return 0;
}*/

int security_services::sign_tbs_data(const Ieee1609Dot2::ToBeSignedData &p_tbs_data, const Ieee1609Dot2BaseTypes::HashAlgorithm &p_hashAlgorithm,
                                     Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_tbs_data: ", p_tbs_data);

  // Get certificate
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::certificate = '%s'", p_params[params::certificate].c_str());
  Ieee1609Dot2::CertificateBase decoded_certificate;
  // Set signature type
  _security_db.get()->get_certificate(p_params[params::certificate], decoded_certificate);
  std::string sig("NISTP-256");
  if (decoded_certificate.toBeSigned().verifyKeyIndicator().ischosen(Ieee1609Dot2::VerificationKeyIndicator::ALT_verificationKey)) {
    if (decoded_certificate.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaNistP384)) {
      sig.assign("NISTP-384");
    } else if (decoded_certificate.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaBrainpoolP256r1)) {
      sig.assign("BP-256");
    } else if (decoded_certificate.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaBrainpoolP384r1)) {
      sig.assign("BP-384");
    }
  }
  loggers::get_instance().log("security_services::sign_tbs_data: sig = '%s'", sig.c_str());
  params::const_iterator it = p_params.find(params::signature);
  if (it == p_params.cend()) {
    p_params.insert(std::pair<std::string, std::string>(params::signature, sig));
  } else {
    p_params[params::signature] = sig;
  }
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::signature = '%s'", p_params[params::signature].c_str());

  // Encode the ToBeSignedData
  etsi_ts103097_tobesigned_data_codec tbs_data_codec;
  OCTETSTRING                         os;
  tbs_data_codec.encode(p_tbs_data, os);
  if (os.lengthof() == 0) {
    loggers::get_instance().warning("security_services::sign_tbs_data: Failed to encode ToBeSignedData");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_tbs_data: encoded tbs_data=", os);
  // Hash ToBeSignedData
  OCTETSTRING hashed_data;
  if (p_hashAlgorithm == Ieee1609Dot2BaseTypes::HashAlgorithm::sha256) {
    hash_sha256(os, hashed_data);
  } else {
    hash_sha384(os, hashed_data);
  }
  loggers::get_instance().log_msg("security_services::sign_tbs_data: encoded hashed_data=", hashed_data);
  // Sign ToBeSignedData
  int result = -1;
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::signature = '%s'",
                              p_params[params::signature].c_str()); // TODO this parameter is useless, use content of the certificate
  loggers::get_instance().log("security_services::sign_tbs_data: encoded params::certificate = '%s'", p_params[params::certificate].c_str());
  if (p_params[params::signature].compare("NISTP-256") == 0) {
    result = sign_ecdsa_nistp256(hashed_data, p_signature, p_params);
  } else if (p_params[params::signature].compare("NISTP-384") == 0) {
    result = sign_ecdsa_nistp384(hashed_data, p_signature, p_params);
  } else if (p_params[params::signature].compare("BP-256") == 0) {
    result = sign_ecdsa_brainpoolp256r1(hashed_data, p_signature, p_params);
  } else if (p_params[params::signature].compare("BP-384") == 0) {
    result = sign_ecdsa_brainpoolp384r1(hashed_data, p_signature, p_params);
  } else {
    loggers::get_instance().error("security_services::sign_tbs_data: Unsupported signature algorithm");
    result = -1;
  }
  if (result != 0) {
    loggers::get_instance().warning("security_services::sign_tbs_data: Failed to sign payload");
    return -1;
  }

  return 0;
}

int security_services::hash_sha256(const OCTETSTRING &p_data, OCTETSTRING &p_hash_data) {
  loggers::get_instance().log_msg(">>> security_services::hash_sha256: ", p_data);

  sha256 hash;
  return hash.generate(p_data, p_hash_data);
}

int security_services::hash_sha384(const OCTETSTRING &p_data, OCTETSTRING &p_hash_data) {
  loggers::get_instance().log_msg(">>> security_services::hash_sha384: ", p_data);

  sha384 hash;
  return hash.generate(p_data, p_hash_data);
}

int security_services::sign_ecdsa_nistp256(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_ecdsa_nistp256: ", p_hash);

  std::string certificate_id = p_params[params::certificate];
  loggers::get_instance().log("security_services::sign_ecdsa_nistp256: encoded certificate_id = '%s'", certificate_id.c_str());
  OCTETSTRING pkey;
  if (_security_db->get_private_key(certificate_id, pkey) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp256: Failed to get private key");
    return -1;
  }
  // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING hash_cert;
  if (_security_db->get_hash(certificate_id, hash_cert) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp256: Failed to get whole hash certificate");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp256: hash_issuer: ", hash_cert);
  OCTETSTRING os = p_hash + hash_cert; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp256: hash: ", os);
  OCTETSTRING hashed_data;
  hash_sha256(os, hashed_data); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  security_ecc k(ec_elliptic_curves::nist_p_256, pkey);
  OCTETSTRING  r_sig;
  OCTETSTRING  s_sig;
  if (k.sign(hashed_data, r_sig, s_sig) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp256: Failed to sign payload");
    return -1;
  }
  Ieee1609Dot2BaseTypes::EccP256CurvePoint ep;
  ep.x__only()                         = r_sig;
  p_signature.ecdsaNistP256Signature() = Ieee1609Dot2BaseTypes::EcdsaP256Signature(ep, s_sig);
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp256: signature=", p_signature);

  return 0;
}

int security_services::sign_ecdsa_nistp384(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_ecdsa_nistp384: ", p_hash);

  std::string certificate_id = p_params[params::certificate];
  loggers::get_instance().log("security_services::sign_ecdsa_nistp384: encoded certificate_id = '%s'", certificate_id.c_str());
  OCTETSTRING pkey;
  if (_security_db->get_private_key(certificate_id, pkey) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp384: Failed to get private key");
    return -1;
  }
  // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING hash_cert;
  if (_security_db->get_hash(certificate_id, hash_cert) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp384: Failed to get whole hash certificate");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp384: hash_issuer: ", hash_cert);
  OCTETSTRING os = p_hash + hash_cert; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp384: hash: ", os);
  OCTETSTRING hashed_data;
  hash_sha384(os, hashed_data); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  security_ecc k(ec_elliptic_curves::nist_p_384, pkey);
  OCTETSTRING  r_sig;
  OCTETSTRING  s_sig;
  if (k.sign(hashed_data, r_sig, s_sig) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_nistp384: Failed to sign payload");
    return -1;
  }
  Ieee1609Dot2BaseTypes::EccP384CurvePoint ep;
  ep.x__only()                         = r_sig;
  p_signature.ecdsaNistP384Signature() = Ieee1609Dot2BaseTypes::EcdsaP384Signature(ep, s_sig);
  loggers::get_instance().log_msg("security_services::sign_ecdsa_nistp384: signature=", p_signature);

  return 0;
}

int security_services::verify_sign_ecdsa_nistp256(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature,
                                                  const std::string &p_certificate_id, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::verify_sign_ecdsa_nistp256:", p_hash);
  loggers::get_instance().log(">>> security_services::verify_sign_ecdsa_nistp256: %s", p_certificate_id.c_str());

  OCTETSTRING public_key_x;
  OCTETSTRING public_key_y;
  if (_security_db->get_public_keys(p_certificate_id, public_key_x, public_key_y) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp256 (%s): Failed to get public keys", p_certificate_id.c_str());
    return -1;
  }

  // Generate the hash to be verified: Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING issuer; // Hash (Signer identifier input)
  if (_security_db->get_hash(p_certificate_id, issuer) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp256 (%s): Failed to get hash of the issuer certificate",
                                    p_certificate_id.c_str());
    return -1;
  }
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp256: hash_issuer: ", issuer);
  OCTETSTRING hash_data = p_hash + issuer; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp256: hash: ", hash_data);
  OCTETSTRING hash_to_be_verified;
  hash_sha256(hash_data, hash_to_be_verified); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp256: hash_to_be_verified: ", hash_to_be_verified);

  // Build the signature
  OCTETSTRING signature;
  if (p_signature.ecdsaNistP256Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_x__only)) {
    signature = p_signature.ecdsaNistP256Signature().rSig().x__only() + p_signature.ecdsaNistP256Signature().sSig();
  } else if (p_signature.ecdsaNistP256Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
    signature = p_signature.ecdsaNistP256Signature().rSig().compressed__y__0() + p_signature.ecdsaNistP256Signature().sSig();
  } else if (p_signature.ecdsaNistP256Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
    signature = p_signature.ecdsaNistP256Signature().rSig().compressed__y__1() + p_signature.ecdsaNistP256Signature().sSig();
  } else if (p_signature.ecdsaNistP256Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
    signature = p_signature.ecdsaNistP256Signature().rSig().uncompressedP256().x() + p_signature.ecdsaNistP256Signature().rSig().uncompressedP256().y() +
                p_signature.ecdsaNistP256Signature().sSig();
  } else {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp256 (%s): Invalid curve point", p_certificate_id.c_str());
    return -1;
  }
  security_ecc k(ec_elliptic_curves::nist_p_256, public_key_x, public_key_y);
  if (k.sign_verif(hash_to_be_verified, signature) == 0) {
    return 0;
  }

  return -1;
}

int security_services::verify_sign_ecdsa_nistp384(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature,
                                                  const std::string &p_certificate_id, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::verify_sign_ecdsa_nistp384:", p_hash);
  loggers::get_instance().log(">>> security_services::verify_sign_ecdsa_nistp384: %s", p_certificate_id.c_str());

  OCTETSTRING public_key_x;
  OCTETSTRING public_key_y;
  if (_security_db->get_public_keys(p_certificate_id, public_key_x, public_key_y) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp384 (%s): Failed to get public keys", p_certificate_id.c_str());
    return -1;
  }

  // Generate the hash to be verified: Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING issuer; // Hash (Signer identifier input)
  if (_security_db->get_hash(p_certificate_id, issuer) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp384 (%s): Failed to get hash of the issuer certificate",
                                    p_certificate_id.c_str());
    return -1;
  }
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp384: hash_issuer: ", issuer);
  OCTETSTRING hash_data = p_hash + issuer; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp384: hash: ", hash_data);
  OCTETSTRING hash_to_be_verified;
  hash_sha384(hash_data, hash_to_be_verified); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_nistp384: hash_to_be_verified: ", hash_to_be_verified);

  // Build the signature
  OCTETSTRING signature;
  if (p_signature.ecdsaNistP384Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_x__only)) {
    signature = p_signature.ecdsaNistP384Signature().rSig().x__only() + p_signature.ecdsaNistP384Signature().sSig();
  } else if (p_signature.ecdsaNistP384Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__0)) {
    signature = p_signature.ecdsaNistP384Signature().rSig().compressed__y__0() + p_signature.ecdsaNistP384Signature().sSig();
  } else if (p_signature.ecdsaNistP384Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__1)) {
    signature = p_signature.ecdsaNistP384Signature().rSig().compressed__y__1() + p_signature.ecdsaNistP384Signature().sSig();
  } else if (p_signature.ecdsaNistP384Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_uncompressedP384)) {
    signature = p_signature.ecdsaNistP384Signature().rSig().uncompressedP384().x() + p_signature.ecdsaNistP384Signature().rSig().uncompressedP384().y() +
                p_signature.ecdsaNistP384Signature().sSig();
  } else {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_nistp384 (%s): Invalid curve point", p_certificate_id.c_str());
    return -1;
  }
  security_ecc k(ec_elliptic_curves::nist_p_384, public_key_x, public_key_y);
  if (k.sign_verif(hash_to_be_verified, signature) == 0) {
    return 0;
  }

  return -1;
}

int security_services::sign_ecdsa_brainpoolp256r1(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_ecdsa_brainpoolp256r1: ", p_hash);

  std::string certificate_id = p_params[params::certificate];
  loggers::get_instance().log("security_services::sign_ecdsa_brainpoolp256r1: encoded certificate_id = '%s'", certificate_id.c_str());
  OCTETSTRING pkey;
  if (_security_db->get_private_key(certificate_id, pkey) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp256r1: Failed to get private key");
    return -1;
  }
  // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING hash_cert;
  if (_security_db->get_hash(certificate_id, hash_cert) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp256r1: Failed to get whole hash certificate");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp256r1: hash_issuer: ", hash_cert);
  OCTETSTRING os = p_hash + hash_cert; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp256r1: hash: ", os);
  OCTETSTRING hashed_data;
  hash_sha256(os, hashed_data); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, pkey);
  OCTETSTRING  r_sig;
  OCTETSTRING  s_sig;
  if (k.sign(hashed_data, r_sig, s_sig) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp256r1: Failed to sign payload");
    return -1;
  }
  Ieee1609Dot2BaseTypes::EccP256CurvePoint ep;
  ep.x__only()                                = r_sig;
  p_signature.ecdsaBrainpoolP256r1Signature() = Ieee1609Dot2BaseTypes::EcdsaP256Signature(ep, s_sig);
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp256r1: signature=", p_signature);

  return 0;
}

int security_services::verify_sign_ecdsa_brainpoolp256r1(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature,
                                                         const std::string &p_certificate_id, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::verify_sign_ecdsa_brainpoolp256r1:", p_hash);
  loggers::get_instance().log(">>> security_services::verify_sign_ecdsa_brainpoolp256r1: %s", p_certificate_id.c_str());

  OCTETSTRING public_key_x;
  OCTETSTRING public_key_y;
  if (_security_db->get_public_keys(p_certificate_id, public_key_x, public_key_y) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp256r1 (%s): Failed to get public keys", p_certificate_id.c_str());
    return -1;
  }

  // Generate the hash to be verified: Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING issuer; // Hash (Signer identifier input)
  if (_security_db->get_hash(p_certificate_id, issuer) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp256r1 (%s): Failed to get hash of the issuer certificate",
                                    p_certificate_id.c_str());
    return -1;
  }
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp256r1: hash_issuer: ", issuer);
  OCTETSTRING hash_data = p_hash + issuer; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp256r1: hash: ", hash_data);
  OCTETSTRING hash_to_be_verified;
  hash_sha256(hash_data, hash_to_be_verified); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp256r1: hash_to_be_verified: ", hash_to_be_verified);

  // Build the signature
  OCTETSTRING signature;
  if (p_signature.ecdsaBrainpoolP256r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_x__only)) {
    signature = p_signature.ecdsaBrainpoolP256r1Signature().rSig().x__only() + p_signature.ecdsaBrainpoolP256r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP256r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
    signature = p_signature.ecdsaBrainpoolP256r1Signature().rSig().compressed__y__0() + p_signature.ecdsaBrainpoolP256r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP256r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
    signature = p_signature.ecdsaBrainpoolP256r1Signature().rSig().compressed__y__1() + p_signature.ecdsaBrainpoolP256r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP256r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
    signature = p_signature.ecdsaBrainpoolP256r1Signature().rSig().uncompressedP256().x() +
                p_signature.ecdsaBrainpoolP256r1Signature().rSig().uncompressedP256().y() + p_signature.ecdsaBrainpoolP256r1Signature().sSig();
  } else {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp256r1 (%s): Invalid curve point", p_certificate_id.c_str());
    return -1;
  }
  security_ecc k(ec_elliptic_curves::brainpool_p_256_r1, public_key_x, public_key_y);
  if (k.sign_verif(hash_to_be_verified, signature) == 0) {
    return 0;
  }

  return -1;
}

int security_services::sign_ecdsa_brainpoolp384r1(const OCTETSTRING &p_hash, Ieee1609Dot2BaseTypes::Signature &p_signature, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::sign_ecdsa_brainpoolp384r1: ", p_hash);

  std::string certificate_id = p_params[params::certificate];
  loggers::get_instance().log("security_services::sign_ecdsa_brainpoolp384r1: encoded certificate_id = '%s'", certificate_id.c_str());
  OCTETSTRING pkey;
  if (_security_db->get_private_key(certificate_id, pkey) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp384r1: Failed to get private key");
    return -1;
  }
  // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING hash_cert;
  if (_security_db->get_hash(certificate_id, hash_cert) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp384r1: Failed to get whole hash certificate");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp384r1: hash_issuer: ", hash_cert);
  OCTETSTRING os = p_hash + hash_cert; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp384r1: hash: ", os);
  OCTETSTRING hashed_data;
  hash_sha384(os, hashed_data); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, pkey);
  OCTETSTRING  r_sig;
  OCTETSTRING  s_sig;
  if (k.sign(hashed_data, r_sig, s_sig) != 0) {
    loggers::get_instance().warning("security_services::sign_ecdsa_brainpoolp384r1: Failed to sign payload");
    return -1;
  }
  Ieee1609Dot2BaseTypes::EccP384CurvePoint ep;
  ep.x__only()                                = r_sig;
  p_signature.ecdsaBrainpoolP384r1Signature() = Ieee1609Dot2BaseTypes::EcdsaP384Signature(ep, s_sig);
  loggers::get_instance().log_msg("security_services::sign_ecdsa_brainpoolp384r1: signature=", p_signature);

  return 0;
}

int security_services::verify_sign_ecdsa_brainpoolp384r1(const OCTETSTRING &p_hash, const Ieee1609Dot2BaseTypes::Signature &p_signature,
                                                         const std::string &p_certificate_id, params &p_params) {
  loggers::get_instance().log_msg(">>> security_services::verify_sign_ecdsa_brainpoolp384r1:", p_hash);
  loggers::get_instance().log(">>> security_services::verify_sign_ecdsa_brainpoolp384r1: %s", p_certificate_id.c_str());

  OCTETSTRING public_key_x;
  OCTETSTRING public_key_y;
  if (_security_db->get_public_keys(p_certificate_id, public_key_x, public_key_y) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp384r1 (%s): Failed to get public keys", p_certificate_id.c_str());
    return -1;
  }

  // Generate the hash to be verified: Hash ( Hash (Data input) || Hash (Signer identifier input) )
  OCTETSTRING issuer; // Hash (Signer identifier input)
  if (_security_db->get_hash(p_certificate_id, issuer) != 0) {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp384r1 (%s): Failed to get hash of the issuer certificate",
                                    p_certificate_id.c_str());
    return -1;
  }
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp384r1: hash_issuer: ", issuer);
  OCTETSTRING hash_data = p_hash + issuer; // Hash (Data input) || Hash (Signer identifier input)
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp384r1: hash: ", hash_data);
  OCTETSTRING hash_to_be_verified;
  hash_sha384(hash_data, hash_to_be_verified); // Hash ( Hash (Data input) || Hash (Signer identifier input) )
  loggers::get_instance().log_msg("security_services::verify_sign_ecdsa_brainpoolp384r1: hash_to_be_verified: ", hash_to_be_verified);

  // Build the signature
  OCTETSTRING signature;
  if (p_signature.ecdsaBrainpoolP384r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_x__only)) {
    signature = p_signature.ecdsaBrainpoolP384r1Signature().rSig().x__only() + p_signature.ecdsaBrainpoolP384r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP384r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__0)) {
    signature = p_signature.ecdsaBrainpoolP384r1Signature().rSig().compressed__y__0() + p_signature.ecdsaBrainpoolP384r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP384r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__1)) {
    signature = p_signature.ecdsaBrainpoolP384r1Signature().rSig().compressed__y__1() + p_signature.ecdsaBrainpoolP384r1Signature().sSig();
  } else if (p_signature.ecdsaBrainpoolP384r1Signature().rSig().ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_uncompressedP384)) {
    signature = p_signature.ecdsaBrainpoolP384r1Signature().rSig().uncompressedP384().x() +
                p_signature.ecdsaBrainpoolP384r1Signature().rSig().uncompressedP384().y() + p_signature.ecdsaBrainpoolP384r1Signature().sSig();
  } else {
    loggers::get_instance().warning("security_services::verify_sign_ecdsa_brainpoolp384r1 (%s): Invalid curve point", p_certificate_id.c_str());
    return -1;
  }
  security_ecc k(ec_elliptic_curves::brainpool_p_384_r1, public_key_x, public_key_y);
  if (k.sign_verif(hash_to_be_verified, signature) == 0) {
    return 0;
  }

  return -1;
}

int security_services::extract_verification_keys(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y,
                                                 OCTETSTRING &p_public_comp_key, INTEGER &p_public_comp_key_mode) {
  loggers::get_instance().log("security_services::extract_verification_keys");

  if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaNistP256)) {
    if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().compressed__y__0();
      security_ecc ecc(ec_elliptic_curves::nist_p_256, p_public_comp_key, ecc_compressed_mode::compressed_y_0);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(0);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().ischosen(
                 Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().compressed__y__1();
      security_ecc ecc(ec_elliptic_curves::nist_p_256, p_public_comp_key, ecc_compressed_mode::compressed_y_1);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(1);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().ischosen(
                 Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
      security_ecc ecc(ec_elliptic_curves::nist_p_256, p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().uncompressedP256().x(),
                       p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().uncompressedP256().y());
      p_public_comp_key      = ecc.public_key_compressed();
      p_public_comp_key_mode = INTEGER((int)ecc.public_key_compressed_mode());
      p_public_key_x         = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().uncompressedP256().x();
      p_public_key_y         = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaNistP256().uncompressedP256().y();
    } else {
      loggers::get_instance().error("security_services::extract_verification_keys: Unsupported VerificationKey");
      return -1;
    }
  } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaBrainpoolP256r1)) {
    if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().ischosen(
          Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().compressed__y__0();
      security_ecc ecc(ec_elliptic_curves::brainpool_p_256_r1, p_public_comp_key, ecc_compressed_mode::compressed_y_0);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(0);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().ischosen(
                 Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().compressed__y__1();
      security_ecc ecc(ec_elliptic_curves::brainpool_p_256_r1, p_public_comp_key, ecc_compressed_mode::compressed_y_1);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(1);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().ischosen(
                 Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
      p_public_key_x = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().uncompressedP256().x();
      p_public_key_y = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP256r1().uncompressedP256().y();
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaBrainpoolP384r1)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().compressed__y__0();
      security_ecc ecc(ec_elliptic_curves::brainpool_p_384_r1, p_public_comp_key, ecc_compressed_mode::compressed_y_0);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(0);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().ischosen(
                 Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__1)) {
      p_public_comp_key = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().compressed__y__1();
      security_ecc ecc(ec_elliptic_curves::brainpool_p_384_r1, p_public_comp_key, ecc_compressed_mode::compressed_y_1);
      p_public_key_x         = ecc.public_key_x();
      p_public_key_y         = ecc.public_key_y();
      p_public_comp_key_mode = INTEGER(1);
    } else if (p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().ischosen(
                 Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_uncompressedP384)) {
      p_public_key_x = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().uncompressedP384().x();
      p_public_key_y = p_cert.toBeSigned().verifyKeyIndicator().verificationKey().ecdsaBrainpoolP384r1().uncompressedP384().y();
    } else {
      loggers::get_instance().error("security_services::extract_verification_keys: Unsupported VerificationKey");
      return -1;
    }
  } else {
    loggers::get_instance().error("security_services::extract_verification_keys: Unsupported VerificationKey");
    return -1;
  }

  return 0;
}

int security_services::extract_encryption_keys(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_public_enc_key_x, OCTETSTRING &p_public_enc_key_y,
                                               OCTETSTRING &p_public_enc_comp_key, INTEGER &p_public_enc_comp_key_mode) {
  loggers::get_instance().log("security_services::extract_encryption_keys");

  if (p_cert.toBeSigned().encryptionKey().ispresent()) {
    const Ieee1609Dot2BaseTypes::PublicEncryptionKey &p = static_cast<const Ieee1609Dot2BaseTypes::PublicEncryptionKey &>(p_cert.toBeSigned().encryptionKey());
    if (p.publicKey().ischosen(Ieee1609Dot2BaseTypes::BasePublicEncryptionKey::ALT_eciesNistP256)) {
      if (p.publicKey().eciesNistP256().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
        p_public_enc_comp_key = p.publicKey().eciesNistP256().compressed__y__0();
        security_ecc ecc(ec_elliptic_curves::nist_p_256, p_public_enc_comp_key, ecc_compressed_mode::compressed_y_0);
        p_public_enc_key_x         = ecc.public_key_x();
        p_public_enc_key_y         = ecc.public_key_y();
        p_public_enc_comp_key_mode = INTEGER(0);
      } else if (p.publicKey().eciesNistP256().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
        const OCTETSTRING &p_public_enc_comp_key = p.publicKey().eciesNistP256().compressed__y__1();
        security_ecc       ecc(ec_elliptic_curves::nist_p_256, p_public_enc_comp_key, ecc_compressed_mode::compressed_y_1);
        p_public_enc_key_x         = ecc.public_key_x();
        p_public_enc_key_y         = ecc.public_key_y();
        p_public_enc_comp_key_mode = INTEGER(1);
      } else if (p.publicKey().eciesNistP256().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
        p_public_enc_key_x = p.publicKey().eciesNistP256().uncompressedP256().x();
        p_public_enc_key_y = p.publicKey().eciesNistP256().uncompressedP256().y();
      } else {
        loggers::get_instance().error("security_services::extract_encryption_keys: Unsupported EncryptionKey");
        return -1;
      }
    } else if (p.publicKey().ischosen(Ieee1609Dot2BaseTypes::BasePublicEncryptionKey::ALT_eciesBrainpoolP256r1)) {
      if (p.publicKey().eciesBrainpoolP256r1().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
        p_public_enc_comp_key = p.publicKey().eciesBrainpoolP256r1().compressed__y__0();
        security_ecc ecc(ec_elliptic_curves::brainpool_p_256_r1, p_public_enc_comp_key, ecc_compressed_mode::compressed_y_0);
        p_public_enc_key_x         = ecc.public_key_x();
        p_public_enc_key_y         = ecc.public_key_y();
        p_public_enc_comp_key_mode = INTEGER(0);
      } else if (p.publicKey().eciesBrainpoolP256r1().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
        p_public_enc_comp_key = p.publicKey().eciesBrainpoolP256r1().compressed__y__1();
        security_ecc ecc(ec_elliptic_curves::brainpool_p_256_r1, p_public_enc_comp_key, ecc_compressed_mode::compressed_y_1);
        p_public_enc_key_x         = ecc.public_key_x();
        p_public_enc_key_y         = ecc.public_key_y();
        p_public_enc_comp_key_mode = INTEGER(1);
      } else if (p.publicKey().eciesBrainpoolP256r1().ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
        p_public_enc_key_x = p.publicKey().eciesBrainpoolP256r1().uncompressedP256().x();
        p_public_enc_key_y = p.publicKey().eciesBrainpoolP256r1().uncompressedP256().y();
      } else {
        loggers::get_instance().error("security_services::extract_encryption_keys: Unsupported EncryptionKey");
        return -1;
      }
    } else {
      loggers::get_instance().error("security_services::extract_encryption_keys: Unsupported EncryptionKey");
      return -1;
    }
  } else {
    loggers::get_instance().warning("security_services::extract_encryption_keys: EncryptionKey omitted");
    p_public_enc_key_x    = OCTETSTRING(0, nullptr);
    p_public_enc_key_y    = OCTETSTRING(0, nullptr);
    p_public_enc_comp_key = OCTETSTRING(0, nullptr);
    return 0; // Normal termination
  }

  return 0;
} // End of method extract_encryption_keys

int security_services::extract_and_store_certificate(const Ieee1609Dot2::CertificateBase &p_certificate, std::string &p_certificate_id) {
  loggers::get_instance().log_msg(">>> security_services::extract_and_store_certificate: ", p_certificate);

  // Encode certificate
  etsi_ts103097_certificate_codec codec;
  OCTETSTRING                     enc_cert;
  codec.encode(p_certificate, enc_cert);
  if (enc_cert.lengthof() == 0) {
    loggers::get_instance().warning("security_services::extract_and_store_certificate: Failed to encode certificate");
    return -1;
  }
  loggers::get_instance().log_msg("security_services::extract_and_store_certificate: Encoded certificate=", enc_cert);
  OCTETSTRING hash_cert_sha_256;
  hash_sha256(enc_cert, hash_cert_sha_256);
  loggers::get_instance().log_msg("security_services::extract_and_store_certificate: hash_cert_sha_256= ", hash_cert_sha_256);
  int result = -1;
  if (p_certificate.issuer().ischosen(Ieee1609Dot2::IssuerIdentifier::ALT_sha256AndDigest)) {
    // Calculate the hash according to the hashId
    OCTETSTRING hash_cert(hash_cert_sha_256);
    loggers::get_instance().log_msg("security_services::extract_and_store_certificate: hash_cert= ", hash_cert);
    const OCTETSTRING hashed_id8 = substr(hash_cert, hash_cert.lengthof() - 8, 8);
    // Retrieve the certificate identifier from digest
    loggers::get_instance().log_msg("security_services::extract_and_store_certificate: Retrieve the certificate identifier from digest: ", hashed_id8);
    result = _security_db.get()->get_certificate_id(hashed_id8, p_certificate_id);
    if (result == -1) {                                                                    // Not found in current DB
      if (_security_cache.get()->get_certificate_id(hashed_id8, p_certificate_id) == -1) { // Not found in TS cache
        loggers::get_instance().log_msg("security_services::extract_and_store_certificate: Store new certificate in cache: ", p_certificate);
        // const std::vector<unsigned char> v(static_cast<const unsigned char*>(hashed_id8), static_cast<const unsigned char*>(hashed_id8) +
        // hashed_id8.lengthof()); p_certificate_id = converter::get_instance().bytes_to_hexa(v);
        p_certificate_id = std::string(static_cast<const char *>(hex2str(oct2hex(hashed_id8))));
        // Add it into the cache
        OCTETSTRING public_key_x, public_key_y, public_comp_key;
        INTEGER     public_comp_key_mode;
        if (extract_verification_keys(p_certificate, public_key_x, public_key_y, public_comp_key, public_comp_key_mode) == -1) {
          loggers::get_instance().error("security_services::extract_and_store_certificate: Unsupported EncryptionKey");
          return -1;
        }
        // Add encryption keys
        OCTETSTRING public_enc_key_x, public_enc_key_y, public_enc_comp_key;
        INTEGER     public_enc_comp_key_mode;
        if (extract_encryption_keys(p_certificate, public_enc_key_x, public_enc_key_y, public_enc_comp_key, public_enc_comp_key_mode) == -1) {
          loggers::get_instance().error("security_services::extract_and_store_certificate: Unsupported EncryptionKey");
          return -1;
        }
        // And store it into the cache
        _security_cache.get()->store_certificate(CHARSTRING(p_certificate_id.c_str()), enc_cert, int2oct(0, 32), // No way to get the private key here
                                                 public_key_x, public_key_y, public_comp_key, public_comp_key_mode, hash_cert, hash_cert_sha_256, hashed_id8,
                                                 p_certificate.issuer().sha256AndDigest(), OCTETSTRING(0, nullptr), // Encryption private not used
                                                 public_enc_key_x, public_enc_key_y, public_enc_comp_key, public_enc_comp_key_mode);
      }
    }
  } else if (p_certificate.issuer().ischosen(Ieee1609Dot2::IssuerIdentifier::ALT_sha384AndDigest)) {
    // Calculate the hash according to the hashId
    OCTETSTRING hash_cert;
    hash_sha384(enc_cert, hash_cert);
    loggers::get_instance().log_msg("security_services::extract_and_store_certificate: hash_cert= ", hash_cert);
    const OCTETSTRING hashed_id8 = substr(hash_cert, hash_cert.lengthof() - 8, 8);
    // Retrieve the certificate identifier from digest
    loggers::get_instance().log("security_services::extract_and_store_certificate: Retrieve the certificate identifier from digest");
    result = _security_db.get()->get_certificate_id(hashed_id8, p_certificate_id);
    if (result == -1) {
      if (_security_cache.get()->get_certificate_id(hashed_id8, p_certificate_id) == -1) {
        loggers::get_instance().log_msg("security_services::extract_and_store_certificate: Store new certificate in cache: ", p_certificate);
        // const std::vector<unsigned char> v(static_cast<const unsigned char*>(hashed_id8), static_cast<const unsigned char*>(hashed_id8) +
        // hashed_id8.lengthof()); p_certificate_id = converter::get_instance().bytes_to_hexa(v);
        p_certificate_id = std::string(static_cast<const char *>(hex2str(oct2hex(hashed_id8))));
        // Add it into the cache
        OCTETSTRING public_key_x, public_key_y, public_comp_key;
        INTEGER     public_comp_key_mode;
        if (extract_verification_keys(p_certificate, public_key_x, public_key_y, public_comp_key, public_comp_key_mode) == -1) {
          loggers::get_instance().error("security_services::extract_and_store_certificate: Unsupported EncryptionKey");
          return -1;
        }
        // Add encryption keys
        OCTETSTRING public_enc_key_x, public_enc_key_y, public_enc_comp_key;
        INTEGER     public_enc_comp_key_mode;
        if (extract_encryption_keys(p_certificate, public_enc_key_x, public_enc_key_y, public_enc_comp_key, public_enc_comp_key_mode) == -1) {
          loggers::get_instance().error("security_services::extract_and_store_certificate: Unsupported EncryptionKey");
          return -1;
        }
        // And store it into the cache
        _security_cache.get()->store_certificate(CHARSTRING(p_certificate_id.c_str()), enc_cert, int2oct(0, 48), // No way to get the private key here
                                                 public_key_x, public_key_y, public_comp_key, public_comp_key_mode, hash_cert, hash_cert_sha_256, hashed_id8,
                                                 p_certificate.issuer().sha384AndDigest(), OCTETSTRING(0, nullptr), // Encryption private not used
                                                 public_enc_key_x, public_enc_key_y, public_enc_comp_key, public_enc_comp_key_mode);
      }
    }
  } else {
    loggers::get_instance().error("security_services::extract_and_store_certificate: Unsupported issuer");
    return -1;
  }

  return 0;
} // End of method extract_and_store_certificate

int security_services::read_certificate(const CHARSTRING &p_certificate_id, OCTETSTRING &p_certificate) const {
  return _security_db.get()->get_certificate(std::string(static_cast<const char *>(p_certificate_id)), p_certificate);
}

int security_services::read_certificate_digest(const CHARSTRING &p_certificate_id, OCTETSTRING &p_digest) const {
  return _security_db.get()->get_hashed_id(std::string(static_cast<const char *>(p_certificate_id)), p_digest);
}

int security_services::read_certificate_hash(const CHARSTRING &p_certificate_id, OCTETSTRING &p_hash) const {
  return _security_db.get()->get_hash(std::string(static_cast<const char *>(p_certificate_id)), p_hash);
}

int security_services::read_certificate_hash_sha_256(const CHARSTRING &p_certificate_id, OCTETSTRING &p_hash) const {
  return _security_db.get()->get_hash_sha_256(std::string(static_cast<const char *>(p_certificate_id)), p_hash);
}

int security_services::read_certificate_from_digest(const OCTETSTRING &p_digest, CHARSTRING &p_certificate_id) const {
  std::string certificate_id;
  if (_security_db.get()->get_certificate_id(p_digest, certificate_id) != -1) {
    p_certificate_id = CHARSTRING(certificate_id.c_str());
    return 0;
  }
  return -1;
}

int security_services::read_certificate_from_hashed_id3(const OCTETSTRING &p_digest, CHARSTRING &p_certificate_id) const {
  std::string certificate_id;
  if (_security_db.get()->get_certificate_hashed_id3(p_digest, certificate_id) != -1) {
    p_certificate_id = CHARSTRING(certificate_id.c_str());
    return 0;
  }
  return -1;
}

int security_services::read_private_key(const CHARSTRING &p_certificate_id, OCTETSTRING &p_private_key) const {
  return _security_db.get()->get_private_key(std::string(static_cast<const char *>(p_certificate_id)), p_private_key);
}

int security_services::read_private_enc_key(const CHARSTRING &p_certificate_id, OCTETSTRING &p_private_enc_key) const {
  return _security_db.get()->get_private_enc_key(std::string(static_cast<const char *>(p_certificate_id)), p_private_enc_key);
}
