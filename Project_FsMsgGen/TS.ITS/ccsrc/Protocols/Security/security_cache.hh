/*!
 * \file      security_cache.hh
 * \brief     Header file for a certificates caching storage mechanism.
 *            It is used to store certificates received from neighbors and not present in the data base
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "security_db_record.hh"

class OCTETSTRING; //! TITAN forward declaration
class INTEGER;     //! TITAN forward declaration
class CHARSTRING;  //! TITAN forward declaration

struct security_cache_comp {
  bool operator()(const OCTETSTRING &p_lhs, const OCTETSTRING &p_rhs) const;
};

/*!
 * \class security_cache
 * \brief This class provides security record description for in memory security database. In memory mens there is no disk load/save mechanism
 */
class security_cache {
  static bool fill_vector(OCTETSTRING &p_vector, const OCTETSTRING &p_org);

protected: /*! \protectedsection */
  // TODO Enforce with const security_cache_record, and const std::string
  std::map<std::string, std::unique_ptr<security_db_record>> _certificates; //! List of the certificates indexed by the certificate identifier
  std::map<OCTETSTRING, std::string, security_cache_comp>    _hashed_id8s;  //! List of the certificates indexed by the HashedId8
  // std::map<std::vector<unsigned char>, std::string> _hashed_id8s; //! List of the certificates indexed by the HashedId8

public: /*! \publicsection */
  /*!
   * \brief Default ctor
   */
  security_cache();
  /*!
   * \brief Default private dtor
   */
  virtual ~security_cache();

  int get_certificate_id(const OCTETSTRING &p_hashed_id8, std::string &p_certificate_id) const;
  int get_certificate(const std::string &p_certificate_id, OCTETSTRING &p_certificate) const;
  int get_certificate(const std::string &p_certificate_id, Ieee1609Dot2::CertificateBase &p_certificate) const;
  int get_certificate(const OCTETSTRING &p_hashed_id3, Ieee1609Dot2::CertificateBase &p_certificate) const;
  int get_certificate_hashed_id3(const OCTETSTRING &p_hashed_id3, std::string &p_certificate_id) const;
  int get_issuer(const std::string &p_certificate_id, OCTETSTRING &p_hashed_id_issuer) const;
  int get_hashed_id(const std::string &p_certificate_id, OCTETSTRING &p_hashed_id) const;
  int get_hash(const std::string &p_certificate_id, OCTETSTRING &p_hash) const;
  int get_hash_sha_256(const std::string &p_certificate_id, OCTETSTRING &p_hash) const;
  int get_private_key(const std::string &p_certificate_id, OCTETSTRING &p_private_key) const;
  int get_public_keys(const std::string &p_certificate_id, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y) const;
  int get_public_comp_key(const std::string &p_certificate_id, OCTETSTRING &p_public_comp_key, INTEGER &p_comp_mode) const;
  int get_private_enc_key(const std::string &p_certificate_id, OCTETSTRING &p_private_enc_key) const;
  int get_public_enc_keys(const std::string &p_certificate_id, OCTETSTRING &p_public_enc_key_x, OCTETSTRING &p_public_enc_key_y) const;
  int get_public_enc_comp_key(const std::string &p_certificate_id, OCTETSTRING &p_public_enc_comp_key, INTEGER &p_enc_comp_mode) const;

  virtual int store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key, const OCTETSTRING &p_public_key_x,
                                const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_compressed_key, const INTEGER &p_public_compressed_key_mode,
                                const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashid8, const OCTETSTRING &p_issuer,
                                const OCTETSTRING &p_private_enc_key, const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                                const OCTETSTRING &p_public_enc_compressed_key, const INTEGER &p_public_enc_compressed_key_mode);

  virtual int clear();

  void dump() const;

}; // End of class security_cache
