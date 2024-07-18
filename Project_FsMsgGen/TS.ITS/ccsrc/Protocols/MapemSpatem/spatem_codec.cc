#include "spatem_codec.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"
#include "mapem_spatem_types.hh"

int spatem_codec::encode(const SPATEM__PDU__Descriptions::SPATEM &spatem, OCTETSTRING &data) {
  loggers::get_instance().log(">>> spatem_codec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(spatem, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("spatem_codec::encode: ", data);
  }
  return rc;
}

int spatem_codec::decode(const OCTETSTRING &data, SPATEM__PDU__Descriptions::SPATEM &spatem, params *params) {
  loggers::get_instance().log_msg(">>> spatem_codec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), spatem);
  loggers::get_instance().log("spatem_codec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("spatem_codec::decode: ", spatem);
  }
  return rc;
}
