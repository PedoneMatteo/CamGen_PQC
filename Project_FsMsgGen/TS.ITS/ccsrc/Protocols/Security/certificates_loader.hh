/*!
 * \file      certificate_loader.hh
 * \brief     Header file for ITS certificates loader definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 * \remark Use xxd -p -c32 <file name> to make a hexdump or do the reverse (e.g. xxd -p -c32 CERT_IUT_A_RCA.vkey)
 */
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include <experimental/filesystem>

#include "security_cache.hh"
#include "security_db_record.hh"
#include "security_ecc.hh"

namespace Ieee1609Dot2BaseTypes {
  class EccP256CurvePoint; //! Declare TITAN class
  class EccP384CurvePoint; //! Declare TITAN class
} // namespace Ieee1609Dot2BaseTypes

/*!
 * \class certificates_loader
 * \brief This class provides mechanism  to load the certificates from the filesystem according the struecture defined in ETSI TS 103 099
 * \remark Singleton pattern
 */
class certificates_loader {
  std::string                         _certificateExt;       //! COER encoding certificate extension. Default: .oer
  std::string                         _privateKeyExt;        //! Private signing key extension. Default: .vkey
  std::string                         _privateEncKeyExt;     //! Private cyphering key extension. Default: .ekey
  std::experimental::filesystem::path _full_path;            //! The full folder path to load certificates
  bool                                _is_cache_initialized; //! Set to true when certificates are successfully loaded from file system
  std::set<std::string>               _directory_filter;     //! Directory filter (for local development purposes only)
  static certificates_loader *        instance;              //! Unique static object reference of this class

  /*!
   * \brief Default private ctor
   */
  certificates_loader();
  /*!
   * \brief Default private dtor
   */
  ~certificates_loader() {
    if (instance != NULL) {
      delete instance;
      instance = NULL;
    }
  };

public: /*! \publicsection */
  /*!
   * \brief Public accessor to the single object reference
   */
  inline static certificates_loader &get_instance() {
    if (instance == NULL)
      instance = new certificates_loader();
    return *instance;
  };

  int build_path(const std::string &p_root_directory);

  int load_certificates(std::map<std::string, std::unique_ptr<security_db_record>> &                            p_certificates,
                        std::map<OCTETSTRING /*std::vector<unsigned char>*/, std::string, security_cache_comp> &p_hashed_id8s);
  // int load_certificate(std::unique_ptr<security_db_record> >& p_certificate, std::map<const OCTETSTRING, const std::string&>& p_hashed_id8s);
  int save_certificate(const security_db_record &p_certificate);

private:
  int retrieve_certificates_list(std::set<std::experimental::filesystem::path> &p_files);

  int build_certificates_cache(std::set<std::experimental::filesystem::path> &                                         p_files,
                               std::map<std::string, std::unique_ptr<security_db_record>> &                            p_certificates,
                               std::map<OCTETSTRING /*std::vector<unsigned char>*/, std::string, security_cache_comp> &p_hashed_id8s);

  void fill_public_key_vectors(const ec_elliptic_curves p_elliptic_curve, const Ieee1609Dot2BaseTypes::EccP256CurvePoint &p_ecc_point,
                               OCTETSTRING &p_public_comp_key, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y);
  void fill_public_key_vectors(const ec_elliptic_curves p_elliptic_curve, const Ieee1609Dot2BaseTypes::EccP384CurvePoint &p_ecc_point,
                               OCTETSTRING &p_public_comp_key, OCTETSTRING &p_public_key_x, OCTETSTRING &p_public_key_y);
}; // End of class certificates_loader
