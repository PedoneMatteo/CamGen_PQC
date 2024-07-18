#pragma once

#include "codec.hh"
#include "denm_pdu_codec.hh"
#include "params.hh"

class BITSTRING;
class OCTETSTRING;
class Base_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace DENM__PDU__Descriptions {
  class DENM;
}

class denm_codec : public codec<DENM__PDU__Descriptions::DENM, DENM__PDU__Descriptions::DENM> {
  denm_pdu_codec asn_codec;

public:
  explicit denm_codec() : codec<DENM__PDU__Descriptions::DENM, DENM__PDU__Descriptions::DENM>(), asn_codec(){};
  virtual ~denm_codec(){};

  virtual int encode(const DENM__PDU__Descriptions::DENM &denm, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, DENM__PDU__Descriptions::DENM &, params *params = NULL);
};
