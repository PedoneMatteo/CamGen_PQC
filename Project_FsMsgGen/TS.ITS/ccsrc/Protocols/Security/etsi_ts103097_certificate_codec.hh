#pragma once

#include "codec.hh"
//#include "oer_codec.hh"
#include "params.hh"

#include "EtsiTs103097Module.hh"
// class BITSTRING;
// class OCTETSTRING;
// class Ieee1609Dot2::CertificateBase;

// class EtsiTs103097PDUCodec// : public oer_codec <Ieee1609Dot2::CertificateBase>
// {
// public:
//   explicit EtsiTs103097PDUCodec() { };
//   virtual int encode (const Ieee1609Dot2::CertificateBase& p_cert, BITSTRING& data);
//   virtual int decode (const BITSTRING& data, Ieee1609Dot2::CertificateBase&);
// };

class etsi_ts103097_certificate_codec : public codec<Ieee1609Dot2::CertificateBase, Ieee1609Dot2::CertificateBase> {
  //  EtsiTs103097PDUCodec asn_codec;

public:
  // explicit etsi_ts103097_certificate_codec() : codec<Ieee1609Dot2::CertificateBase, Ieee1609Dot2::CertificateBase>(), asn_codec() { };
  explicit etsi_ts103097_certificate_codec() : codec<Ieee1609Dot2::CertificateBase, Ieee1609Dot2::CertificateBase>(){};
  virtual ~etsi_ts103097_certificate_codec(){};

  virtual int encode(const Ieee1609Dot2::CertificateBase &p_cert, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2::CertificateBase &p_cert, params *p_params = NULL);
}; // End of class etsi_ts103097_certificate_codec
