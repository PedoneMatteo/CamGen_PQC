#include "EtsiTs103097Module.hh"

#include "asn1/EtsiTs103097Certificate.h"
#include "asn1/asn_application.h" // from asn1c
#include "etsi_ts103097_certificate_codec.hh"

#include "loggers.hh"

int etsi_ts103097_certificate_codec::encode(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts103097_certificate_codec::encode: %s", p_cert.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_cert.encode(*p_cert.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts103097_certificate_codec::encode: ", p_data);

  return 0;
}

int etsi_ts103097_certificate_codec::decode(const OCTETSTRING &p_data, Ieee1609Dot2::CertificateBase &p_cert, params *params) {
  loggers::get_instance().log_msg(">>> etsi_ts103097_certificate_codec::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  _params = params;
  p_cert.decode(*p_cert.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts103097_certificate_codec::decode: ", (const Base_Type &)p_cert);
  return 0;
}

// extern "C" {
//   extern asn_TYPE_descriptor_t asn_DEF_EtsiTs103097;
// }

// int EtsiTs103097PDUCodec::encode (const Ieee1609Dot2::CertificateBase& p_cert, BITSTRING& p_p_data)
// {
//   return _encode(Ieee1609Dot2::CertificateBase_descr_, asn_DEF_EtsiTs103097Certificate, p_cert, p_p_data);
// }

// int EtsiTs103097PDUCodec::decode (const BITSTRING& p_p_data, Ieee1609Dot2::CertificateBase& p_cert)
// {
//   return _decode(Ieee1609Dot2::CertificateBase_descr_, asn_DEF_EtsiTs103097Certificate, p_p_data, p_cert);
// }
