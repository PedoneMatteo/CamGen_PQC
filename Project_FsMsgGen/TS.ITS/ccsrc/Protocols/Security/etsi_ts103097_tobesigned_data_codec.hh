#pragma once

#include "codec.hh"
//#include "CodecOer.hh"
#include "params.hh"

#include "EtsiTs103097Module.hh"
// class BITSTRING;
// class OCTETSTRING;
// class Ieee1609Dot2::ToBeSignedData;

// class EtsiTs103097PDUCodec// : public oer_codec <Ieee1609Dot2::ToBeSignedData>
// {
// public:
//   explicit EtsiTs103097PDUCodec() { };
//   virtual int encode (const Ieee1609Dot2::ToBeSignedData& p_cert, BITSTRING& data);
//   virtual int decode (const BITSTRING& data, Ieee1609Dot2::ToBeSignedData&);
// };

class etsi_ts103097_tobesigned_data_codec : public codec<Ieee1609Dot2::ToBeSignedData, Ieee1609Dot2::ToBeSignedData> {
  //  EtsiTs103097PDUCodec asn_codec;

public:
  // explicit etsi_ts103097_tobesigned_data_codec() : codec<Ieee1609Dot2::ToBeSignedData, Ieee1609Dot2::ToBeSignedData>(), asn_codec() { };
  explicit etsi_ts103097_tobesigned_data_codec() : codec<Ieee1609Dot2::ToBeSignedData, Ieee1609Dot2::ToBeSignedData>(){};
  virtual ~etsi_ts103097_tobesigned_data_codec(){};

  virtual int encode(const Ieee1609Dot2::ToBeSignedData &p_cert, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2::ToBeSignedData &p_cert, params *p_params = NULL);
};
