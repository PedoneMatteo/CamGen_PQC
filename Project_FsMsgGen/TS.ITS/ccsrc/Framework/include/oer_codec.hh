#pragma once

#include "params.hh"

class OCTETSTRING;
class CHARSTRING;
class BITSTRING;

struct asn_TYPE_descriptor_s;
class asn1_recode_oer {
protected:
  int xer2oer(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf);
  int oer2xer(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf);
  int recode(const asn_TYPE_descriptor_s &td, int from, int to, TTCN_Buffer &buf);
};

template <typename TPDU> class oer_codec : public asn1_recode_oer {
public:
  virtual int encode(const TPDU &msg, BITSTRING &bits) = 0;
  virtual int decode(const BITSTRING &bits, TPDU &msg) = 0;

protected:
  inline int _decode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const BITSTRING &p_data, TPDU &msg) {
    TTCN_Buffer buf(bit2oct(p_data));
    TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_WARNING);
    int rc = oer2xer(td, buf);
    if (rc > 0) {
      msg.decode(ttcn, buf, TTCN_EncDec::CT_BER, BER_ACCEPT_ALL);
      rc = buf.get_len();
    }
    return rc;
  }
  inline int _encode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const TPDU &msg, BITSTRING &p_data) {
    int         rc = -1;
    TTCN_Buffer buf;
    TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_WARNING);
    msg.encode(ttcn, buf, TTCN_EncDec::CT_BER, BER_ENCODE_DER);
    if (buf.get_len() > 0) {
      rc = xer2oer(td, buf);
      if (rc > 0) {
        p_data = oct2bit(OCTETSTRING(buf.get_len(), buf.get_data()));
      }
    }
    return rc;
  }
};

#endif
