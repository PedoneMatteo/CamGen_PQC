/*!
 * \file      asn1_recode_per.hh
 * \brief     Header file ASN.1 PER codec based on asn1c external tool.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

class OCTETSTRING;           //! Forward declaration of TITAN class
class CHARSTRING;            //! Forward declaration of TITAN class
class BITSTRING;             //! Forward declaration of TITAN class
class TTCN_Buffer;           //! Forward declaration of TITAN class
class TTCN_EncDec;           //! Forward declaration of TITAN class
class TTCN_Typedescriptor_t; //! Forward declaration of TITAN class

struct asn_TYPE_descriptor_s; //! Forward declaration of asn1c class

class asn1_recode_per {
protected:
  int ber2per(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf);
  int per2ber(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf);
  int recode(const asn_TYPE_descriptor_s &td, int from, int to, TTCN_Buffer &buf);
}; // End of class asn1_recode_per
