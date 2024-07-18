#include "mapem_codec.hh"
#include "LibItsMapemSpatem_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int mapem_codec::encode(const MAPEM__PDU__Descriptions::MAPEM &mapem, OCTETSTRING &data) {
  loggers::get_instance().log(">>> mapem_codec::encode");

  BITSTRING b;
  int       rc = asn_codec.encode(mapem, b);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("mapem_codec::encode: ", data);
  }
  return rc;
}

int mapem_codec::decode(const OCTETSTRING &data, MAPEM__PDU__Descriptions::MAPEM &mapem, params *params) {
  loggers::get_instance().log_msg(">>> mapem_codec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), mapem);
  loggers::get_instance().log("mapem_codec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("mapem_codec::decode: ", mapem);
  }
  return rc;
}
