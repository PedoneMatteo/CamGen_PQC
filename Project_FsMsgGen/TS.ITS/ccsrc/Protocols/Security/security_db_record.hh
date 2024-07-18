/*!
 * \file      security_db_record.hh
 * \brief     Header file for a storage of certificates mechanism.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <string>

#include "security_ecc.hh"

class OCTETSTRING; //! TITAN forward declaration

namespace Ieee1609Dot2 {
  class CertificateBase;
};

/*!
 * \class security_db_record
 * \brief This class provides security record description for security database
 */
class security_db_record {

  ec_elliptic_curves _algorithm;        /*!< Signature . Default: nist_p_256 */
  std::string        _certificate_id;   /*!< Certificate storage*/
  OCTETSTRING        _certificate;      /*!< COER storage */
  OCTETSTRING        _hashed_id_issuer; /*!< Hash id 8 of the issuer certificate */
  OCTETSTRING        _hash;             /*!< Certificate hash storage */
  OCTETSTRING        _hash_sha_256; /*!< Certificate hash SHA 256 storage, for encryption (IEEE 1609.2 Clause 5.3.5 Public key encryption algorithms: ECIES */
  OCTETSTRING        _hashed_id;    /*!< Certificate hash id 8 storage */
  OCTETSTRING        _pr_key;       /*!< Private key storage */
  OCTETSTRING        _pu_key_x;     /*!< Public key X-coordinate storage */
  OCTETSTRING        _pu_key_y;     /*!< Public key Y-coordinate storage */
  OCTETSTRING        _pu_comp_key;  /*!< Public compressed key storage */
  OCTETSTRING        _pr_enc_key;   /*!< Private encryption key storage */
  OCTETSTRING        _pu_enc_key_x; /*!< Public encryption key X-coordinate storage */
  OCTETSTRING        _pu_enc_key_y; /*!< Public encryption key Y-coordinate storage */
  OCTETSTRING        _pu_enc_comp_key; /*!< Public compressed encryption key storage */
  bool               _to_be_saved;     /*!< Flag to indicate if the record shall be saved, i.e. has been modified */
  Ieee1609Dot2::CertificateBase *_decoded_certificate;

public: /*! \publicsection */
  /*!
   * \brief Default ctor
   */
  explicit security_db_record()
    : _algorithm{ec_elliptic_curves::nist_p_256}, _certificate_id(), _certificate(), _hashed_id_issuer(), _hash(), _hash_sha_256(), _hashed_id(), _pr_key(),
      _pu_key_x(), _pu_key_y(), _pu_comp_key(), _pr_enc_key(), _pu_enc_key_x(), _pu_enc_key_y(), _pu_enc_comp_key(), _to_be_saved(false),
      _decoded_certificate(nullptr){};
  /*!
   * \brief Specialised ctor
   * \param[in] p_certificate_id The certificate identifier
   * \param[in] p_certificate The COER encoded certificate
   * \param[in] p_decoded_certificate The decoded certificate
   * \param[in] p_hashed_id_issuer The HashedId of the certificate issuer
   * \param[in] p_hash The whole-certificate hash
   * \param[in] p_hash_sha_256 The whole-certificate hash using SHA-256
   * \param[in] p_hashed_id The whole-certificate hashed id
   * \param[in] p_pr_key The private signature key
   * \param[in] p_pu_key_x The public X-coordinate signature key
   * \param[in] p_pu_key_y The public Y-coordinate signature key
   * \param[in] p_pu_comp_key The public compressed coordinate signature key
   * \param[in] p_pr_enc_key The private encryption key
   * \param[in] p_pu_enc_key_x The public X-coordinate encryption key
   * \param[in] p_pu_enc_key_y The public Y-coordinate encryption key
   * \param[in] p_pu_enc_comp_key_y The public compressed coordinate encryption key
   * \param[in] p_to_be_saved Set to true to save on disk this certificate.Default: true
   */
  security_db_record(const std::string &p_certificate_id, const OCTETSTRING &p_certificate, const Ieee1609Dot2::CertificateBase &p_decoded_certificate,
                     const OCTETSTRING &p_hashed_id_issuer, const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashed_id,
                     const OCTETSTRING &p_pr_key, const OCTETSTRING &p_pu_key_x, const OCTETSTRING &p_pu_key_y, const OCTETSTRING &p_pu_comp_key,
                     const OCTETSTRING &p_pr_enc_key, const OCTETSTRING &p_pu_enc_key_x, const OCTETSTRING &p_pu_enc_key_y,
                     const OCTETSTRING &p_pu_enc_comp_key_y, const bool p_to_be_saved = true);

  /*!
   * \brief Default dtor
   */
  ~security_db_record();

  /*!
   * \fn const Ieee1609Dot2::CertificateBase& decoded_certificate() const;
   * \brief Retrieve the decoded certificate value
   * \return The decoded certificate
   */
  const Ieee1609Dot2::CertificateBase &decoded_certificate() const;

  /*!
   * \inline
   * \fn const std::string& certificate_id() const;
   * \brief Retrieve the certificate identifier value
   * \return The certificate identifier
   */
  inline const std::string &certificate_id() const { return _certificate_id; };
  /*!
   * \inline
   * \fn const OCTETSTRING& certificate() const;
   * \brief Retrieve the COER encoded certificate
   * \return The COER encoded certificate
   */
  inline const OCTETSTRING &certificate() const { return _certificate; };
  /*!
   * \inline
   * \fn const OCTETSTRING& hashed_id() const;
   * \brief Retrieve the HashedId of the certificate issuer
   * \return The HashedId of the certificate issuer
   */
  inline const OCTETSTRING &hashed_id() const { return _hashed_id; };
  /*!
   * \inline
   * \fn const OCTETSTRING& hash() const;
   * \brief Retrieve the whole-certificate hashed id
   * \return The whole-certificate hashed id
   */
  inline const OCTETSTRING &hash() const { return _hash; };
  /*!
   * \inline
   * \fn const OCTETSTRING& hash_sha_256() const;
   * \brief Retrieve the whole-certificate hashed id
   * \return The whole-certificate hashed id
   */
  inline const OCTETSTRING &hash_sha_256() const { return _hash_sha_256; };
  inline const OCTETSTRING &issuer() const { return _hashed_id_issuer; };
  inline const OCTETSTRING &private_key() const { return _pr_key; };
  inline const OCTETSTRING &public_key_x() const { return _pu_key_x; };
  inline const OCTETSTRING &public_key_y() const { return _pu_key_y; };
  inline const OCTETSTRING &public_comp_key() const { return _pu_comp_key; };
  inline const OCTETSTRING &private_enc_key() const { return _pr_enc_key; };
  inline const OCTETSTRING &public_enc_key_x() const { return _pu_enc_key_x; };
  inline const OCTETSTRING &public_enc_key_y() const { return _pu_enc_key_y; };
  inline const OCTETSTRING &public_enc_comp_key() const { return _pu_enc_comp_key; };
}; // End of class security_db_record
