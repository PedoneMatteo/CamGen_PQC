#include "SsemCodec.hh"
#include "LibItsSremSsem_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int SsemCodec::encode(const SSEM__PDU__Descriptions::SSEM &ssem, OCTETSTRING &data) {
  loggers::get_instance().log(">>> SsemCodec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(ssem, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("SsemCodec::encode: ", data);
  }
  return rc;
}

int SsemCodec::decode(const OCTETSTRING &data, SSEM__PDU__Descriptions::SSEM &ssem, params *params) {
  loggers::get_instance().log_msg(">>> SsemCodec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), ssem);
  loggers::get_instance().log("SsemCodec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("SsemCodec::decode: ", ssem);
  }
  return rc;
}

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_SSEM;
}

int SsemPDUCodec::encode(const SSEM__PDU__Descriptions::SSEM &p_ssem, BITSTRING &p_data) {
  return _encode(SSEM__PDU__Descriptions::SSEM_descr_, asn_DEF_SSEM, p_ssem, p_data);
}

int SsemPDUCodec::decode(const BITSTRING &p_data, SSEM__PDU__Descriptions::SSEM &p_ssem) {
  return _decode(SSEM__PDU__Descriptions::SSEM_descr_, asn_DEF_SSEM, p_data, p_ssem);
}
