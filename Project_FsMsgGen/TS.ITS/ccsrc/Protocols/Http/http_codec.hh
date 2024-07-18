#pragma once

#include <memory>

#include "codec.hh"
#include "params.hh"

class Base_Type;
class Record_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsHttp__TypesAndValues {
  class HttpMessage;
  class Request;
  class Response;
  class Headers;
  class Header;
} // namespace LibItsHttp__TypesAndValues
namespace LibItsHttp__MessageBodyTypes {
  class HttpMessageBody;
}

struct encoding_context {
  unsigned int  length;
  unsigned char is_content_length_present;

  encoding_context() { reset(); };
  void reset() {
    length                    = -1;
    is_content_length_present = 0x00;
  };
};

struct decoding_context {
  unsigned int  length;
  unsigned char is_binary;
  bool          chunked;

  decoding_context() { reset(); };
  void reset() {
    length    = -1;
    is_binary = 0x00;
    chunked   = false;
  };
};

class http_codec : public codec<LibItsHttp__TypesAndValues::HttpMessage, LibItsHttp__TypesAndValues::HttpMessage> {
  encoding_context                                                        _ec;
  decoding_context                                                        _dc;
  std::map<std::string, std::unique_ptr<codec<Record_Type, Record_Type>>> _codecs;

public:
  explicit http_codec() : codec<LibItsHttp__TypesAndValues::HttpMessage, LibItsHttp__TypesAndValues::HttpMessage>(), _ec(), _dc(), _codecs(){};
  virtual ~http_codec(){};

  virtual int encode(const LibItsHttp__TypesAndValues::HttpMessage &, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, LibItsHttp__TypesAndValues::HttpMessage &, params *params = NULL);

  void set_payload_codecs(const std::string &p_codecs);

private:
  int encode_request(const LibItsHttp__TypesAndValues::Request &p_request, TTCN_Buffer &p_encoding_buffer);
  int encode_response(const LibItsHttp__TypesAndValues::Response &p_response, TTCN_Buffer &p_encoding_buffer);
  int encode_body(const LibItsHttp__MessageBodyTypes::HttpMessageBody &p_message_body, OCTETSTRING &p_encoding_buffer, const std::string &p_content_type);

  int decode_headers(TTCN_Buffer &decoding_buffer, LibItsHttp__TypesAndValues::Headers &headers, std::string &p_content_type);
  int decode_header(CHARSTRING &header_line, LibItsHttp__TypesAndValues::Header &header);
  int decode_body(TTCN_Buffer &decoding_buffer, LibItsHttp__MessageBodyTypes::HttpMessageBody &message_body, const std::string &p_content_type);
  int get_line(TTCN_Buffer &buffer, CHARSTRING &to, const bool concatenate_header_lines = false);

}; // End of class http_codec
