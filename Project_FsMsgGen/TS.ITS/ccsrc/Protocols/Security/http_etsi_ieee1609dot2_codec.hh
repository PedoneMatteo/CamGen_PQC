#pragma once

#include "etsi_ts103097_data_codec.hh"
#include "etsi_ts103097_certificate_codec.hh"

#include "params.hh"

namespace LibItsHttp__BinaryMessageBodyTypes {
  class BinaryBody;
}

class http_etsi_ieee1609dot2_codec : public codec<Ieee1609Dot2::Ieee1609Dot2Data, Ieee1609Dot2::Ieee1609Dot2Data> {
  etsi_ts103097_data_codec _codec;
  etsi_ts103097_certificate_codec _codec_cert;

public:
  explicit http_etsi_ieee1609dot2_codec() : codec<Ieee1609Dot2::Ieee1609Dot2Data, Ieee1609Dot2::Ieee1609Dot2Data>(), _codec(), _codec_cert(){};
  virtual ~http_etsi_ieee1609dot2_codec(){};

  virtual int encode(const Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2::Ieee1609Dot2Data &p_ieee_1609dot2_data, params *params = NULL);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2::CertificateBase &p_ieee_1609dot2_certificate, params *params = NULL);
};
