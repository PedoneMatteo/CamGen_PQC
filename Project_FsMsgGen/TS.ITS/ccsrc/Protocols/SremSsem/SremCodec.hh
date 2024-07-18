#ifndef SREMCODEC_H
#define SREMCODEC_H

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "LibItsSremSsem_TypesAndValues.hh"

class SremPDUCodec : public per_codec<SREM__PDU__Descriptions::SREM> {
public:
  SremPDUCodec(){};
  virtual int encode(const SREM__PDU__Descriptions::SREM &srem, BITSTRING &data);
  virtual int decode(const BITSTRING &data, SREM__PDU__Descriptions::SREM &);
};

class SremCodec : public codec<SREM__PDU__Descriptions::SREM, SREM__PDU__Descriptions::SREM> {
  SremPDUCodec asn_codec;

public:
  SremCodec() : codec<SREM__PDU__Descriptions::SREM, SREM__PDU__Descriptions::SREM>(), asn_codec(){};
  virtual ~SremCodec(){};

  virtual int encode(const SREM__PDU__Descriptions::SREM &srem, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, SREM__PDU__Descriptions::SREM &, params *params = NULL);
};

#endif
