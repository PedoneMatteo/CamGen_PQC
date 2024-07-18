#include "LibItsMapemSpatem_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"
#include "mapem_codec.hh"

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_MAPEM;
}

int mapem_pdu_codec::encode(const MAPEM__PDU__Descriptions::MAPEM &p_mapem, BITSTRING &p_data) {
  return _encode(MAPEM__PDU__Descriptions::MAPEM_descr_, asn_DEF_MAPEM, p_mapem, p_data);
}

int mapem_pdu_codec::decode(const BITSTRING &p_data, MAPEM__PDU__Descriptions::MAPEM &p_mapem) {
  return _decode(MAPEM__PDU__Descriptions::MAPEM_descr_, asn_DEF_MAPEM, p_data, p_mapem);
}
