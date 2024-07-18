#include "cam_pdu_codec.hh"
#include "LibItsCam_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

extern "C" {
extern asn_TYPE_descriptor_t asn_DEF_CAM;
}

int cam_pdu_codec::encode(const CAM__PDU__Descriptions::CAM &p_cam, BITSTRING &p_data) {
  return _encode(CAM__PDU__Descriptions::CAM_descr_, asn_DEF_CAM, p_cam, p_data);
}

int cam_pdu_codec::decode(const BITSTRING &p_data, CAM__PDU__Descriptions::CAM &p_cam) {
  return _decode(CAM__PDU__Descriptions::CAM_descr_, asn_DEF_CAM, p_data, p_cam);
}
