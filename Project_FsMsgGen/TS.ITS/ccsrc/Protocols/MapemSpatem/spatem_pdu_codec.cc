#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"
#include "mapem_spatem_types.hh"
#include "spatem_codec.hh"

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_SPATEM;
}

int spatem_pdu_codec::encode(const SPATEM__PDU__Descriptions::SPATEM &p_spatem, BITSTRING &p_data) {
  return _encode(SPATEM__PDU__Descriptions::SPATEM_descr_, asn_DEF_SPATEM, p_spatem, p_data);
}

int spatem_pdu_codec::decode(const BITSTRING &p_data, SPATEM__PDU__Descriptions::SPATEM &p_spatem) {
  return _decode(SPATEM__PDU__Descriptions::SPATEM_descr_, asn_DEF_SPATEM, p_data, p_spatem);
}
