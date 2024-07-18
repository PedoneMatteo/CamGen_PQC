#include "SremCodec.hh"
#include "LibItsSremSsem_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int SremCodec::encode(const SREM__PDU__Descriptions::SREM &srem, OCTETSTRING &data) {
  loggers::get_instance().log(">>> SremCodec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(srem, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("SremCodec::encode: ", data);
  }
  return rc;
}

int SremCodec::decode(const OCTETSTRING &data, SREM__PDU__Descriptions::SREM &srem, params *params) {
  loggers::get_instance().log_msg(">>> SremCodec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), srem);
  loggers::get_instance().log("SremCodec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("SremCodec::decode: ", srem);
  }
  return rc;
}

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_SREM;
}

int SremPDUCodec::encode(const SREM__PDU__Descriptions::SREM &p_srem, BITSTRING &p_data) {
  return _encode(SREM__PDU__Descriptions::SREM_descr_, asn_DEF_SREM, p_srem, p_data);
}

int SremPDUCodec::decode(const BITSTRING &p_data, SREM__PDU__Descriptions::SREM &p_srem) {
  return _decode(SREM__PDU__Descriptions::SREM_descr_, asn_DEF_SREM, p_data, p_srem);
}
