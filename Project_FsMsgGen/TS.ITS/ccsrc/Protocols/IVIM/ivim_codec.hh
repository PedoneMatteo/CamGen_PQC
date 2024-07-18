#pragma once

#include "codec.hh"
#include "ivim_pdu_codec.hh"
#include "params.hh"

#include "LibItsIvim_TypesAndValues.hh"
// class BITSTRING;
// class OCTETSTRING;
// class IVIM__PDU__Descriptions::IVIM;

class ivim_codec : public codec<IVIM__PDU__Descriptions::IVIM, IVIM__PDU__Descriptions::IVIM> {
  ivim_pdu_codec asn_codec;

public:
  explicit ivim_codec() : codec<IVIM__PDU__Descriptions::IVIM, IVIM__PDU__Descriptions::IVIM>(), asn_codec(){};
  virtual ~ivim_codec(){};

  virtual int encode(const IVIM__PDU__Descriptions::IVIM &ivim, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, IVIM__PDU__Descriptions::IVIM &, params *params = NULL);
}; // End of class ivim_codec
