/*!
 * \file      security_cache.cc
 * \brief     Source file for a certificates caching storage mechanism.
 *            It is used to store certificates received from neighbors and not present in the data base
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <TTCN3.hh>

#include "security_cache.hh"

#include "params.hh"

#include "converter.hh"

using namespace std; // Required for isnan()
#include "etsi_ts103097_certificate_codec.hh"

#include "loggers.hh"

bool security_cache_comp::operator()(const OCTETSTRING &p_lhs, const OCTETSTRING &p_rhs) const {
  // loggers::get_instance().log_msg(">>> security_cache_comp::operator(): p_lhs= ", p_lhs);
  // loggers::get_instance().log_msg(">>> security_cache_comp::operator(): p_rhs= ", p_rhs);
  unsigned char *first1 = (unsigned char *)static_cast<const unsigned char *>(p_lhs);
  unsigned char *first2 = (unsigned char *)static_cast<const unsigned char *>(p_rhs);
  unsigned char *last1  = p_lhs.lengthof() + (unsigned char *)static_cast<const unsigned char *>(p_lhs);
  unsigned char *last2  = p_rhs.lengthof() + (unsigned char *)static_cast<const unsigned char *>(p_rhs);

  while (first1 != last1) {
    if ((first2 == last2) || (*first2 < *first1)) {
      return false;
    } else if (*first1 < *first2) {
      return true;
    }
    ++first1;
    ++first2;
  } // End of 'while' statement
  return (first2 != last2);
}

security_cache::security_cache() : _certificates(), _hashed_id8s() { loggers::get_instance().log(">>> security_cache::security_cache"); } // End of ctor

security_cache::~security_cache() {
  loggers::get_instance().log(">>> security_cache::~security_cache");
  clear();
} // End of dtor

int security_cache::clear() {
  loggers::get_instance().log(">>> security_cache::clear");
  _certificates.clear(); // Smart pointers will do the job
  _hashed_id8s.clear();

  return 0;
} // End of clear method

int security_cache::get_certificate_id(const OCTETSTRING &p_hashed_id8, std::string &p_certificate_id) const {
  loggers::get_instance().log_msg(">>> security_cache::get_certificate_id: ", p_hashed_id8);

  std::map<OCTETSTRING, std::string>::const_iterator it = _hashed_id8s.find(p_hashed_id8);
  // std::vector<unsigned char> v(static_cast<const unsigned char*>(p_hashed_id8), p_hashed_id8.lengthof() + static_cast<const unsigned char*>(p_hashed_id8));
  // std::map<std::vector<unsigned char>, std::string>::const_iterator it = _hashed_id8s.find(v);
  if (it == _hashed_id8s.cend()) {
    dump();
    p_certificate_id = "";
    return -1;
  }
  p_certificate_id = it->second;

  return 0;
}

int security_cache::get_certificate(const std::string &p_certificate_id, OCTETSTRING &p_certificate) const {
  loggers::get_instance().log(">>> security_cache::get_certificate: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_certificate: record not found");
    dump();
    return -1;
  }
  p_certificate = it->second.get()->certificate();

  return 0;
}

int security_cache::get_certificate(const std::string &p_certificate_id, Ieee1609Dot2::CertificateBase &p_certificate) const {
  loggers::get_instance().log(">>> security_cache::get_certificate: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_certificate: record not found");
    return -1;
  }
  p_certificate = it->second.get()->decoded_certificate();

  return 0;
}

int security_cache::get_certificate(const OCTETSTRING &p_hashed_id3, Ieee1609Dot2::CertificateBase &p_certificate) const {
  loggers::get_instance().log_msg(">>> security_cache::get_certificate: ", p_hashed_id3);

  std::map<OCTETSTRING, std::string, security_cache_comp>::const_iterator s;
  for (s = _hashed_id8s.cbegin(); s != _hashed_id8s.cend(); ++s) {
    if ((p_hashed_id3[2] == (s->first)[(s->first).lengthof() - 1]) && (p_hashed_id3[1] == (s->first)[(s->first).lengthof() - 2]) &&
        (p_hashed_id3[0] == (s->first)[(s->first).lengthof() - 3])) {
      break;
    }
  } // End of 'for' statement
  if (s == _hashed_id8s.cend()) {
    loggers::get_instance().warning("security_cache::get_certificate: hashedId3 does not match");
    return -1;
  }
  /*TODO std::map<OCTETSTRING, std::string, security_cache_comp>::const_iterator s = std::find_if(
                                                                                           _hashed_id8s.cbegin(),
                                                                                           _hashed_id8s.cend(),
                                                                                           hashed_id_matching
                                                                                           );*/
  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(s->second);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_certificate: record not found");
    return -1;
  }
  p_certificate = it->second.get()->decoded_certificate();

  return 0;
}

int security_cache::get_certificate_hashed_id3(const OCTETSTRING &p_hashed_id3, std::string &p_certificate_id) const {
  loggers::get_instance().log_msg(">>> security_cache::get_certificate_hashed_id3: ", p_hashed_id3);

  std::map<OCTETSTRING, std::string, security_cache_comp>::const_iterator s;
  for (s = _hashed_id8s.cbegin(); s != _hashed_id8s.cend(); ++s) {
    if ((p_hashed_id3[2] == (s->first)[(s->first).lengthof() - 1]) && (p_hashed_id3[1] == (s->first)[(s->first).lengthof() - 2]) &&
        (p_hashed_id3[0] == (s->first)[(s->first).lengthof() - 3])) {
      break;
    }
  } // End of 'for' statement
  if (s == _hashed_id8s.cend()) {
    loggers::get_instance().warning("security_cache::get_certificate_hashed_id3: hashedId3 does not match");
    return -1;
  }
  /*TODO std::map<OCTETSTRING, std::string, security_cache_comp>::const_iterator s = std::find_if(
                                                                                           _hashed_id8s.cbegin(),
                                                                                           _hashed_id8s.cend(),
                                                                                           hashed_id_matching
                                                                                           );*/
  p_certificate_id = s->second;

  return 0;
}

int security_cache::get_issuer(const std::string &p_certificate_id, OCTETSTRING &p_hashed_id_issuer) const {
  loggers::get_instance().log(">>> security_cache::get_issuer: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_issuer: record not found");
    return -1;
  }
  p_hashed_id_issuer = it->second.get()->issuer();

  return 0;
}

int security_cache::get_hashed_id(const std::string &p_certificate_id, OCTETSTRING &p_hashed_id) const {
  loggers::get_instance().log(">>> security_cache::get_hashed_id: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_hashed_id: record not found");
    return -1;
  }
  p_hashed_id = it->second.get()->hashed_id();

  return 0;
}

int security_cache::get_hash(const std::string &p_certificate_id, OCTETSTRING &p_hash) const {
  loggers::get_instance().log(">>> security_cache::get_hash: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_hash: record not found");
    return -1;
  }
  p_hash = it->second.get()->hash();

  return 0;
}

int security_cache::get_hash_sha_256(const std::string &p_certificate_id, OCTETSTRING &p_hash) const {
  loggers::get_instance().log(">>> security_cache::get_hash_sha_256: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_hash_sha_256: record not found");
    return -1;
  }
  p_hash = it->second.get()->hash_sha_256();

  return 0;
}

int security_cache::get_private_key(const std::string &p_certificate_id, OCTETSTRING &p_private_key) const {
  loggers::get_instance().log(">>> security_cache::get_private_key: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_private_key: record not found");
    return -1;
  }
  p_private_key = it->second.get()->private_key();

  return 0;
}

int security_cache::get_public_keys(const std::string &p_certificate_id, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y) const {
  loggers::get_instance().log(">>> security_cache::get_public_keys: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_public_keys: record not found");
    return -1;
  }
  p_public_key_x = it->second.get()->public_key_x();
  p_public_key_y = it->second.get()->public_key_y();

  return 0;
}

int security_cache::get_public_comp_key(const std::string &p_certificate_id, OCTETSTRING &p_public_comp_key, INTEGER &p_comp_mode) const {
  loggers::get_instance().log(">>> security_cache::get_public_comp_key: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_public_comp_key: record not found");
    return -1;
  }
  const OCTETSTRING public_comp_key = it->second.get()->public_comp_key();                                                  // 33 or 49 bytes length
  p_public_comp_key = OCTETSTRING(public_comp_key.lengthof() - 1, 1 + static_cast<const unsigned char *>(public_comp_key)); // 32 or 48 bytes length
  p_comp_mode       = INTEGER(((public_comp_key[0].get_octet() % 2) == 0) ? 0 : 1);                                         // compressed-y-0 or compressed-y-1

  return 0;
}

int security_cache::get_private_enc_key(const std::string &p_certificate_id, OCTETSTRING &p_private_enc_key) const {
  loggers::get_instance().log(">>> security_cache::get_private_enc_key: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_private_enc_key: record not found");
    return -1;
  }
  p_private_enc_key = it->second.get()->private_enc_key();

  return 0;
}

int security_cache::get_public_enc_keys(const std::string &p_certificate_id, OCTETSTRING &p_public_enc_key_x, OCTETSTRING &p_public_enc_key_y) const {
  loggers::get_instance().log(">>> security_cache::get_public_enc_keys: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_public_enc_keys: record not found");
    return -1;
  }
  p_public_enc_key_x = it->second.get()->public_enc_key_x();
  p_public_enc_key_y = it->second.get()->public_enc_key_y();

  return 0;
}

int security_cache::get_public_enc_comp_key(const std::string &p_certificate_id, OCTETSTRING &p_public_enc_comp_key, INTEGER &p_enc_comp_mode) const {
  loggers::get_instance().log(">>> security_cache::get_public_enc_comp_key: '%s'", p_certificate_id.c_str());

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(p_certificate_id);
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_cache::get_public_enc_comp_key: record not found");
    return -1;
  }
  const OCTETSTRING public_enc_comp_key = it->second.get()->public_enc_comp_key();                                                      // 33 or 49 bytes length
  p_public_enc_comp_key = OCTETSTRING(public_enc_comp_key.lengthof() - 1, 1 + static_cast<const unsigned char *>(public_enc_comp_key)); // 32 or 48 bytes length
  p_enc_comp_mode       = INTEGER(((public_enc_comp_key[0].get_octet() % 2) == 0) ? 0 : 1); // compressed-y-0 or compressed-y-1

  return 0;
}

bool security_cache::fill_vector(OCTETSTRING &p_vector, const OCTETSTRING &p_org) {
  if (p_org.is_bound()) {
    p_vector = p_org;
    return true;
  }
  p_vector = OCTETSTRING(0, nullptr);
  return false;
}

int security_cache::store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key,
                                      const OCTETSTRING &p_public_key_x, const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_compressed_key,
                                      const INTEGER &p_public_compressed_key_mode, const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256,
                                      const OCTETSTRING &p_hashed_id8, const OCTETSTRING &p_issuer, const OCTETSTRING &p_private_enc_key,
                                      const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                                      const OCTETSTRING &p_public_enc_compressed_key, const INTEGER &p_public_enc_compressed_key_mode) {
  loggers::get_instance().log_msg(">>> security_cache::store_certificate: ", p_cert_id);

  std::string key(static_cast<const char *>(p_cert_id));
  OCTETSTRING cert, private_key, public_key_x, public_key_y, public_comp_key, hash, hash_sha_256, hashed_id8, issuer;
  OCTETSTRING private_enc_key, public_enc_key_x, public_enc_key_y, public_enc_comp_key;

  security_cache::fill_vector(cert, p_cert);
  security_cache::fill_vector(private_key, p_private_key);

  security_cache::fill_vector(public_key_x, p_public_key_x);
  security_cache::fill_vector(public_key_y, p_public_key_y);
  if (fill_vector(public_comp_key, p_public_compressed_key)) {
    public_comp_key =
      int2oct((unsigned char)(2 + p_public_compressed_key_mode), 1) + public_comp_key; // Add one byte to indicate cmpressed-y-0 (0x02) or compressed-y-1 (0x03)
    loggers::get_instance().log_msg("security_cache::store_certificate: public_comp_key: ", public_comp_key);
  }

  security_cache::fill_vector(hash, p_hash);
  security_cache::fill_vector(hash_sha_256, p_hash_sha_256);
  security_cache::fill_vector(hashed_id8, p_hashed_id8);
  security_cache::fill_vector(issuer, p_issuer);

  security_cache::fill_vector(private_enc_key, p_private_enc_key);
  security_cache::fill_vector(public_enc_key_x, p_public_enc_key_x);
  security_cache::fill_vector(public_enc_key_y, p_public_enc_key_y);
  if (fill_vector(public_enc_comp_key, p_public_enc_compressed_key)) {
    public_enc_comp_key = int2oct((unsigned char)(2 + p_public_compressed_key_mode), 1) +
                          public_enc_comp_key; // Add one byte to indicate cmpressed-y-0 (0x02) or compressed-y-1 (0x03)
    loggers::get_instance().log_msg("security_cache::store_certificate: public_enc_comp_key: ", public_enc_comp_key);
  }

  Ieee1609Dot2::CertificateBase   decoded_certificate;
  etsi_ts103097_certificate_codec codec;
  codec.decode(p_cert, decoded_certificate);
  loggers::get_instance().log_msg("security_cache::store_certificate: Decoded certificate: ", decoded_certificate);
  security_db_record *p = new security_db_record(key,  // Certificate id
                                                 cert, // Certificate
                                                 decoded_certificate,
                                                 issuer,             // Hashed ID for the issuer
                                                 hash,               // Whole-certificate hash
                                                 hash_sha_256,       // Whole-certificate hash using SHA-256
                                                 hashed_id8,         // Whole-certificate hashed ID
                                                 private_key,        // Private key
                                                 public_key_x,       // Public key X-coordinate
                                                 public_key_y,       // Public key Y-coordinate
                                                 public_comp_key,    // Public compressed key, 33 or 49 bytes
                                                 private_enc_key,    // Private encryption key
                                                 public_enc_key_x,   // Public encryption key X-coordinate
                                                 public_enc_key_y,   // Public encryption key Y-coordinate
                                                 public_enc_comp_key // Public encryption compressed key, 33 or 49 bytes
  );
  if (p == nullptr) {
    return -1;
  }
  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator i = _certificates.find(key);
  if (i != _certificates.cend()) {
    _certificates.erase(i);
  }

  _certificates.insert(std::pair<std::string, std::unique_ptr<security_db_record>>(key, std::unique_ptr<security_db_record>(p)));

  std::map<OCTETSTRING, std::string>::const_iterator h = _hashed_id8s.find(hashed_id8);
  // std::vector<unsigned char> v(static_cast<const unsigned char*>(hashed_id8), hashed_id8.lengthof() + static_cast<const unsigned char*>(hashed_id8));
  // std::map<std::vector<unsigned char>, std::string>::const_iterator h = _hashed_id8s.find(v);
  if (h != _hashed_id8s.cend()) {
    loggers::get_instance().log_msg("security_cache::store_certificate: Replace entry ", h->first);
    _hashed_id8s.erase(h);
  }
  loggers::get_instance().log_msg("security_cache::store_certificate: Add entry ", hashed_id8);
  loggers::get_instance().log("security_cache::store_certificate: Add entry for key %s", key.c_str());
  _hashed_id8s.insert(std::pair<OCTETSTRING, std::string &>(hashed_id8, key));
  //_hashed_id8s.insert(std::pair<std::vector<unsigned char>, std::string&>(v, key));

  // dump();

  return 0;
}

void security_cache::dump() const {
  loggers::get_instance().log("security_cache::dump_certificates: # items = %d", _certificates.size());
  for (std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.cbegin(); it != _certificates.cend(); ++it) {
    security_db_record *p = it->second.get();
    loggers::get_instance().log("security_cache::dump: certificate_id = %s", p->certificate_id().c_str());
    loggers::get_instance().log_msg("security_cache::dump: certificate = ", p->certificate());
    loggers::get_instance().log_msg("security_cache::dump: decoded certificate = ", p->decoded_certificate());
    loggers::get_instance().log_msg("security_cache::dump: issuer = ", p->issuer());
    loggers::get_instance().log_msg("security_cache::dump: hashed_id = ", p->hashed_id());
    loggers::get_instance().log_msg("security_cache::dump: hash = ", p->hash());
    loggers::get_instance().log_msg("security_cache::dump: hash_sha_256 = ", p->hash_sha_256());
    loggers::get_instance().log_msg("security_cache::dump: private_key = ", p->private_key());
    loggers::get_instance().log_msg("security_cache::dump: public_key_x = ", p->public_key_x());
    loggers::get_instance().log_msg("security_cache::dump: public_key_y = ", p->public_key_y());
    loggers::get_instance().log_msg("security_cache::dump: public_comp_key = ", p->public_comp_key());
    loggers::get_instance().log_msg("security_cache::dump: private_enc_key = ", p->private_enc_key());
    loggers::get_instance().log_msg("security_cache::dump: public_enc_key_x = ", p->public_enc_key_x());
    loggers::get_instance().log_msg("security_cache::dump: public_enc_key_y = ", p->public_enc_key_y());
    loggers::get_instance().log_msg("security_cache::dump: public_enc_comp_key = ", p->public_enc_comp_key());
  } // End of 'for' statement
  // for (std::map<OCTETSTRING, std::string>::const_iterator it = _hashed_id8s.cbegin(); it != _hashed_id8s.cend(); ++it) {
  loggers::get_instance().log("security_cache::dump: Hashedid8 table length: %d", _hashed_id8s.size());
  for (std::map<OCTETSTRING /*std::vector<unsigned char>*/, std::string>::const_iterator it = _hashed_id8s.cbegin(); it != _hashed_id8s.cend(); ++it) {
    loggers::get_instance().log_msg("security_cache::dump: Hashedid8 key = ", it->first);
    loggers::get_instance().log("security_cache::dump: Hashedid8 idx ==> %s", it->second.c_str());
  } // End of 'for' statement
} // End of method dump
