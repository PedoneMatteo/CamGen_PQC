#pragma once

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "LibItsIvim_TypesAndValues.hh"
// class BITSTRING;
// class OCTETSTRING;
// class IVIM__PDU__Descriptions::IVIM;

class ivim_pdu_codec : public per_codec<IVIM__PDU__Descriptions::IVIM> {
public:
  explicit ivim_pdu_codec(){};
  virtual int encode(const IVIM__PDU__Descriptions::IVIM &ivim, BITSTRING &data);
  virtual int decode(const BITSTRING &data, IVIM__PDU__Descriptions::IVIM &);
};
