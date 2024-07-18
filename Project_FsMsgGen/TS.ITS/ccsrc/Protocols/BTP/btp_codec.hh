#ifndef BTPCODEC_H
#define BTPCODEC_H

#include "codec.hh"
#include "params.hh"

class Base_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsBtp__TestSystem {
  class BtpReq;
  class BtpInd;
} // namespace LibItsBtp__TestSystem

namespace LibItsBtp__TypesAndValues {
  class BtpPacket;
}

class btp_codec : public codec<LibItsBtp__TypesAndValues::BtpPacket, LibItsBtp__TypesAndValues::BtpPacket> {
  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

  int decode_(Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &decoding_buffer);

public:
  enum BtpType { btpA, btpB };
  btp_codec() : codec<LibItsBtp__TypesAndValues::BtpPacket, LibItsBtp__TypesAndValues::BtpPacket>() { _type = btp_codec::btpA; };
  virtual ~btp_codec(){};

  virtual int encode(const LibItsBtp__TypesAndValues::BtpPacket &, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, LibItsBtp__TypesAndValues::BtpPacket &, params *params = NULL);

  inline void set_btp_type(const btp_codec::BtpType p_btp_type) { _type = p_btp_type; };

private:
  BtpType _type;
};

#endif
