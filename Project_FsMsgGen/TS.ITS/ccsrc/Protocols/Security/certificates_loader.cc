#include <fstream>
#include <iostream>

#include "EtsiTs103097Module.hh"

#include "certificates_loader.hh"

#include "etsi_ts103097_certificate_codec.hh"

#include "sha256.hh"
#include "sha384.hh"

#include "converter.hh"

#include "loggers.hh"

certificates_loader *certificates_loader::instance = nullptr;

certificates_loader::certificates_loader()
  : _certificateExt{".oer"}, _privateKeyExt{".vkey"}, _privateEncKeyExt{".ekey"},
    _full_path(), _is_cache_initialized{false}, _directory_filter{".svn", "._.DS_Store", ".DS_Store"} {
  // loggers::get_instance().log(">>> certificates_loader::certificates_loader");
} // End of ctor

int certificates_loader::build_path(const std::string &p_root_directory) {
  // loggers::get_instance().log(">>> certificates_loader::build_path: '%s'", p_root_directory.c_str());

  // Build full path
  if (!p_root_directory.empty()) {
    _full_path = p_root_directory;
    if (!std::experimental::filesystem::exists(_full_path) ||
        !std::experimental::filesystem::is_directory(_full_path)) { // FIXME Coredump when app hasn't the rights to create the directory!!!!
      // Create directory
      if (!std::experimental::filesystem::create_directory(_full_path)) {
        _full_path = "./";
      } else { // Set rights for all users
        std::experimental::filesystem::permissions(_full_path,
                                                   std::experimental::filesystem::perms::add_perms | std::experimental::filesystem::perms::owner_all |
                                                     std::experimental::filesystem::perms::group_all | std::experimental::filesystem::perms::others_all);
      }
    }
  } else {
    _full_path = "./";
  }
  std::experimental::filesystem::canonical(_full_path);
  // loggers::get_instance().log("certificates_loader::build_path: full path: %s", _full_path.string().c_str());
  if (!std::experimental::filesystem::exists(_full_path)) {
    loggers::get_instance().warning("certificates_loader::build_path: Invalid path");
    _full_path.clear();
    return -1;
  }

  return 0;
} // End of method build_path

int certificates_loader::load_certificates(std::map<std::string, std::unique_ptr<security_db_record>> &                            p_certificates,
                                           std::map<OCTETSTRING /*std::vector<unsigned char>*/, std::string, security_cache_comp> &p_hashed_id8s) {
  // loggers::get_instance().log(">>> certificates_loader::load_certificates");

  // Sanity check
  if (_is_cache_initialized) {
    return 0;
  }

  // Retrieve the list of the files in the path
  std::set<std::experimental::filesystem::path> files;
  if (retrieve_certificates_list(files) == -1) {
    loggers::get_instance().warning("certificates_loader::load_certificates: Failed to build the list of certificate files");
    return -1;
  }
  // Build the certificates cache
  if (build_certificates_cache(files, p_certificates, p_hashed_id8s) == -1) {
    loggers::get_instance().warning("certificates_loader::load_certificates: Failed to build the certificate cache");
    return -1;
  }

  _is_cache_initialized = true;

  return 0;
} // End of method load_certificates

int certificates_loader::retrieve_certificates_list(std::set<std::experimental::filesystem::path> &p_files) {
  // loggers::get_instance().log(">>> certificates_loader::retrieve_certificates_list");

  // Walk through directories
  std::set<std::experimental::filesystem::path> folders;
  for (const std::experimental::filesystem::directory_entry it : std::experimental::filesystem::recursive_directory_iterator(_full_path.string())) {
    // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Processing directory '%s'", it.path().string().c_str());
    if (std::experimental::filesystem::is_directory(it)) {
      std::set<std::string>::const_iterator i = _directory_filter.find(it.path().filename());
      if (i != _directory_filter.cend()) {
        // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Exclude directory '%s'", it.path().string().c_str());
        continue;
      }
      // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Add directory '%s'", it.path().string().c_str());
      folders.insert(it.path());
    }
  } // End of 'for' statement

  if (folders.size() == 0) {
    loggers::get_instance().warning("certificates_loader::retrieve_certificates_list: No folder after filtering");
    folders.insert(_full_path);
  }
  // Process files
  p_files.clear();
  for (std::set<std::experimental::filesystem::path>::const_reverse_iterator f = folders.crbegin(); f != folders.crend(); ++f) {
    // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Processing directory '%s'", f->string().c_str());
    for (const std::experimental::filesystem::directory_entry it : std::experimental::filesystem::recursive_directory_iterator(*f)) {
      // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Processing file '%s'", it.path().filename().string().c_str());
      if (std::experimental::filesystem::is_regular_file(it)) {
        // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Check extension '%s'", it.path().extension().string().c_str());
        if (it.path().extension().string().compare(_certificateExt) == 0) {
          // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: Add file '%s'", it.path().filename().string().c_str());
          p_files.insert(it);
        }
      }
    } // End of 'for' statement
  }   // End of 'for' statement
  // loggers::get_instance().log("certificates_loader::retrieve_certificates_list: # of files to cache: %d", p_files.size());
  if (p_files.size() == 0) {
    loggers::get_instance().warning("certificates_loader::retrieve_certificates_list: No certificate found");
    return -1;
  }

  return 0;
} // End of method retrieve_certificates_list

int certificates_loader::build_certificates_cache(std::set<std::experimental::filesystem::path> &                                         p_files,
                                                  std::map<std::string, std::unique_ptr<security_db_record>> &                            p_certificates,
                                                  std::map<OCTETSTRING /*std::vector<unsigned char>*/, std::string, security_cache_comp> &p_hashed_id8s) {
  // loggers::get_instance().log(">>> certificates_loader::build_certificates_cache");

  std::set<std::experimental::filesystem::path>::const_iterator                         it = p_files.cbegin();
  std::pair<std::map<std::string, std::unique_ptr<security_db_record>>::iterator, bool> result;
  do {
    loggers::get_instance().log("certificates_loader::build_certificates_cache: Caching '%s'", it->string().c_str());
    std::experimental::filesystem::path p   = *it;
    const std::string &                 key = p.stem();
    // loggers::get_instance().log("certificates_loader::build_certificates_cache: Key = '%s'", key.c_str());

    // Load certificate file
    it = p_files.find(p.replace_extension(_certificateExt));
    if (it == p_files.cend()) {
      loggers::get_instance().warning("certificates_loader::build_certificates_cache: Certificate file not found for '%s'", key.c_str());
      return -1;
    }
    // loggers::get_instance().log("certificates_loader::build_certificates_cache: Caching certificate '%s'", it->string().c_str());
    std::ifstream is(it->string(), ios::in | ios::binary);
    if (!is.is_open()) {
      loggers::get_instance().warning("certificates_loader::build_certificates_cache: Failed to open certificate file");
      return -1;
    }
    OCTETSTRING certificate = int2oct(0, std::experimental::filesystem::file_size(*it));
    is.read((char *)static_cast<const unsigned char *>(certificate), certificate.lengthof());
    is.close();
    // Remove items from the list
    p_files.erase(it);

    // Load private key file
    OCTETSTRING private_key;
    p = p.replace_extension(_privateKeyExt);
    // loggers::get_instance().log("certificates_loader::build_certificates_cache: Caching private keys '%s'", p.string().c_str());
    is.open(p, ios::in | ios::binary);
    if (!is.is_open()) {
      loggers::get_instance().warning("certificates_loader::build_certificates_cache: Failed to open Private key");
      private_key = OCTETSTRING(0, nullptr);
    } else {
      int size = std::experimental::filesystem::file_size(p);
      if ((size != 32) && (size != 48)) {
        loggers::get_instance().warning("certificates_loader::build_certificates_cache: Private key size is incorrect for '%s'", key.c_str());
        return -1;
      }
      private_key = int2oct(0, size);
      is.read((char *)static_cast<const unsigned char *>(private_key), private_key.lengthof());
      is.close();
    }

    // Load private encryption key file if present
    OCTETSTRING private_enc_key;
    p = p.replace_extension(_privateEncKeyExt);
    // loggers::get_instance().log("certificates_loader::build_certificates_cache: Caching private encryption key '%s'", p.string().c_str());
    is.open(p, ios::in | ios::binary);
    if (!is.is_open()) {
      loggers::get_instance().warning("certificates_loader::build_certificates_cache: Failed to open Private encryption key file");
      private_enc_key = OCTETSTRING(0, nullptr);
    } else {
      int size = std::experimental::filesystem::file_size(p);
      if (size != 32) { // IEEE Std 1609.2 2017: NistP256 or BrainpoolP256r1
        loggers::get_instance().warning("certificates_loader::build_certificates_cache: Private encryption key size is incorrect for '%s'", key.c_str());
        return -1;
      }
      private_enc_key = int2oct(0, size);
      is.read((char *)static_cast<const unsigned char *>(private_enc_key), private_enc_key.lengthof());
      is.close();
    }

    // Build DB record
    etsi_ts103097_certificate_codec codec;
    Ieee1609Dot2::CertificateBase   decoded_certificate;
    codec.decode(certificate, decoded_certificate);
    if (!decoded_certificate.is_value()) {
      loggers::get_instance().warning("certificates_loader::build_certificates_cache: Failed to decode certificate for '%s'", key.c_str());
      return -1;
    } else {
      // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Decoded certificate: ", decoded_certificate);
      // Prepare all fields
      if (!decoded_certificate.toBeSigned().verifyKeyIndicator().ischosen(Ieee1609Dot2::VerificationKeyIndicator::ALT_verificationKey)) {
        loggers::get_instance().warning("certificates_loader::build_certificates_cache: Wrong VerificationKeyIndicator variant for '%s'", key.c_str());
        return -1;
      }
      OCTETSTRING public_key_x;
      OCTETSTRING public_key_y;
      OCTETSTRING public_comp_key; // public compressed key, 33 or 49 bytes length, byte #0 indicating compressed-y-0 (0x02) or compressed-y-1 (0x03)
      Ieee1609Dot2BaseTypes::PublicVerificationKey &b = decoded_certificate.toBeSigned().verifyKeyIndicator().verificationKey();
      if (b.ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaNistP256)) {
        Ieee1609Dot2BaseTypes::EccP256CurvePoint &p = b.ecdsaNistP256();
        fill_public_key_vectors(ec_elliptic_curves::nist_p_256, p, public_comp_key, public_key_x, public_key_y);
      } else if (b.ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaNistP384)) {
          Ieee1609Dot2BaseTypes::EccP384CurvePoint &p = b.ecdsaNistP384();
          fill_public_key_vectors(ec_elliptic_curves::nist_p_384, p, public_comp_key, public_key_x, public_key_y);
      } else if (b.ischosen(Ieee1609Dot2BaseTypes::PublicVerificationKey::ALT_ecdsaBrainpoolP256r1)) {
        Ieee1609Dot2BaseTypes::EccP256CurvePoint &p = b.ecdsaBrainpoolP256r1();
        fill_public_key_vectors(ec_elliptic_curves::brainpool_p_256_r1, p, public_comp_key, public_key_x, public_key_y);
      } else { // ALT_ecdsaBrainpoolP384r1
        Ieee1609Dot2BaseTypes::EccP384CurvePoint &p = b.ecdsaBrainpoolP384r1();
        fill_public_key_vectors(ec_elliptic_curves::brainpool_p_384_r1, p, public_comp_key, public_key_x, public_key_y);
      }
      // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_key_x: ", public_key_x);
      // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_key_y: ", public_key_y);
      // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_comp_key: ", public_comp_key);

      OCTETSTRING public_enc_key_x;
      OCTETSTRING public_enc_key_y;
      OCTETSTRING public_enc_comp_key;
      if (decoded_certificate.toBeSigned().encryptionKey().ispresent()) {
        Ieee1609Dot2BaseTypes::PublicEncryptionKey &v =
          static_cast<Ieee1609Dot2BaseTypes::PublicEncryptionKey &>(decoded_certificate.toBeSigned().encryptionKey());
        Ieee1609Dot2BaseTypes::BasePublicEncryptionKey &b = v.publicKey();
        if (b.ischosen(Ieee1609Dot2BaseTypes::BasePublicEncryptionKey::ALT_eciesNistP256)) {
          Ieee1609Dot2BaseTypes::EccP256CurvePoint &p = v.publicKey().eciesNistP256();
          fill_public_key_vectors(ec_elliptic_curves::nist_p_256, p, public_enc_comp_key, public_enc_key_x, public_enc_key_y);
        } else if (b.ischosen(Ieee1609Dot2BaseTypes::BasePublicEncryptionKey::ALT_eciesBrainpoolP256r1)) {
          Ieee1609Dot2BaseTypes::EccP256CurvePoint &p = v.publicKey().eciesBrainpoolP256r1();
          fill_public_key_vectors(ec_elliptic_curves::brainpool_p_256_r1, p, public_enc_comp_key, public_enc_key_x, public_enc_key_y);
        }
        // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_enc_key_x: ", public_enc_key_x);
        // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_enc_key_y: ", public_enc_key_y);
        // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: public_enc_comp_key: ", public_enc_comp_key);
      } else {
        public_enc_key_x    = OCTETSTRING(0, nullptr);
        public_enc_key_y    = OCTETSTRING(0, nullptr);
        public_enc_comp_key = OCTETSTRING(0, nullptr);
      }

      OCTETSTRING hash_sha_256; // Whole-certificate hash using SHA-256
      sha256      sha;
      sha.generate(certificate, hash_sha_256);
      loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Whole-certificate SHA-256 hash for encryption: ", hash_sha_256);

      OCTETSTRING hash;                    // Whole-certificate hash
      OCTETSTRING hashed_id;               // Whole-certificate hashedid-8
      OCTETSTRING issuer;                  // Certificate issuer
      if (public_key_x.lengthof() == 32) { // See IEEE Std 1609.2a-2017 Clause 6.4.3 CertificateBase
        hash = hash_sha_256;
        loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Whole-certificate SHA-256 hash: ", hash);
        hashed_id = OCTETSTRING(8, static_cast<const unsigned char *>(hash) + hash.lengthof() - 8);
      } else if (public_key_x.lengthof() == 48) {
        sha384 sha;
        sha.generate(certificate, hash);
        loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Whole-certificate SHA-384 hash: ", hash);
        hashed_id = OCTETSTRING(8, static_cast<const unsigned char *>(hash) + hash.lengthof() - 8);
      } else {
        hash         = OCTETSTRING(0, nullptr);
        hash_sha_256 = OCTETSTRING(0, nullptr);
        hashed_id    = int2oct(0, 8);
      }
      loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: hash: ", hash);
      loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: hashed_id: ", hashed_id);

      if (decoded_certificate.issuer().ischosen(Ieee1609Dot2::IssuerIdentifier::ALT_sha256AndDigest)) {
        issuer = decoded_certificate.issuer().sha256AndDigest();
      } else if (decoded_certificate.issuer().ischosen(Ieee1609Dot2::IssuerIdentifier::ALT_sha384AndDigest)) {
        issuer = decoded_certificate.issuer().sha384AndDigest();
      } else if (decoded_certificate.issuer().ischosen(Ieee1609Dot2::IssuerIdentifier::ALT_self__)) {
        OCTETSTRING h;
        if (decoded_certificate.issuer().self__() == Ieee1609Dot2BaseTypes::HashAlgorithm::sha256) {
          sha256 sha;
          sha.generate(certificate, h);
          // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Whole-certificate SHA-256 hash: ", hash);
        } else {
          sha384 sha;
          sha.generate(certificate, h);
          // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: Whole-certificate SHA-384 hash: ", hash);
        }
        issuer = OCTETSTRING(8, static_cast<const unsigned char *>(h) + h.lengthof() - 8);
      } else {
        issuer = int2oct(0, 8);
      }
      loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: issuer: ", issuer);

      // Create new record
      security_db_record *p = new security_db_record(
        key,
        certificate, // Certificate
        decoded_certificate,
        issuer, // Hashed ID fo the issuer
        hash, hash_sha_256,
        hashed_id,          // Hashed ID
        private_key,        // Private key
        public_key_x,       // public keys X-coordinate
        public_key_y,       // public keys Y-coordinate
        public_comp_key,    // public compressed key, 33 or 49 bytes length, byte #0 indicating compressed-y-0 (0x02) or compressed-y-1 (0x03)
        private_enc_key,    // Private encryption key
        public_enc_key_x,   // Public encryption key X-coordinate
        public_enc_key_y,   // Public encryption key Y-coordinate
        public_enc_comp_key // Public compressed encryption key
      );
      result = p_certificates.insert(std::pair<const std::string, std::unique_ptr<security_db_record>>(key, std::unique_ptr<security_db_record>(p)));
      if (result.second == false) {
        loggers::get_instance().warning("certificates_loader::build_certificates_cache: Failed to insert new record '%s'", key.c_str());
        delete p;
        return -1;
      }
      // loggers::get_instance().log_msg("certificates_loader::build_certificates_cache: add key: ", result.first->second.get()->hashed_id());
      // loggers::get_instance().log("certificates_loader::build_certificates_cache: for : %s", result.first->first.c_str());
      p_hashed_id8s.insert(std::pair<OCTETSTRING, std::string>(result.first->second.get()->hashed_id(), result.first->first));
      /*std::vector<unsigned char> v(static_cast<const unsigned char*>(result.first->second.get()->hashed_id()),
      result.first->second.get()->hashed_id().lengthof() + static_cast<const unsigned char*>(result.first->second.get()->hashed_id()));
      loggers::get_instance().log_to_hexa("security_cache::store_certificate: Converted hashed_id8: ", v.data(), v.size());
      p_hashed_id8s.insert(std::pair<std::vector<unsigned char>, std::string>(v, result.first->first));*/
    }
    // Reset pointer
    it = p_files.cbegin();
  } while (it != p_files.cend());

  return 0;
} // End of method build_certificates_cache

int certificates_loader::save_certificate(const security_db_record &p_certificate) {
  loggers::get_instance().log(">>> certificates_loader::save_certificate: certificate_id=%s", p_certificate.certificate_id().c_str());
  loggers::get_instance().log_msg("certificates_loader::save_certificate: hash=", p_certificate.hash());
  loggers::get_instance().log_msg("certificates_loader::save_certificate: hashedid8=", p_certificate.hashed_id());

  // Certificate file
  std::experimental::filesystem::path p(_full_path);
  p /= p_certificate.certificate_id();
  p += _certificateExt;
  if (std::experimental::filesystem::exists(p)) {
    std::experimental::filesystem::remove(p);
  }
  loggers::get_instance().log("certificates_loader::save_certificate: Certificate file: '%s'", p.string().c_str());
  std::ofstream os(p.string(), ios::out | ios::binary);
  os.write((char *)static_cast<const unsigned char *>(p_certificate.certificate()), p_certificate.certificate().lengthof());
  os.close();
  std::experimental::filesystem::permissions(p, std::experimental::filesystem::perms::add_perms | std::experimental::filesystem::perms::owner_all |
                                                  std::experimental::filesystem::perms::group_all | std::experimental::filesystem::perms::others_all);

  // Private key
  p = _full_path;
  p /= p_certificate.certificate_id();
  p += _privateKeyExt;
  if (std::experimental::filesystem::exists(p)) {
    std::experimental::filesystem::remove(p);
  }
  loggers::get_instance().log("certificates_loader::save_certificate: Private key file: '%s'", p.string().c_str());
  os.open(p.string(), ios::out | ios::binary);
  loggers::get_instance().log_msg("certificates_loader::save_certificate: Private key content: ", p_certificate.private_key());
  os.write((char *)static_cast<const unsigned char *>(p_certificate.private_key()), p_certificate.private_key().lengthof());
  os.close();
  std::experimental::filesystem::permissions(p, std::experimental::filesystem::perms::add_perms | std::experimental::filesystem::perms::owner_all |
                                                  std::experimental::filesystem::perms::group_all | std::experimental::filesystem::perms::others_all);

  // Private encryption key
  if (p_certificate.private_enc_key().lengthof() != 0) {
    p = _full_path;
    p /= p_certificate.certificate_id();
    p += _privateEncKeyExt;
    if (std::experimental::filesystem::exists(p)) {
      std::experimental::filesystem::remove(p);
    }
    loggers::get_instance().log("certificates_loader::save_certificate: Private encryption key file: '%s'", p.string().c_str());
    os.open(p.string(), ios::out | ios::binary);
    os.write((char *)static_cast<const unsigned char *>(p_certificate.private_enc_key()), p_certificate.private_enc_key().lengthof());
    os.close();
    std::experimental::filesystem::permissions(p, std::experimental::filesystem::perms::add_perms | std::experimental::filesystem::perms::owner_all |
                                                    std::experimental::filesystem::perms::group_all | std::experimental::filesystem::perms::others_all);
  }

  return 0;
} // End of method save_certificate

void certificates_loader::fill_public_key_vectors(const ec_elliptic_curves p_elliptic_curve, const Ieee1609Dot2BaseTypes::EccP256CurvePoint &p_ecc_point,
                                                  OCTETSTRING &p_public_comp_key, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y) {
  loggers::get_instance().log_msg(">>> certificates_loader::fill_public_key_vectors: ", p_ecc_point);

  if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__0)) {
    p_public_comp_key = int2oct(2, 1) + p_ecc_point.compressed__y__0();
    security_ecc ecc(p_elliptic_curve, p_ecc_point.compressed__y__0(), ecc_compressed_mode::compressed_y_0);
    p_public_key_x = ecc.public_key_x();
    p_public_key_y = ecc.public_key_y();
  } else if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_compressed__y__1)) {
    p_public_comp_key = int2oct(3, 1) + p_ecc_point.compressed__y__1();
    security_ecc ecc(p_elliptic_curve, p_ecc_point.compressed__y__1(), ecc_compressed_mode::compressed_y_1);
    p_public_key_x = ecc.public_key_x();
    p_public_key_y = ecc.public_key_y();
  } else if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP256CurvePoint::ALT_uncompressedP256)) {
    p_public_key_x    = p_ecc_point.uncompressedP256().x();
    p_public_key_y    = p_ecc_point.uncompressedP256().y();
    p_public_comp_key = int2oct(0, 33);
  } else {
    p_public_key_x    = int2oct(0, 32);
    p_public_key_y    = int2oct(0, 32);
    p_public_comp_key = int2oct(0, 33);
  }
}
void certificates_loader::fill_public_key_vectors(const ec_elliptic_curves p_elliptic_curve, const Ieee1609Dot2BaseTypes::EccP384CurvePoint &p_ecc_point,
                                                  OCTETSTRING &p_public_comp_key, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y) {
  // loggers::get_instance().log_msg(">>> certificates_loader::fill_public_key_vectors: ", p_ecc_point);

  if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__0)) {
    p_public_comp_key = int2oct(2, 1) + p_ecc_point.compressed__y__0();
    security_ecc ecc(p_elliptic_curve, p_ecc_point.compressed__y__0(), ecc_compressed_mode::compressed_y_0);
    p_public_key_x = ecc.public_key_x();
    p_public_key_y = ecc.public_key_y();
  } else if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_compressed__y__1)) {
    p_public_comp_key = int2oct(3, 1) + p_ecc_point.compressed__y__1();
    security_ecc ecc(p_elliptic_curve, p_ecc_point.compressed__y__1(), ecc_compressed_mode::compressed_y_1);
    p_public_key_x = ecc.public_key_x();
    p_public_key_y = ecc.public_key_y();
  } else if (p_ecc_point.ischosen(Ieee1609Dot2BaseTypes::EccP384CurvePoint::ALT_uncompressedP384)) {
    p_public_key_x    = p_ecc_point.uncompressedP384().x();
    p_public_key_y    = p_ecc_point.uncompressedP384().y();
    p_public_comp_key = int2oct(0, 49);
  } else {
    p_public_key_x    = int2oct(0, 48);
    p_public_key_y    = int2oct(0, 48);
    p_public_comp_key = int2oct(0, 49);
  }
}
