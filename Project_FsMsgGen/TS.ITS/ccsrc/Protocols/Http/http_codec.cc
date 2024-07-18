#include <regex>
#include <stdexcept>
#include <string>

#include "codec_stack_builder.hh"

#include "http_codec.hh"

#include "loggers.hh"

#include "LibItsHttp_MessageBodyTypes.hh"
#include "LibItsHttp_TypesAndValues.hh"
#include "LibItsHttp_XmlMessageBodyTypes.hh"

#include "http_etsi_ieee1609dot2_codec.hh"

int http_codec::encode(const LibItsHttp__TypesAndValues::HttpMessage &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> http_codec::encode: ", (const Base_Type &)msg);
  loggers::get_instance().log(">>> http_codec::encode: %p", this);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;

  _ec.reset();

  int result;
  if (msg.ischosen(LibItsHttp__TypesAndValues::HttpMessage::ALT_request)) {
    result = encode_request(msg.request(), encoding_buffer);
  } else if (msg.ischosen(LibItsHttp__TypesAndValues::HttpMessage::ALT_response)) {
    result = encode_response(msg.response(), encoding_buffer);
  } else {
    loggers::get_instance().warning("http_codec::encode: Unbound HttpMessage");
    return -1;
  }

  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());

  loggers::get_instance().log_msg("<<< http_codec::encode: data=", data);
  return result;
}

int http_codec::decode(const OCTETSTRING &data, LibItsHttp__TypesAndValues::HttpMessage &msg, params *params) {
  loggers::get_instance().log_msg(">>> http_codec::decode: data=", data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  loggers::get_instance().log_to_hexa("http_codec::decode: decoding_buffer=", decoding_buffer);

  _dc.reset();

  _params = params;

  // Get the first line (e.g. HTTP/1.1 302 Found or POST / HTTP/1.1)
  CHARSTRING message_id;
  if (get_line(decoding_buffer, message_id) == -1) {
    return -1;
  }
  loggers::get_instance().log_msg("http_codec::decode: message_id: ", message_id);
  // Extract parameters
  try {
    std::string          str(static_cast<const char *>(message_id));
    std::regex           rgx("\\s*(\\w+)/");
    std::sregex_iterator begin(str.cbegin(), str.cend(), rgx);
    std::smatch          m = *begin;
    loggers::get_instance().log("http_codec::decode: %d - %s", m.size(), m[0].str().c_str());
    if (m[0].str().compare("HTTP/") == 0) { // HTTP response
      LibItsHttp__TypesAndValues::Response response;
      std::regex                           rgx("\\s*HTTP/(\\d+)\\.(\\d+)\\s+(\\d+)\\s+([\\w\\s\\t\\v\\f]+)*");
      std::sregex_iterator                 begin(str.cbegin(), str.cend(), rgx);
      std::smatch                          m = *begin;
      loggers::get_instance().log("http_codec::decode: Process response: %d", m.size());
      if (m.size() != 5) {
        loggers::get_instance().error("http_codec::decode: Unsupported tag");
        return -1;
      }
      response.version__major() = std::stoi(m[1].str().c_str());
      response.version__minor() = std::stoi(m[2].str().c_str());
      response.statuscode()     = std::stoi(m[3].str().c_str());
      response.statustext()     = CHARSTRING(m[4].str().c_str());
      LibItsHttp__TypesAndValues::Headers headers;
      std::string                         content_type;
      decode_headers(decoding_buffer, headers, content_type);
      response.header() = headers;
      loggers::get_instance().log_to_hexa("Before decoding Body: ", decoding_buffer);
      LibItsHttp__MessageBodyTypes::HttpMessageBody body;
      if (decode_body(decoding_buffer, body, content_type) == -1) {
        response.body().set_to_omit();
      } else {
        response.body() = OPTIONAL<LibItsHttp__MessageBodyTypes::HttpMessageBody>(body);
      }
      msg.response() = response;
    } else { // HTTP request
      LibItsHttp__TypesAndValues::Request request;
      std::regex                          rgx("\\s*(\\w+)\\s+(.+)\\s+HTTP/(\\d)\\.(\\d)");
      std::sregex_iterator                begin(str.cbegin(), str.cend(), rgx);
      std::smatch                         m = *begin;
      if (m.size() != 5) {
        loggers::get_instance().error("http_codec::decode: Unsupported tag");
        return -1;
      }
      request.method()         = CHARSTRING(m[1].str().c_str());
      request.uri()            = CHARSTRING(m[2].str().c_str());
      request.version__major() = std::stoi(m[3].str().c_str());
      request.version__minor() = std::stoi(m[4].str().c_str());
      LibItsHttp__TypesAndValues::Headers headers;
      std::string                         content_type;
      decode_headers(decoding_buffer, headers, content_type);
      request.header() = headers;
      OPTIONAL<LibItsHttp__MessageBodyTypes::HttpMessageBody> body;
      body.set_to_omit();
      if (decode_body(decoding_buffer, body, content_type) == -1) {
        request.body().set_to_omit();
      } else {
        request.body() = body;
      }
      msg.request() = request;
    }

    loggers::get_instance().log_msg("<<< http_codec::decode: ", (const Base_Type &)msg);
    return 0;
  } catch (const std::logic_error &e) {
    return -1;
  }
}

int http_codec::encode_request(const LibItsHttp__TypesAndValues::Request &p_request, TTCN_Buffer &p_encoding_buffer) {
  loggers::get_instance().log_msg(">>> http_codec::encode_request: ", (const Base_Type &)p_request);

  // Encode generic part
  p_encoding_buffer.put_cs(p_request.method());
  p_encoding_buffer.put_c(' ');
  p_encoding_buffer.put_cs(p_request.uri());
  p_encoding_buffer.put_cs(" HTTP/");
  p_encoding_buffer.put_cs(int2str(p_request.version__major()));
  p_encoding_buffer.put_c('.');
  p_encoding_buffer.put_cs(int2str(p_request.version__minor()));
  p_encoding_buffer.put_cs("\r\n");

  // Encode headers excepeted the Content-Length
  const LibItsHttp__TypesAndValues::Headers &headers = p_request.header();
  std::string                                content_type;
  for (int i = 0; i < headers.size_of(); i++) {
    const LibItsHttp__TypesAndValues::Header &header = headers[i];
    loggers::get_instance().log_msg("http_codec::encode_request: Processing header ", header.header__name());
    if (std::string(static_cast<const char *>(header.header__name())).compare("Content-Length") == 0) { // Skip it, processed later
      loggers::get_instance().log("http_codec::encode_request: Skip it");
      continue;
    } else {
      p_encoding_buffer.put_cs(header.header__name());
      p_encoding_buffer.put_cs(": ");
      const OPTIONAL<LibItsHttp__TypesAndValues::charstring__list> &o = header.header__value();
      if (o.ispresent()) {
        const LibItsHttp__TypesAndValues::charstring__list &v = dynamic_cast<const OPTIONAL<LibItsHttp__TypesAndValues::charstring__list> &>(o);
        if (v.size_of() > 0) {
          loggers::get_instance().log_msg("http_codec::encode_request: Processing value ", v[0]);
          if (std::string(static_cast<const char *>(header.header__name())).compare("Content-Type") == 0) { // Store it for HTTP body payload encoding
            loggers::get_instance().log("http_codec::encode_request: Storing Content-Type");
            int j = 0;
            while (j < v.size_of()) {
              content_type += v[j++];
            } // End of 'while' statement
          }
          p_encoding_buffer.put_cs(v[0]);
          int j = 1;
          while (j < v.size_of()) {
            p_encoding_buffer.put_cs(", ");
            loggers::get_instance().log_msg("http_codec::encode_request: Processing value ", v[j]);
            p_encoding_buffer.put_cs(v[j++]);
          } // End of 'while' statement
        }
      } // else, do not include it
    }
    p_encoding_buffer.put_cs("\r\n");
  } // End of 'for' statement

  // Encode message body
  const OPTIONAL<LibItsHttp__MessageBodyTypes::HttpMessageBody> &v = p_request.body();
  OCTETSTRING                                                    os;
  if (v.ispresent()) {
    const LibItsHttp__MessageBodyTypes::HttpMessageBody &body = static_cast<const LibItsHttp__MessageBodyTypes::HttpMessageBody &>(*v.get_opt_value());
    loggers::get_instance().log_msg("http_codec::encode_request: body: ", body);
    if (encode_body(body, os, content_type) == -1) {
      loggers::get_instance().warning("http_codec::encode_request: Failed to encode HTTP body");
      _ec.length                    = 0;
      _ec.is_content_length_present = 0x00;
    } else {
      _ec.length                    = os.lengthof();
      _ec.is_content_length_present = 0x01;
    }
    loggers::get_instance().log("http_codec::encode_request: length=%d", _ec.length);
  } else {
    loggers::get_instance().log("http_codec::encode_request: HTTP body field not present");
    _ec.length                    = 0;
    _ec.is_content_length_present = 0x00;
  }

  // Encode Content-Length header
  p_encoding_buffer.put_cs("Content-Length: ");
  if (_ec.length != 0) {
    loggers::get_instance().log("http_codec::encode_request: Content-Length: %s",
                                static_cast<const char *>(int2str(_ec.length + 2 /*Stand for the last CRLF*/)));
    p_encoding_buffer.put_cs(static_cast<const char *>(int2str(_ec.length)));
    _ec.is_content_length_present = 0x01;
  } else {
    p_encoding_buffer.put_cs("0");
    _ec.is_content_length_present = 0x00;
  }
  loggers::get_instance().log("http_codec::encode_request: Content-Length: %d - %x", _ec.length, _ec.is_content_length_present);
  p_encoding_buffer.put_cs("\r\n");

  // Add message body
  p_encoding_buffer.put_cs("\r\n");
  if (_ec.is_content_length_present == 0x01) {
    loggers::get_instance().log_msg("http_codec::encode_request: Add body ", os);
    p_encoding_buffer.put_os(os);
    // p_encoding_buffer.put_cs("\r\n");
  }

  loggers::get_instance().log_to_hexa("<<< http_codec::encode_request: ", p_encoding_buffer);
  return 0;
}

int http_codec::encode_response(const LibItsHttp__TypesAndValues::Response &p_response, TTCN_Buffer &p_encoding_buffer) {
  loggers::get_instance().log_msg(">>> http_codec::encode_response: ", (const Base_Type &)p_response);

  // Encode generic part
  p_encoding_buffer.put_cs("HTTP/");
  p_encoding_buffer.put_cs(int2str(p_response.version__major()));
  p_encoding_buffer.put_c('.');
  p_encoding_buffer.put_cs(int2str(p_response.version__minor()));
  p_encoding_buffer.put_cs(" ");
  p_encoding_buffer.put_cs(int2str(p_response.statuscode()));
  p_encoding_buffer.put_cs(" ");
  if (p_response.statustext().lengthof() != 0) {
    p_encoding_buffer.put_cs(p_response.statustext());
  }
  p_encoding_buffer.put_cs("\r\n");

  // Encode headers excepeted the Content-Length
  const LibItsHttp__TypesAndValues::Headers &headers = p_response.header();
  std::string                                content_type;
  for (int i = 0; i < headers.size_of(); i++) {
    const LibItsHttp__TypesAndValues::Header &header = headers[i];
    loggers::get_instance().log_msg("http_codec::encode_response: Processing header ", header.header__name());
    if (std::string(static_cast<const char *>(header.header__name())).compare("Content-Length") == 0) {
      continue;
    } else {
      p_encoding_buffer.put_cs(header.header__name());
      p_encoding_buffer.put_cs(": ");
      const OPTIONAL<LibItsHttp__TypesAndValues::charstring__list> &o = header.header__value();
      if (o.ispresent()) {
        const LibItsHttp__TypesAndValues::charstring__list &v = dynamic_cast<const OPTIONAL<LibItsHttp__TypesAndValues::charstring__list> &>(o);
        if (v.size_of() > 0) {
          loggers::get_instance().log_msg("http_codec::encode_response: Processing value ", v[0]);
          if (std::string(static_cast<const char *>(header.header__name())).compare("Content-Type") == 0) { // Store it for HTTP body payload encoding
            loggers::get_instance().log("http_codec::encode_response: Storing Content-Type");
            int j = 0;
            while (j < v.size_of()) {
              content_type += v[j++];
            } // End of 'while' statement
          }
          p_encoding_buffer.put_cs(v[0]);
          int j = 1;
          while (j < v.size_of()) {
            p_encoding_buffer.put_cs(", ");
            loggers::get_instance().log_msg("http_codec::encode_response: Processing value ", v[j]);
            p_encoding_buffer.put_cs(v[j++]);
            j += 1;
          } // End of 'while' statement
        }
      } // else, do not include it
    }
    p_encoding_buffer.put_cs("\r\n");
  } // End of 'for' statement

  // Encode message body
  const OPTIONAL<LibItsHttp__MessageBodyTypes::HttpMessageBody> &v = p_response.body();
  OCTETSTRING                                                    os;
  if (v.ispresent()) {
    const LibItsHttp__MessageBodyTypes::HttpMessageBody &body = static_cast<const LibItsHttp__MessageBodyTypes::HttpMessageBody &>(*v.get_opt_value());
    loggers::get_instance().log_msg("http_codec::encode_response: body: ", body);
    if (encode_body(body, os, content_type) == -1) {
      _ec.length                    = 0;
      _ec.is_content_length_present = 0x00;
    } else {
      _ec.length                    = os.lengthof();
      _ec.is_content_length_present = 0x01;
    }
    loggers::get_instance().log("http_codec::encode_response: length=%d", _ec.length);
  } else {
    loggers::get_instance().log("http_codec::encode_response: HTTP body field not present");
    _ec.length                    = 0;
    _ec.is_content_length_present = 0x00;
  }

  // Encode Content-Length header
  p_encoding_buffer.put_cs("Content-Length: ");
  if (_ec.length != 0) {
    loggers::get_instance().log("http_codec::encode_request: Content-Length: %s",
                                static_cast<const char *>(int2str(_ec.length + 2 /*Stand for the last CRLF*/)));
    p_encoding_buffer.put_cs(static_cast<const char *>(int2str(_ec.length)));
    _ec.is_content_length_present = 0x01;
  } else {
    p_encoding_buffer.put_cs("0");
    _ec.is_content_length_present = 0x00;
  }
  loggers::get_instance().log("http_codec::encode_request: Content-Length: %d - %x", _ec.length, _ec.is_content_length_present);
  p_encoding_buffer.put_cs("\r\n");

  // Add message body
  p_encoding_buffer.put_cs("\r\n");
  if (_ec.is_content_length_present == 0x01) {
    loggers::get_instance().log_msg("http_codec::encode_request: Add body ", os);
    p_encoding_buffer.put_os(os);
    //    p_encoding_buffer.put_cs("\r\n");
  }

  loggers::get_instance().log_to_hexa("<<< http_codec::encode_response: ", p_encoding_buffer);
  return 0;
}

int http_codec::decode_headers(TTCN_Buffer &decoding_buffer, LibItsHttp__TypesAndValues::Headers &headers, std::string &p_content_type) {
  loggers::get_instance().log(">>> http_codec::decode_headers");
  loggers::get_instance().log_to_hexa("http_codec::decode_headers: ", decoding_buffer);

  CHARSTRING cstr;
  int        i = 0;
  while (true) {
    switch (get_line(decoding_buffer, cstr, true)) {
      case 0: {
        loggers::get_instance().log_msg("http_codec::decode_headers: ", cstr);
        LibItsHttp__TypesAndValues::Header header;
        if (decode_header(cstr, header) == -1) {
          loggers::get_instance().warning("http_codec::decode_headers: Failed to decode header %s", static_cast<const char *>(cstr));
          return -1;
        }
        headers[i++] = header;
        if (std::string(static_cast<const char *>(header.header__name())).compare("Content-Type") == 0) {
          if (header.header__value().is_present() != 0) {
            const PreGenRecordOf::PREGEN__RECORD__OF__CHARSTRING &l =
              static_cast<const PreGenRecordOf::PREGEN__RECORD__OF__CHARSTRING &>(*header.header__value().get_opt_value());
            p_content_type = static_cast<const char *>(l[0]);
          } else {
            p_content_type = "";
          }
        }
      } break;
      case 1:
        loggers::get_instance().log_msg("<<< http_codec::decode_headers: ", headers);
        return 0;
      case -1:
        loggers::get_instance().warning("http_codec::decode_headers: Failed to decode headers");
        return -1;
    } // End of 'switch' statement
  }   // End of 'while' statement
}

int http_codec::decode_header(CHARSTRING &header_line, LibItsHttp__TypesAndValues::Header &header) {
  loggers::get_instance().log_msg(">>> http_codec::decode_header", header_line);

  try {
    std::string          str(static_cast<const char *>(header_line));
    std::regex           rgx("([0-9a-zA-Z-]+)\\:\\s+(.+)(,(.+))*");
    std::sregex_iterator begin(str.cbegin(), str.cend(), rgx);
    std::smatch          m = *begin;
    if (m.size() < 5) {
      loggers::get_instance().warning("http_codec::decode_header: Failed to decode header %s", str.c_str());
      return -1;
    }
    loggers::get_instance().log("http_codec::decode_header: %d", m.size());
    header.header__name() = CHARSTRING(m[1].str().c_str());
    LibItsHttp__TypesAndValues::charstring__list v;
    for (unsigned int j = 0; j < m.size(); j++) {
      if (m[j + 2].str().length() == 0) {
        break;
      }
      v[j] = CHARSTRING(m[j + 2].str().c_str());
    } // End of 'for' statement
    header.header__value() = OPTIONAL<LibItsHttp__TypesAndValues::charstring__list>(v);

    if (m[1].str().compare("Content-Length") == 0) {
      // Save the the body length
      loggers::get_instance().log("http_codec::decode_header: decoded Content-Length %s", m[2].str().c_str());
      _dc.length = std::stoi(m[2].str());
    } else if (m[1].str().compare("Transfer-Encoding") == 0) {
      if (m[2].str().find("chunked") != std::string::npos) {
        _dc.chunked = true;
        loggers::get_instance().log("http_codec::decode_header: decoded Transfer-Encoding %x", _dc.chunked);
      }
    }

    return 0;
  } catch (const std::logic_error &e) {
    return -1;
  }
}

int http_codec::encode_body(const LibItsHttp__MessageBodyTypes::HttpMessageBody &p_message_body, OCTETSTRING &p_encoding_buffer,
                            const std::string &p_content_type) {
  loggers::get_instance().log_msg(">>> http_codec::encode_body: ", (const Base_Type &)p_message_body);

  // Sanity check
  if (p_content_type.empty()) {
    loggers::get_instance().warning("http_codec::encode_body: Failed to select a codec for  HTTP body payload");
    return -1;
  }

  if (p_message_body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_binary__body)) {
    const LibItsHttp__BinaryMessageBodyTypes::BinaryBody &binary_body = p_message_body.binary__body();
    if (binary_body.ischosen(LibItsHttp__BinaryMessageBodyTypes::BinaryBody::ALT_raw)) {
      p_encoding_buffer = OCTETSTRING(binary_body.raw().lengthof(), (unsigned char *)static_cast<const unsigned char *>(binary_body.raw()));
    } else {
      std::map<std::string, std::unique_ptr<codec<Record_Type, Record_Type>>>::const_iterator it;
      bool                                                                                    processed = false;
      if (p_content_type.find("x-its") != std::string::npos) {
        loggers::get_instance().log("http_codec::encode_body: Find x-its");
        it = _codecs.find("http_its"); // TODO Use params
        if (it != _codecs.cend()) {
          loggers::get_instance().log("http_codec::encode_body: Call '%s'", it->first.c_str());
          if (binary_body.ischosen(LibItsHttp__BinaryMessageBodyTypes::BinaryBody::ALT_ieee1609dot2__data)) {
            _codecs["http_its"]->encode((Record_Type &)binary_body.ieee1609dot2__data(), p_encoding_buffer); // TODO Use params
            processed = true;
          } else if (binary_body.ischosen(LibItsHttp__BinaryMessageBodyTypes::BinaryBody::ALT_ieee1609dot2__certificate)) {
            _codecs["http_its"]->encode((Record_Type &)binary_body.ieee1609dot2__certificate(), p_encoding_buffer); // TODO Use params
            processed = true;
          } else {
            loggers::get_instance().warning("http_codec::encode_body: Unsupported variant");
          }
        }
      } // TODO Add new HTTP message codec here
      if (!processed) {
        loggers::get_instance().warning("http_codec::encode_body: Unsupported HTTP codec, use raw field as default");
        p_encoding_buffer = OCTETSTRING(0, nullptr);
      }
    }
  } else if (p_message_body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_html__body)) {
    p_encoding_buffer = OCTETSTRING(p_message_body.html__body().lengthof(), (unsigned char *)static_cast<const char *>(p_message_body.html__body()));
  } else if (p_message_body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_text__body)) {
    p_encoding_buffer = OCTETSTRING(p_message_body.text__body().lengthof(), (unsigned char *)static_cast<const char *>(p_message_body.text__body()));
  } else if (p_message_body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_xml__body)) {
    const LibItsHttp__XmlMessageBodyTypes::XmlBody &xml_body = p_message_body.xml__body();
    if (xml_body.ischosen(LibItsHttp__XmlMessageBodyTypes::XmlBody::ALT_raw)) {
      p_encoding_buffer = OCTETSTRING(xml_body.raw().lengthof(), (unsigned char *)static_cast<const char *>(xml_body.raw()));
    } else {
      std::map<std::string, std::unique_ptr<codec<Record_Type, Record_Type>>>::const_iterator it;
      bool                                                                                    processed = false;
      loggers::get_instance().log("http_codec::encode_body: Content-Type:'%s'", p_content_type.c_str());
      if (p_content_type.find("held") != std::string::npos) {
        it = _codecs.find("held"); // TODO Use params
        if (it != _codecs.cend()) {
          loggers::get_instance().log("http_codec::encode_body: Call 'held_codec'");
          _codecs["held"]->encode((Record_Type &)xml_body, p_encoding_buffer); // TODO Use params
          processed = true;
        }
      } else if (p_content_type.find("lost") != std::string::npos) {
        it = _codecs.find("lost"); // TODO Use params
        if (it != _codecs.cend()) {
          loggers::get_instance().log("http_codec::encode_body: Call 'lost_codec'");
          _codecs["lost"]->encode((Record_Type &)xml_body, p_encoding_buffer); // TODO Use params
          processed = true;
        }
      } // TODO Add new HTTP message codec here
      if (!processed) {
        loggers::get_instance().warning("http_codec::encode_body: Unsupported HTTP codec, use raw field as default");
        p_encoding_buffer = OCTETSTRING(0, nullptr);
      }
    }
  } else {
    loggers::get_instance().warning("http_codec::encode_body: Failed to encode HTTP message body");
    return -1;
  }
  loggers::get_instance().log_msg("http_codec::encode_body: HTTP message ", p_encoding_buffer);
  _ec.length = p_encoding_buffer.lengthof();
  loggers::get_instance().log("http_codec::encode_body: HTTP message length: %d", _ec.length);

  return 0;
}

int http_codec::decode_body(TTCN_Buffer &decoding_buffer, LibItsHttp__MessageBodyTypes::HttpMessageBody &message_body, const std::string &p_content_type) {
  loggers::get_instance().log(">>> http_codec::decode_body");
  loggers::get_instance().log_to_hexa("http_codec::decode_body", decoding_buffer);
  loggers::get_instance().log("http_codec::decode_body: # of codecs=%d - %p", _codecs.size(), this);
  loggers::get_instance().log("http_codec::decode_body: Content-Type=%s", p_content_type.c_str());

  // Sanity checks
  if (decoding_buffer.get_len() - decoding_buffer.get_pos() <= 0) {
    return -1;
  }
  /* TODO Uncommentif (p_content_type.empty()) {
    loggers::get_instance().warning("http_codec::encode_body: Failed to select a codec for  HTTP body payload");
    return -1;
    }*/

  OCTETSTRING s(decoding_buffer.get_len() - decoding_buffer.get_pos(), decoding_buffer.get_data() + decoding_buffer.get_pos());
  loggers::get_instance().log_msg("http_codec::decode_body: raw body=", s);

  // Align the payload length with the specified Content-Lenght value
  loggers::get_instance().log("http_codec::decode_body: _dc.length=%d - body length=%d", _dc.length, s.lengthof());
  OCTETSTRING body;
  if (_dc.length != 0) {
    const unsigned char *p = static_cast<const unsigned char *>(s);
    if ((unsigned int)s.lengthof() <= _dc.length) {
      body = OCTETSTRING(s.lengthof(), p);
    } else {
      body = OCTETSTRING(_dc.length, p);
    }
  } else {
    loggers::get_instance().warning("http_codec::decode_body: No Conten-Length header, process all remaining bytes");
    body = s;
  }
  loggers::get_instance().log_msg("http_codec::decode_body: Aligned body=", body);
  loggers::get_instance().log("http_codec::decode_body: body length=%d", body.lengthof());
  /* TODO To be removed
  // Remove CRLF if any
  int counter = 0;
  if ((body[body.lengthof() - 1].get_octet() == 0x0d) || (body[body.lengthof() - 1].get_octet() == 0x0a)) {
    counter += 1;
    if ((body[body.lengthof() - 2].get_octet() == 0x0d) || (body[body.lengthof() - 2].get_octet() == 0x0a)) {
      counter += 1;
    }
  }
  loggers::get_instance().log("http_codec::decode_body: counter=%d", counter);
  body = OCTETSTRING(body.lengthof() - counter, static_cast<const unsigned char*>(body));
  */
  if (_dc.chunked) {
    int         counter = 0;
    int         prev    = 0;
    OCTETSTRING os(0, nullptr);
    do {
      while (counter < body.lengthof()) { // Extract the size of the chunk <chunk size>\r[\n]
        if ((body[counter].get_octet() == '\r') || (body[counter].get_octet() == '\n')) {
          break;
        }
        counter += 1;
      } // End of 'while' statement
      loggers::get_instance().log("http_codec::decode_body: Chunked(0): prev = %d, counter=%d / %d", prev, counter, body.lengthof());
      if (counter < body.lengthof()) {
        int         idx = counter - prev;
        OCTETSTRING trunk(idx, static_cast<const unsigned char *>(body));
        loggers::get_instance().log_msg("http_codec::decode_body: trunk: ", trunk);
        std::string str((const char *)static_cast<const unsigned char *>(trunk), idx);
        loggers::get_instance().log("http_codec::decode_body: str: '%s'", str.c_str());
        int len = std::stoi(str, nullptr, 16); // converter::get_instance().string_to_int(str);
        loggers::get_instance().log("http_codec::decode_body: Chunk len: %d", len);
        while (counter < body.lengthof() && ((body[counter].get_octet() == '\r') || (body[counter].get_octet() == '\n'))) { // Skip additional \n
          counter += 1;
        } // End of 'while' statement
        if (counter < body.lengthof()) {
          loggers::get_instance().log("http_codec::decode_body: Chunked (1): prev = %d, counter=%d / %d", prev, counter, body.lengthof());
          os += OCTETSTRING(len, counter + static_cast<const unsigned char *>(body));
          loggers::get_instance().log_msg("http_codec::decode_body: os=", os);
          counter += len;
          loggers::get_instance().log("http_codec::decode_body: Chunked: %02x %02x %02x", body[counter].get_octet(), body[counter + 1].get_octet(),
                                      body[counter + 2].get_octet());
          loggers::get_instance().log("http_codec::decode_body: Chunked (2): prev = %d, counter=%d / %d", prev, counter, body.lengthof());
          while (counter < body.lengthof() && ((body[counter].get_octet() == '\r') || (body[counter].get_octet() == '\n'))) { // Skip additional \n
            counter += 1;
          } // End of 'while' statement
          prev = counter;
          loggers::get_instance().log("http_codec::decode_body: Chunked (3): prev = %d, counter=%d / %d", prev, counter, body.lengthof());
        }
      }
    } while (counter < body.lengthof()); // Process next chunk if any
    body = os;
    loggers::get_instance().log_msg("http_codec::decode_body: Finalised body=", body);
  }
  // Check if HTTP message body contains binary characters
  for (int i = 0; i < body.lengthof(); i++) {
    unsigned char c = body[i].get_octet();
    if (!std::isprint(c) && !std::isspace(c) && !std::ispunct(c)) {
      loggers::get_instance().log("http_codec::decode_body: Byte #%d is not printable: 0x%02x", i, body[i].get_octet());
      _dc.is_binary = 0x01;
      break;
    }
  } // End of 'for' statement
  loggers::get_instance().log("http_codec::decode_body: Binary mode: %x", _dc.is_binary);
  LibItsHttp__MessageBodyTypes::HttpMessageBody v;
  if (_dc.is_binary == 0x01) {
    LibItsHttp__BinaryMessageBodyTypes::BinaryBody                                          binary_body;
    std::map<std::string, std::unique_ptr<codec<Record_Type, Record_Type>>>::const_iterator it;
    bool                                                                                    processed = false;
    // TODO To be refined adding a string identifier to check which codec to use. E.g. held_code.id() returns "xmlns=\"urn:ietf:params:xml:ns:geopriv:held\">"
    if ((p_content_type.find("x-its") != std::string::npos) || (p_content_type.find("application/octet-stream") != std::string::npos)) {
      loggers::get_instance().log("http_codec::decode_body: Find 'x-its'");
      it = _codecs.cbegin(); //_codecs.find("http_its");
      if (it != _codecs.cend()) {
        /***
            FIXME:
            This code generate a codedump, I don't undertsand the reason.
            The same code works file for Ng112 HELD & LOST codec. Ununderstandable!!!!
            ==> Use a patch
            if (_codecs["http_its"].get() != nullptr) {
            loggers::get_instance().log("http_codec::decode_body: Call 'http_etsi_ieee1609dot2_codec'");
            if (_codecs["http_its"]->decode(body, (Record_Type&)binary_body) == 0) {
            processed = true;
            }
            }*/
        loggers::get_instance().log("http_codec::decode_body: Call '%s'", it->first.c_str());
        http_etsi_ieee1609dot2_codec *codec = new http_etsi_ieee1609dot2_codec();
        if (body[0].get_octet() != 0x80) {
          if (codec->decode(body, binary_body.ieee1609dot2__data()) == 0) {
            message_body.binary__body() = binary_body;
            processed                   = true;
          }
        } else {
          if (codec->decode(body, binary_body.ieee1609dot2__certificate()) == 0) {
            message_body.binary__body() = binary_body;
            processed                   = true;
          }
        }
        delete codec;
      }
    } // TODO Add new HTTP message codec here
    if (!processed) {
      loggers::get_instance().warning("http_codec::decode_body: Unsupported HTTP codec, use raw field as default");
      binary_body.raw()           = body;
      message_body.binary__body() = binary_body;
    }
  } else {
    // Convert into string
    params p;
    p["decode_str"] = std::string(static_cast<const unsigned char *>(body), body.lengthof() + static_cast<const unsigned char *>(body));
    loggers::get_instance().log("http_codec::decode_body: decode_str: %s", p["decode_str"].c_str());
    // Try to identify xml
    if (p["decode_str"].find("<?xml version=") != std::string::npos) {
      loggers::get_instance().log("http_codec::decode_body: Find xml message");
      LibItsHttp__XmlMessageBodyTypes::XmlBody xml_body;
      // TODO To be refined adding a string identifier to check which codec to use. E.g. held_code.id() returns "xmlns=\"urn:ietf:params:xml:ns:geopriv:held\">"
      if ((p["decode_str"].find("=\"urn:ietf:params:xml:ns:geopriv:held\"") != std::string::npos) ||
          (p["decode_str"].find("=\"urn:ietf:params:xml:ns:pidf\"") != std::string::npos)) {
        loggers::get_instance().log("http_codec::decode_body: Find 'urn:ietf:params:xml:ns:geopriv:held'");
        if (_codecs["held"].get() != nullptr) {
          loggers::get_instance().log("http_codec::decode_body: Call 'held_codec'");
          if (_codecs["held"]->decode(body, (Record_Type &)xml_body, &p) == -1) {
            loggers::get_instance().warning("http_codec::decode_body: Failed to decode HELD message");
            xml_body.raw() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
          } else {
            loggers::get_instance().log_msg("http_codec::decode_body: Decoded message:", xml_body);
            message_body.xml__body() = xml_body;
          }
        } else {
          loggers::get_instance().warning("http_codec::decode_body: No codec for HELD");
          xml_body.raw() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
        }
        message_body.xml__body() = xml_body;
      } else if (p["decode_str"].find("=\"urn:ietf:params:xml:ns:lost1\"") != std::string::npos) {
        loggers::get_instance().log("http_codec::decode_body: Find 'urn:ietf:params:xml:ns:lost1'");
        if (_codecs["lost"].get() != nullptr) {
          loggers::get_instance().log("http_codec::decode_body: Call 'lost_codec'");
          if (_codecs["lost"]->decode(body, (Record_Type &)xml_body, &p) == -1) {
            loggers::get_instance().warning("http_codec::decode_body: Failed to decode LOST message");
            xml_body.raw() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
          } else {
            loggers::get_instance().log_msg("http_codec::decode_body: Decoded message:", xml_body);
            message_body.xml__body() = xml_body;
          }
        } else {
          loggers::get_instance().warning("http_codec::decode_body: No codec for LOST");
          xml_body.raw() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
        }
        message_body.xml__body() = xml_body;
      } else {
        loggers::get_instance().warning("http_codec::decode_body: No XML codec found");
        xml_body.raw()           = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
        message_body.xml__body() = xml_body;
      }
    } else if (p["decode_str"].find("<html>") != std::string::npos) { // Try to identify HTML
      loggers::get_instance().log("http_codec::decode_body: Find html message");
      LibItsHttp__MessageBodyTypes::HtmlBody html_body;
      message_body.html__body() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
    } else {
      loggers::get_instance().log("http_codec::decode_body: Use textBdy as default");
      LibItsHttp__MessageBodyTypes::TextBody text_body;
      message_body.text__body() = CHARSTRING(body.lengthof(), (char *)static_cast<const unsigned char *>(body));
    }
  }

  return 0;
}

int http_codec::get_line(TTCN_Buffer &buffer, CHARSTRING &to, const bool concatenate_header_lines) {
  unsigned int         i     = 0;
  const unsigned char *cc_to = buffer.get_read_data();

  // Sanity checks
  if (buffer.get_read_len() == 0) {
    return -1;
  }

  while (true) {
    // Skip spaces, and empty lines
    for (; i < buffer.get_read_len() && cc_to[i] != '\0' && cc_to[i] != '\r' && cc_to[i] != '\n'; i++)
      ;
    if (i >= buffer.get_read_len()) { // No more characters to process
      to = CHARSTRING("");
      return -1;
    } else if (cc_to[i] == '\n') { // New line found, we don't care is '\r' is missing
      if ((i > 0) && ((i + 1) < buffer.get_read_len()) && concatenate_header_lines && ((cc_to[i + 1] == ' ') || (cc_to[i + 1] == '\t'))) {
        i += 1; // Skip it
      } else {
        to = CHARSTRING(i, (const char *)cc_to);
        buffer.set_pos(buffer.get_pos() + i + 1);
        return i == 0 ? 1 : 0;
      }
    } else {
      if ((i + 1) < buffer.get_read_len() && cc_to[i + 1] != '\n') {
        return -1;
      } else if (i > 0 && (i + 2) < buffer.get_read_len() && concatenate_header_lines && (cc_to[i + 2] == ' ' || cc_to[i + 2] == '\t')) {
        i += 2;
      } else {
        to = CHARSTRING(i, (const char *)cc_to);
        buffer.set_pos(buffer.get_pos() + i + 2);
        return i == 0 ? 1 : 0;
      }
    }
  } // End of 'while' statement
}

void http_codec::set_payload_codecs(const std::string &p_codecs) {
  loggers::get_instance().log(">>> http_codec::set_payload_codecs: %s", p_codecs.c_str());

  // Sanity check
  if (p_codecs.length() == 0) {
    return;
  }

  // Extract codecs
  try {
    std::regex           rgx("(\\w+):(\\w+)(;(\\w+):(\\w+))*");
    std::sregex_iterator begin(p_codecs.cbegin(), p_codecs.cend(), rgx);
    std::sregex_iterator end = std::sregex_iterator();
    // E.g. 9 - xml - :held_codec - held_codec - ;html:html_codec - html:html_codec - html - :html_codec - html_codec
    for (std::sregex_iterator it = begin; it != end; ++it) {
      std::smatch m = *it;
      loggers::get_instance().log("http_codec::set_payload_codecs: %d - %s - %s - %s - %s - %s - %s - %s - %s", m.size(), m[1].str().c_str(),
                                  m[2].str().c_str(), m[3].str().c_str(), m[4].str().c_str(), m[5].str().c_str(), m[6].str().c_str(), m[7].str().c_str(),
                                  m[8].str().c_str());
      for (unsigned int j = 1; j < m.size() - 1; j += 3) { // Exclude m[0]
        loggers::get_instance().log("http_codec::set_payload_codecs: insert (%s, %s), j = %d", m[j].str().c_str(), m[j + 1].str().c_str(), j);
        if (m[j].str().empty()) {
          break;
        }
        std::string key(m[j].str());
        loggers::get_instance().log("http_codec::set_payload_codecs: Add codec %s", key.c_str());
        _codecs.insert(
                       std::make_pair(key, std::unique_ptr<codec<Record_Type, Record_Type>>(codec_stack_builder::get_instance()->get_codec(m[j + 1].str().c_str()))));
      } // End of 'for' statement
    }   // End of 'for' statement
    loggers::get_instance().log("http_codec::set_payload_codecs: _codecs length=%d - %p", _codecs.size(), this);
  } catch (const std::logic_error &e) {
    loggers::get_instance().warning("http_codec::set_payload_codecs: std::logic_error: %s", e.what());
    _codecs.clear();
  }
}
