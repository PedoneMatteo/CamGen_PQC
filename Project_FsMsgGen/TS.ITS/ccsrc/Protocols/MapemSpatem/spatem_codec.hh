#pragma once

#include "codec.hh"
#include "params.hh"
#include "spatem_pdu_codec.hh"

#include "mapem_spatem_types.hh"

class spatem_codec : public codec<SPATEM__PDU__Descriptions::SPATEM, SPATEM__PDU__Descriptions::SPATEM> {
  spatem_pdu_codec asn_codec;

public:
  explicit spatem_codec() : codec<SPATEM__PDU__Descriptions::SPATEM, SPATEM__PDU__Descriptions::SPATEM>(), asn_codec(){};
  virtual ~spatem_codec(){};

  virtual int encode(const SPATEM__PDU__Descriptions::SPATEM &spatem, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, SPATEM__PDU__Descriptions::SPATEM &, params *params = NULL);
}; // End of class spatem_codec
