#pragma once

#include <string>

class decoding_context {
  unsigned char _next_header;
  unsigned char _header_type;
  unsigned char _header_sub_type;
  unsigned char _traffic_class;
  unsigned int  _lifetime;
  unsigned int  _length;

public:
  explicit decoding_context() { reset(); };
  ~decoding_context(){};
  inline void reset() {
    _next_header     = 0xff;
    _header_type     = 0xff;
    _header_sub_type = 0xff;
    _lifetime        = 0;
    _length          = -1;
  };

public:
  inline unsigned char get_next_header() { return _next_header; };
  inline void          set_next_header(const unsigned char p_next_header) { _next_header = p_next_header; };
  inline unsigned char get_header_type() { return _header_type; };
  inline void          set_header_type(const unsigned char p_header_type) { _header_type = p_header_type; };
  inline unsigned char get_header_sub_type() { return _header_sub_type; };
  inline void          set_header_sub_type(const unsigned char p_header_sub_type) { _header_sub_type = p_header_sub_type; };
  inline unsigned char get_traffic_class() { return _traffic_class; };
  inline void          set_traffic_class(const unsigned char p_traffic_class) { _traffic_class = p_traffic_class; };
  inline unsigned int  get_lifetime() { return _lifetime; };
  inline void          set_lifetime(const unsigned int p_lifetime) { _lifetime = p_lifetime; };
  inline unsigned int  get_length() { return _length; };
  inline void          set_length(const unsigned int p_length) { _length = p_length; };
}; // End of class decoding_context
