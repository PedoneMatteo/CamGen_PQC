#pragma once

#include "codec.hh"
#include "mapem_pdu_codec.hh"
#include "params.hh"

#include "LibItsMapemSpatem_TypesAndValues.hh"

class mapem_codec : public codec<MAPEM__PDU__Descriptions::MAPEM, MAPEM__PDU__Descriptions::MAPEM> {
  mapem_pdu_codec asn_codec;

public:
  explicit mapem_codec() : codec<MAPEM__PDU__Descriptions::MAPEM, MAPEM__PDU__Descriptions::MAPEM>(), asn_codec(){};
  virtual ~mapem_codec(){};

  virtual int encode(const MAPEM__PDU__Descriptions::MAPEM &sapem, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, MAPEM__PDU__Descriptions::MAPEM &, params *params = NULL);
}; // End of class mapem_codec
