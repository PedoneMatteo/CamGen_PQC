#include "denm_pdu_codec.hh"
#include "LibItsDenm_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_DENM;
}

int denm_pdu_codec::encode(const DENM__PDU__Descriptions::DENM &p_denm, BITSTRING &p_data) {
  return _encode(DENM__PDU__Descriptions::DENM_descr_, asn_DEF_DENM, p_denm, p_data);
}

int denm_pdu_codec::decode(const BITSTRING &p_data, DENM__PDU__Descriptions::DENM &p_denm) {
  return _decode(DENM__PDU__Descriptions::DENM_descr_, asn_DEF_DENM, p_data, p_denm);
}
