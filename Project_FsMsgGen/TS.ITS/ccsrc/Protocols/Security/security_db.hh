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

#include "security_cache.hh"

class OCTETSTRING; //! TITAN forward declaration

/*!
 * \class security_db
 * \brief This class provides security record description for security database
 */
class security_db : public security_cache {

public: /*! \publicsection */
  /*!
   * \brief Ctor with full path to the certificates storage
   * \param[in] p_db_path The full path to the certificates storage
   */
  security_db(const std::string &p_db_path);
  /*!
   * \brief Default dtor
   */
  ~security_db();

  /*!
   * \fn int store_certificate(const CHARSTRING& p_cert_id, const OCTETSTRING& p_cert, const OCTETSTRING& p_private_key, const OCTETSTRING& p_public_key_x,
   * const OCTETSTRING& p_public_key_y, const OCTETSTRING& p_public_compressed_key, const INTEGER& p_public_compressed_key_mode, const OCTETSTRING& p_hashid8,
   * const OCTETSTRING& p_issuer, const OCTETSTRING& p_private_enc_key, const OCTETSTRING& p_public_enc_key_x, const OCTETSTRING& p_public_enc_key_y, const
   * OCTETSTRING& p_public_enc_compressed_key, const INTEGER& p_public_enc_compressed_key_mode); \brief Store a new certificate \param[in] p_cert_id The
   * certificate identifier \return 0 on success, -1 otherwise
   */
  int store_certificate(const CHARSTRING &p_cert_id, const OCTETSTRING &p_cert, const OCTETSTRING &p_private_key, const OCTETSTRING &p_public_key_x,
                        const OCTETSTRING &p_public_key_y, const OCTETSTRING &p_public_compressed_key, const INTEGER &p_public_compressed_key_mode,
                        const OCTETSTRING &p_hash, const OCTETSTRING &p_hash_sha_256, const OCTETSTRING &p_hashid8, const OCTETSTRING &p_issuer,
                        const OCTETSTRING &p_private_enc_key, const OCTETSTRING &p_public_enc_key_x, const OCTETSTRING &p_public_enc_key_y,
                        const OCTETSTRING &p_public_enc_compressed_key, const INTEGER &p_public_enc_compressed_key_mode);

private: /*! \privatesection */
  /*!
   * \fn int load_from_files(const std::string& p_db_path);
   * \brief Initialise the storage of certificates mechanism
   * \param[in] p_db_path The full path to the certificates storage
   * \return 0 on success, -1 otherwise
   */
  int load_from_files(const std::string &p_db_path);
}; // End of class security_db
