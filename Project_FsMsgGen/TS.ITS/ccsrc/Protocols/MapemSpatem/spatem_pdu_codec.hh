#pragma once

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "mapem_spatem_types.hh"

class spatem_pdu_codec : public per_codec<SPATEM__PDU__Descriptions::SPATEM> {
public:
  explicit spatem_pdu_codec(){};
  virtual int encode(const SPATEM__PDU__Descriptions::SPATEM &spatem, BITSTRING &data);
  virtual int decode(const BITSTRING &data, SPATEM__PDU__Descriptions::SPATEM &);
}; // End of class spatem_pdu_codec
