#include "RtcmemCodec.hh"
#include "LibItsRtcmem_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int RtcmemCodec::encode(const RTCMEM__PDU__Descriptions::RTCMEM &rtcmem, OCTETSTRING &data) {
  loggers::get_instance().log(">>> RtcmemCodec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(rtcmem, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("RtcmemCodec::encode: ", data);
  }
  return rc;
}

int RtcmemCodec::decode(const OCTETSTRING &data, RTCMEM__PDU__Descriptions::RTCMEM &p_rtcmem, params *params) {
  loggers::get_instance().log_msg(">>> RtcmemCodec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), p_rtcmem);
  loggers::get_instance().log("RtcmemCodec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("RtcmemCodec::decode: ", p_rtcmem);
  }
  return rc;
}

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_RTCMEM;
}

int RtcmemPDUCodec::encode(const RTCMEM__PDU__Descriptions::RTCMEM &p_rtcmem, BITSTRING &p_data) {
  return _encode(RTCMEM__PDU__Descriptions::RTCMEM_descr_, asn_DEF_RTCMEM, p_rtcmem, p_data);
}

int RtcmemPDUCodec::decode(const BITSTRING &p_data, RTCMEM__PDU__Descriptions::RTCMEM &p_rtcmem) {
  return _decode(RTCMEM__PDU__Descriptions::RTCMEM_descr_, asn_DEF_RTCMEM, p_data, p_rtcmem);
}
