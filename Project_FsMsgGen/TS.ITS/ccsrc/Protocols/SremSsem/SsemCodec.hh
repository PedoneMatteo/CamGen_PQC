#ifndef SSEMCODEC_H
#define SSEMCODEC_H

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "LibItsSremSsem_TypesAndValues.hh"

class SsemPDUCodec : public per_codec<SSEM__PDU__Descriptions::SSEM> {
public:
  SsemPDUCodec(){};
  virtual int encode(const SSEM__PDU__Descriptions::SSEM &ssem, BITSTRING &data);
  virtual int decode(const BITSTRING &data, SSEM__PDU__Descriptions::SSEM &);
};

class SsemCodec : public codec<SSEM__PDU__Descriptions::SSEM, SSEM__PDU__Descriptions::SSEM> {
  SsemPDUCodec asn_codec;

public:
  SsemCodec() : codec<SSEM__PDU__Descriptions::SSEM, SSEM__PDU__Descriptions::SSEM>(), asn_codec(){};
  virtual ~SsemCodec(){};

  virtual int encode(const SSEM__PDU__Descriptions::SSEM &ssem, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, SSEM__PDU__Descriptions::SSEM &, params *params = NULL);
};

#endif
