#include <TTCN3.hh>

#include "security_db.hh"

#include "params.hh"

#include "converter.hh"

using namespace std; // Required for isnan()
#include "certificates_loader.hh"

#include "etsi_ts103097_certificate_codec.hh"

#include "loggers.hh"

security_db::security_db(const std::string &p_db_path) {
  loggers::get_instance().log(">>> security_db::security_db: '%s'", p_db_path.c_str());

  load_from_files(p_db_path);
  dump();
} // End of ctor

security_db::~security_db() { loggers::get_instance().log(">>> security_db::~security_db"); } // End of dtor

int security_db::load_from_files(const std::string &p_db_path) {
  loggers::get_instance().log(">>> security_db::load_from_files: '%s'", p_db_path.c_str());

  if (certificates_loader::get_instance().build_path(p_db_path) == -1) {
    loggers::get_instance().log(">>> security_db::load_from_files: certificates_loader::build_path method failed");
    return -1;
  }

  if (certificates_loader::get_instance().load_certificates(_certificates, _hashed_id8s) == -1) {
    loggers::get_instance().log(">>> security_db::load_from_files: certificates_loader::load_certificates method failed");
    return -1;
  }

  return 0;
}

int security_db::store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key, const OCTETSTRING &p_public_key_x,
                                   const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_compressed_key, const INTEGER &p_public_compressed_key_mode,
                                   const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashed_id8, const OCTETSTRING &p_issuer,
                                   const OCTETSTRING &p_private_enc_key, const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                                   const OCTETSTRING &p_public_enc_compressd_key, const INTEGER &p_public_enc_compressed_key_mode) {
  loggers::get_instance().log_msg(">>> security_db::store_certificate: ", p_cert_id);

  if (security_cache::store_certificate(p_cert_id, p_cert, p_private_key, p_public_key_x, p_public_key_y, p_public_compressed_key, p_public_compressed_key_mode,
                                        p_hash, p_hash_sha_256, p_hashed_id8, p_issuer, p_private_enc_key, p_public_enc_key_x, p_public_enc_key_y,
                                        p_public_enc_compressd_key, p_public_enc_compressed_key_mode) != 0) {
    return -1;
  }

  std::map<std::string, std::unique_ptr<security_db_record>>::const_iterator it = _certificates.find(std::string(static_cast<const char *>(p_cert_id)));
  if (it == _certificates.cend()) {
    loggers::get_instance().warning("security_db::get_certificate: record not found");
    return -1;
  }
  if (certificates_loader::get_instance().save_certificate(*it->second) == -1) {
    return -1;
  }

  return 0;
}
