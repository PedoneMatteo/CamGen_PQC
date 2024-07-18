#include <TTCN3.hh>

template <class TPDU> int per_codec<TPDU>::_decode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const BITSTRING &p_data, TPDU &msg) {
  TTCN_Buffer buf(bit2oct(p_data));
  TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_WARNING);
  int rc = per2ber(td, buf);
  if (rc > 0) {
    msg.decode(ttcn, buf, TTCN_EncDec::CT_BER, BER_ACCEPT_ALL);
    rc = buf.get_len();
  }
  return rc;
}

template <class TPDU> int per_codec<TPDU>::_encode(const TTCN_Typedescriptor_t &ttcn, const asn_TYPE_descriptor_s &td, const TPDU &msg, BITSTRING &p_data) {
  int         rc = -1;
  TTCN_Buffer buf;
  TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_WARNING);
  msg.encode(ttcn, buf, TTCN_EncDec::CT_BER, BER_ENCODE_DER);
  if (buf.get_len() > 0) {
    rc = ber2per(td, buf);
    if (rc > 0) {
      p_data = oct2bit(OCTETSTRING(buf.get_len(), buf.get_data()));
    }
  }
  return rc;
}
