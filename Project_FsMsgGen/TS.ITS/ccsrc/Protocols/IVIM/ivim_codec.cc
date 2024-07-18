#include "ivim_codec.hh"
#include "LibItsIvim_TypesAndValues.hh"
#include "asn1/asn_application.h" // from asn1c
#include "loggers.hh"

int ivim_codec::encode(const IVIM__PDU__Descriptions::IVIM &ivim, OCTETSTRING &data) {
  loggers::get_instance().log(">>> ivim_codec::encode");
  BITSTRING b;
  int       rc = asn_codec.encode(ivim, b);
  loggers::get_instance().log("ivim_codec::encode: ASN.1 codec returned %d", rc);
  if (rc) {
    data = bit2oct(b);
    loggers::get_instance().log_msg("ivim_codec::encode: ", data);
  }
  return rc;
}

int ivim_codec::decode(const OCTETSTRING &data, IVIM__PDU__Descriptions::IVIM &ivim, params *params) {
  loggers::get_instance().log_msg(">>> ivim_codec::decode: ", data);

  int rc = asn_codec.decode(oct2bit(data), ivim);
  loggers::get_instance().log("ivim_codec::decode: ASN.1 codec returned %d", rc);
  if (rc) {
    loggers::get_instance().log_msg("ivim_codec::decode: ", ivim);
  }
  return rc;
}
