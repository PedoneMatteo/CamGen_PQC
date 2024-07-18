#pragma once

#include "codec.hh"
//#include "CodecOer.hh"
#include "params.hh"

#include "EtsiTs103097Module.hh"
// class BITSTRING;
// class OCTETSTRING;
// class Ieee1609Dot2::Ieee1609Dot2Data;

// class EtsiTs103097PDUCodec// : public oer_codec <Ieee1609Dot2::Ieee1609Dot2Data>
// {
// public:
//   explicit EtsiTs103097PDUCodec() { };
//   virtual int encode (const Ieee1609Dot2::Ieee1609Dot2Data& p_cert, BITSTRING& data);
//   virtual int decode (const BITSTRING& data, Ieee1609Dot2::Ieee1609Dot2Data&);
// };

class etsi_ts103097_data_codec : public codec<Ieee1609Dot2::Ieee1609Dot2Data, Ieee1609Dot2::Ieee1609Dot2Data> {
  //  EtsiTs103097PDUCodec asn_codec;

public:
  // explicit etsi_ts103097_data_codec() : codec<Ieee1609Dot2::Ieee1609Dot2Data, Ieee1609Dot2::Ieee1609Dot2Data>(), asn_codec() { };
  explicit etsi_ts103097_data_codec() : codec<Ieee1609Dot2::Ieee1609Dot2Data, Ieee1609Dot2::Ieee1609Dot2Data>(){};
  virtual ~etsi_ts103097_data_codec(){};

  virtual int encode(const Ieee1609Dot2::Ieee1609Dot2Data &p_ieee1609Dot2Data, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2::Ieee1609Dot2Data &p_ieee1609Dot2Data, params *params = NULL);
};
