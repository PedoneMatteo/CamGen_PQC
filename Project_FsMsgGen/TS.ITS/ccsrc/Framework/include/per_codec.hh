/*!
 * \file      per_codec.hh
 * \brief     Header file for TITAN message to ASN.1 PER message codec.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "asn1_recode_per.hh"
#include "params.hh"

class BITSTRING;             //! Forward declaration of TITAN class
class TTCN_Typedescriptor_t; //! Forward declaration of TITAN class

struct asn_TYPE_descriptor_s; //! Declare asn1c class

/*!
 * \class per_codec
 * \brief  This class provides the interface for all ASN.1 PER codecs.
 * \remark This class uses asn1c external tool
 */
template <typename TPDU> class per_codec : public asn1_recode_per {
public: //! \publicsection
  /*!
   * \fn int encode(const TPDU& p_message, BITSTRING& p_bitstring);
   * \brief Encode TITAN message into ASN.1 PER message
   * \param[in]  p_message   The PDU message to encode
   * \param[out] p_bitstring  The encoded PDU message in bit string format
   * \pure
   */
  virtual int encode(const TPDU &p_message, BITSTRING &p_bitstring) = 0;
  /*!
   * \fn int decode(const BITSTRING& p_bitstring, TPDU& p_message);
   * \brief Decode ASN.1 PER message into TITAN message
   * \param[in] p_bitstring  The network message in bit string format to decode
   * \param[out] p_message   The PDU message
   * \pure
   */
  virtual int decode(const BITSTRING &p_bitstring, TPDU &p_message) = 0;

protected: //! \protectedsection
  int _decode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const BITSTRING &p_data, TPDU &msg);
  int _encode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const TPDU &msg, BITSTRING &p_data);
}; // End of class per_codec

#include "per_codec.t.hh"
