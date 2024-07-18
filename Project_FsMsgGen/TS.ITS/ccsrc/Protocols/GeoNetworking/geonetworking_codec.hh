#pragma once

#include <memory>

#include "codec.hh"
#include "params.hh"

#include "decoding_context.hh"
#include "encoding_context.hh"

class Base_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsGeoNetworking__TypesAndValues {
  class GeoNetworkingPdu;
  class GnNonSecuredPacket;
  class HeaderTST;
  class ExtendedHeader;
  class DecodedPayload;
}; // namespace LibItsGeoNetworking__TypesAndValues

namespace LibItsSecurity__TypesAndValues {
  class SecuredMessage;
};

class geonetworking_codec : public codec<LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu, LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu> {

  encoding_context _ec;
  decoding_context _dc;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);
  int encode_extendedHeader(const LibItsGeoNetworking__TypesAndValues::ExtendedHeader &u, TTCN_Buffer &encoding_buffer);

  int decode_(Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &decoding_buffer);
  int decode_headerTST(LibItsGeoNetworking__TypesAndValues::HeaderTST &u, TTCN_Buffer &decoding_buffer);
  int decode_extendedHeader(LibItsGeoNetworking__TypesAndValues::ExtendedHeader &u, TTCN_Buffer &decodin_buffer);

public:
  explicit geonetworking_codec()
    : codec<LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu, LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu>(), _ec(), _dc(){};
  virtual ~geonetworking_codec(){};

  virtual int encode(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &msg, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &msg, params *params = NULL);

  int encode(const LibItsGeoNetworking__TypesAndValues::GnNonSecuredPacket &p_gnNonSecuredPacket,
             OCTETSTRING &                                                  data); // TODO Duplicate code with virtual encode, to be enhanced
};                               // End of class geonetworking_codec
