#pragma once

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "LibItsMapemSpatem_TypesAndValues.hh"

class mapem_pdu_codec : public per_codec<MAPEM__PDU__Descriptions::MAPEM> {
public:
  explicit mapem_pdu_codec(){};
  virtual int encode(const MAPEM__PDU__Descriptions::MAPEM &sapem, BITSTRING &data);
  virtual int decode(const BITSTRING &data, MAPEM__PDU__Descriptions::MAPEM &);
}; // End of class mapem_pdu_codec
