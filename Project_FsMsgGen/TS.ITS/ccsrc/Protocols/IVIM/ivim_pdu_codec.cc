#include "LibItsIvim_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "ivim_codec.hh"
#include "loggers.hh"

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_IVIM;
}

int ivim_pdu_codec::encode(const IVIM__PDU__Descriptions::IVIM &p_ivim, BITSTRING &p_data) {
  return _encode(IVIM__PDU__Descriptions::IVIM_descr_, asn_DEF_IVIM, p_ivim, p_data);
}

int ivim_pdu_codec::decode(const BITSTRING &p_data, IVIM__PDU__Descriptions::IVIM &p_ivim) {
  return _decode(IVIM__PDU__Descriptions::IVIM_descr_, asn_DEF_IVIM, p_data, p_ivim);
}
