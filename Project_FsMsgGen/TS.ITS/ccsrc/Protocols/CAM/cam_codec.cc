#include "cam_codec.hh"
#include "LibItsCam_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int cam_codec::encode(const CAM__PDU__Descriptions::CAM &cam, OCTETSTRING &data) {
  loggers::get_instance().log(">>> cam_codec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(cam, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("cam_codec::encode: ", data);
  }
  return rc;
}

int cam_codec::decode(const OCTETSTRING &data, CAM__PDU__Descriptions::CAM &cam, params *params) {
  loggers::get_instance().log_msg(">>> cam_codec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), cam);
  loggers::get_instance().log("cam_codec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("cam_codec::decode: ", cam);
  }
  return rc;
}
