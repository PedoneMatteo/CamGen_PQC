#ifndef RTCMEMCODEC_H
#define RTCMEMCODEC_H

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

#include "LibItsRtcmem_TypesAndValues.hh"

class RtcmemPDUCodec : public per_codec<RTCMEM__PDU__Descriptions::RTCMEM> {
public:
  RtcmemPDUCodec(){};
  virtual int encode(const RTCMEM__PDU__Descriptions::RTCMEM &srem, BITSTRING &data);
  virtual int decode(const BITSTRING &data, RTCMEM__PDU__Descriptions::RTCMEM &);
};

class RtcmemCodec : public codec<RTCMEM__PDU__Descriptions::RTCMEM, RTCMEM__PDU__Descriptions::RTCMEM> {
  RtcmemPDUCodec asn_codec;

public:
  RtcmemCodec() : codec<RTCMEM__PDU__Descriptions::RTCMEM, RTCMEM__PDU__Descriptions::RTCMEM>(), asn_codec(){};
  virtual ~RtcmemCodec(){};

  virtual int encode(const RTCMEM__PDU__Descriptions::RTCMEM &srem, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, RTCMEM__PDU__Descriptions::RTCMEM &, params *params = NULL);
};

#endif
