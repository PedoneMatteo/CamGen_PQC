#include "denm_codec.hh"
#include "LibItsDenm_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int denm_codec::encode(const DENM__PDU__Descriptions::DENM &p_denm, OCTETSTRING &p_data) {
  loggers::get_instance().log_msg(">>> denm_codec::encode: ", p_denm);

  BITSTRING b;
  int       rc = asn_codec.encode(p_denm, b);
  if (rc) {
    p_data = bit2oct(b);
  }
  loggers::get_instance().log("<<< denm_codec::encode: %d", rc);
  return rc;
}

int denm_codec::decode(const OCTETSTRING &p_data, DENM__PDU__Descriptions::DENM &p_denm, params *params) {
  loggers::get_instance().log_msg(">>> denm_codec::decode:", p_data);

  int rc = asn_codec.decode(oct2bit(p_data), p_denm);
  loggers::get_instance().log("denm_codec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("denm_codec::decode: ", p_denm);
  }
  return rc;
}
