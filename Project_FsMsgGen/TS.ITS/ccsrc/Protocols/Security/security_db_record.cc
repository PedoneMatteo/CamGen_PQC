/*!
 * \file      security_db_record.cc
 * \brief     Source file for a storage of certificates mechanism.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include "EtsiTs103097Module.hh"

#include "security_db_record.hh"
#include "sha256.hh"
#include "sha384.hh"

security_db_record::security_db_record(const std::string &p_certificate_id, const OCTETSTRING &p_certificate,
                                       const Ieee1609Dot2::CertificateBase &p_decoded_certificate, const OCTETSTRING &p_hashed_id_issuer,
                                       const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashed_id,
                                       const OCTETSTRING &p_pr_key, const OCTETSTRING &p_pu_key_x, const OCTETSTRING &p_pu_key_y,
                                       const OCTETSTRING &p_pu_comp_key, const OCTETSTRING &p_pr_enc_key, const OCTETSTRING &p_pu_enc_key_x,
                                       const OCTETSTRING &p_pu_enc_key_y, const OCTETSTRING &p_pu_enc_comp_key, const bool p_to_be_saved)
  : _algorithm{ec_elliptic_curves::nist_p_256}, _certificate_id(p_certificate_id), _certificate(p_certificate), _hashed_id_issuer(p_hashed_id_issuer),
    _hash(p_hash), _hash_sha_256(p_hash_sha_256), _hashed_id(p_hashed_id), _pr_key(p_pr_key), _pu_key_x(p_pu_key_x), _pu_key_y(p_pu_key_y),
    _pu_comp_key(p_pu_comp_key), _pr_enc_key(p_pr_enc_key), _pu_enc_key_x(p_pu_enc_key_x), _pu_enc_key_y(p_pu_enc_key_y), _pu_enc_comp_key(p_pu_enc_comp_key),
    _to_be_saved(p_to_be_saved), _decoded_certificate(static_cast<Ieee1609Dot2::CertificateBase *>(p_decoded_certificate.clone())) {}

security_db_record::~security_db_record() {
  if (_decoded_certificate != nullptr) {
    delete _decoded_certificate;
  }
};

const Ieee1609Dot2::CertificateBase &security_db_record::decoded_certificate() const { return *_decoded_certificate; }
