#pragma once

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

class BITSTRING;
class OCTETSTRING;
class Base_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace DENM__PDU__Descriptions {
  class DENM;
}

class denm_pdu_codec : public per_codec<DENM__PDU__Descriptions::DENM> {
public:
  explicit denm_pdu_codec(){};
  virtual int encode(const DENM__PDU__Descriptions::DENM &denm, BITSTRING &data);
  virtual int decode(const BITSTRING &data, DENM__PDU__Descriptions::DENM &);
};
