#include "EtsiTs103097Module.hh"

#include "http_etsi_ieee1609dot2_codec_factory.hh"

#include "LibItsHttp_MessageBodyTypes.hh"

#include "security_services.hh"

#include "loggers.hh"

int http_etsi_ieee1609dot2_codec::encode(const Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data, OCTETSTRING &p_data) {
  loggers::get_instance().log_msg(">>> http_etsi_ieee1609dot2_codec::encode: ", p_ieee_1609dot2_data);

  // Encode the Ieee1609Dot2Data
  if (_codec.encode(p_ieee_1609dot2_data, p_data) == -1) {
    loggers::get_instance().warning("http_etsi_ieee1609dot2_codec::encode: Failed to encode InnerEcRequest");
    return -1;
  }

  loggers::get_instance().log_msg("<<< http_etsi_ieee1609dot2_codec::encode: ", p_data);
  return 0;
}

int http_etsi_ieee1609dot2_codec::decode(const OCTETSTRING &p_data, Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data, params *p_params) {
  loggers::get_instance().log_msg(">>> http_etsi_ieee1609dot2_codec::decode (1): ", p_data);

  if (_codec.decode(p_data, p_ieee_1609dot2_data, p_params) == -1) {
    loggers::get_instance().warning("http_etsi_ieee1609dot2_codec::decode (1): Failed to decode data");
    return -1;
  }

  loggers::get_instance().log_msg("<<< http_etsi_ieee1609dot2_codec::decode (1): ", p_ieee_1609dot2_data);
  return 0;
}

int http_etsi_ieee1609dot2_codec::decode(const OCTETSTRING &p_data, Ieee1609Dot2::CertificateBase &p_ieee_1609dot2_certificate, params *p_params) {
  loggers::get_instance().log_msg(">>> http_etsi_ieee1609dot2_codec::decode (2): ", p_data);

  if (_codec_cert.decode(p_data, p_ieee_1609dot2_certificate, p_params) == -1) {
    loggers::get_instance().warning("http_etsi_ieee1609dot2_codec::decode (2): Failed to decode data");
    return -1;
  }

  loggers::get_instance().log_msg("<<< http_etsi_ieee1609dot2_codec::decode (2): ", p_ieee_1609dot2_certificate);
  return 0;
}

http_etsi_ieee1609dot2_codec_factory http_etsi_ieee1609dot2_codec_factory::_f;
