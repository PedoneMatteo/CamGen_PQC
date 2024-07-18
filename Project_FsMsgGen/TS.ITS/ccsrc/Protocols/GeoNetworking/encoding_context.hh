#pragma once

#include <memory>

class encoding_context {
  unsigned char _basic_header;
  unsigned char _next_header;
  unsigned char _header_type;
  unsigned int  _length_position;
  unsigned int  _length;

public:
  explicit encoding_context() { reset(); }
  inline void reset() {
    _basic_header    = 0xff;
    _next_header     = 0xff;
    _header_type     = 0xff;
    _length_position = -1;
    _length          = -1;
  }

public:
  inline unsigned char get_basic_header() { return _basic_header; };
  inline void          set_basic_header(const unsigned char p_basic_header) { _basic_header = p_basic_header; };
  inline unsigned char get_next_header() { return _next_header; };
  inline void          set_next_header(const unsigned char p_next_header) { _next_header = p_next_header; };
  inline unsigned char get_header_type() { return _header_type; };
  inline void          set_header_type(const unsigned char p_header_type) { _header_type = p_header_type; };
  inline unsigned int  get_length_position() { return _length_position; };
  inline void          set_length_position(const unsigned int p_length_position) { _length_position = p_length_position; };
  inline unsigned int  get_length() { return _length; };
  inline void          set_length(const unsigned int p_length) { _length = p_length; };
}; // End of class encoding_context
