#include "LibItsHttp_TypesAndValues.hh"

#include "codec_stack_builder.hh"
#include "http_layer_factory.hh"

#include "loggers.hh"

#include "converter.hh"

using namespace std; // Required for isnan()
#include "LibItsHttp_TestSystem.hh"
#include "LibItsHttp_TypesAndValues.hh"

http_layer::http_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsHttp__TestSystem::HttpPort>(p_type), _params(), _device_mode{false} {
  loggers::get_instance().log(">>> http_layer::http_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);

  params::const_iterator it = _params.find(params::codecs);
  if (it != _params.cend()) {
    _codec.set_payload_codecs(it->second);
  }
  it = _params.find(params::device_mode);
  if (it != _params.cend()) {
    _device_mode = (1 == converter::get_instance().string_to_int(it->second));
  }
  it = _params.find(params::method);
  if (it == _params.cend()) {
    _params[params::method] = "POST";
  }
  it = _params.find(params::uri);
  if (it == _params.cend()) {
    _params[params::uri] = "/";
  }
  it = _params.find(params::host);
  if (it == _params.cend()) {
    _params[params::host] = "127.0.0.1";
  }
  it = _params.find(params::content_type);
  if (it == _params.cend()) {
    _params[params::content_type] = "application/text";
  }
}

void http_layer::sendMsg(const LibItsHttp__TypesAndValues::HttpMessage &p_http_message, params &p_param) {
  loggers::get_instance().log_msg(">>> http_layer::sendMsg: ", p_http_message);

  // Encode HttpMessage
  OCTETSTRING data;
  _codec.encode(p_http_message, data);
  send_data(data, _params);
}

void http_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> http_layer::send_data: ", data);

  if (_device_mode) { // Need to build an HTTP packet
    loggers::get_instance().log("http_layer::send_data: Build http layer");
    TTCN_Buffer buffer;
    buffer.put_cs(_params[params::method].c_str());
    buffer.put_c(' ');
    buffer.put_cs(_params[params::uri].c_str());
    buffer.put_cs(" HTTP/1.1\r\n");
    buffer.put_cs("Host: ");
    buffer.put_cs(_params[params::host].c_str());
    buffer.put_cs("\r\n");
    buffer.put_cs("Content-type: ");
    buffer.put_cs(_params[params::content_type].c_str());
    buffer.put_cs("\r\n");
    buffer.put_cs("Content-length: ");
    buffer.put_cs(static_cast<const char *>(int2str(data.lengthof() + 2 /*Stand for the last CRLF*/)));
    buffer.put_cs("\r\n\r\n");
    buffer.put_os(data);
    buffer.put_cs("\r\n");
    data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  }

  loggers::get_instance().log_msg("http_layer::send_data: ", data);
  send_to_all_layers(data, params);
}

void http_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> http_layer::receive_data: ", data);

  // Decode HTTP message
  LibItsHttp__TypesAndValues::HttpMessage http_message;
  if (_codec.decode(data, http_message) == -1) {
    loggers::get_instance().warning("http_layer::receive_data: Failed to decode data");
    return;
  }
  if (_device_mode) {
    OCTETSTRING os;
    if (http_message.ischosen(LibItsHttp__TypesAndValues::HttpMessage::ALT_response)) {
      if (http_message.response().body().ispresent()) {
        LibItsHttp__MessageBodyTypes::HttpMessageBody &body =
          static_cast<LibItsHttp__MessageBodyTypes::HttpMessageBody &>(*http_message.response().body().get_opt_value());
        if (body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_binary__body)) {
          LibItsHttp__BinaryMessageBodyTypes::BinaryBody &binary = body.binary__body();
          if (binary.ischosen(LibItsHttp__BinaryMessageBodyTypes::BinaryBody::ALT_raw)) {
            os = binary.raw();
          } else {
            loggers::get_instance().warning("http_layer::receive_data: A raw binary payload is expected");
          }
        } else if (body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_html__body)) {
          // TODO To be done
          loggers::get_instance().warning("http_layer::receive_data: Not implemented yet");
        } else if (body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_xml__body)) {
          // TODO To be done
          loggers::get_instance().warning("http_layer::receive_data: Not implemented yet");
        } else if (body.ischosen(LibItsHttp__MessageBodyTypes::HttpMessageBody::ALT_text__body)) {
          // TODO To be done
          loggers::get_instance().warning("http_layer::receive_data: Not implemented yet");
        }
        receive_to_all_layers(os, params);
      } else {
        loggers::get_instance().warning("http_layer::receive_data: No body present");
      }
    } else {
      loggers::get_instance().warning("http_layer::receive_data: An HTTP response is expected");
    }
  } else {
    // Pass it to the ports
    to_all_upper_ports(http_message, params);
  }
}

http_layer_factory http_layer_factory::_f;
