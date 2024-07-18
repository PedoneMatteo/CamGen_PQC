#include "btp_types.hh"

#include "btp_layer_factory.hh"

#include "loggers.hh"

#include "converter.hh"

btp_layer::btp_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsBtp__TestSystem::BtpPort>(p_type), _params(), _codec(), _device_mode{true} {
  loggers::get_instance().log(">>> btp_layer::btp_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);

  // Sanity check
  params::const_iterator it = _params.find(params::btp_type);
  if (it == _params.cend()) {
    _params[params::btp_type] = std::string("btpB");
  }
  it = _params.find(params::btp_destination_port);
  if (it == _params.cend()) {
    _params[params::btp_destination_port] = std::to_string(2001);
  }
  it = _params.find(params::btp_info);
  if (it == _params.cend()) {
    _params[params::btp_info] = std::to_string(0);
  }
  it = _params.find(params::device_mode);
  if (it != _params.cend()) {
    _device_mode = (1 == converter::get_instance().string_to_int(it->second));
  }
}

void btp_layer::sendMsg(const LibItsBtp__TypesAndValues::BtpReq &p, params &params) {
  loggers::get_instance().log(">>> btp_layer::sendMsg");
  // params.log();

  // Encode BTP PDU
  OCTETSTRING data;
  _codec.encode(p.msgOut(), data);
  send_data(data, params);
}

void btp_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> btp_layer::send_data: ", data);
  params.log();  // TODO To be removed
  _params.log(); // TODO To be removed

  if (_device_mode) {
    LibItsBtp__TypesAndValues::BtpHeader header;
    std::string                          btp_type;
    params::const_iterator               it = params.find(params::next_header);
    if (it != params.cend()) {
      btp_type = it->second;
    } else {
      btp_type = _params[params::btp_type];
    }
    loggers::get_instance().log("btp_layer::send_data: btp_type=%s", btp_type.c_str());
    int btp_destination_port = std::stoi(_params[params::btp_destination_port]); // Default value
    it                       = params.find(params::btp_destination_port);
    if (it != params.cend()) {
      btp_destination_port = std::stoi(params[params::btp_destination_port]); // Overwritting default value
    }
    loggers::get_instance().log("btp_layer::send_data: btp_destination_port=%d", btp_destination_port);
    if (btp_type.compare("btpA") == 0) {
      header.btpAHeader() = LibItsBtp__TypesAndValues::BtpAHeader(btp_destination_port, std::stoi(_params[params::btp_info]));
    } else {
      header.btpBHeader() = LibItsBtp__TypesAndValues::BtpBHeader(btp_destination_port, std::stoi(_params[params::btp_info]));
    }
    LibItsBtp__TypesAndValues::BtpPacket p(header, data);
    loggers::get_instance().log_msg("btp_layer::send_data: ", p);

    // Encode BTP PDU
    OCTETSTRING os;
    _codec.encode(p, os);
    data = os;
  }

  send_to_all_layers(data, params);
}

void btp_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> btp_layer::receive_data: ", data);
  params.log();
  // Decode the payload
  LibItsBtp__TypesAndValues::BtpInd p;
  loggers::get_instance().log("btp_layer::receive_data: Looking for %s", params::gn_next_header.c_str());
  params::const_iterator it = params.find(params::gn_next_header);
  if (it != params.cend()) {
    loggers::get_instance().log("btp_layer::receive_data: Call set_btp_type with %s/%d", it->second.c_str(),
                                (it->second.compare("2") == 0) ? btp_codec::btpB : btp_codec::btpA);
    _codec.set_btp_type((it->second.compare("2") == 0) ? btp_codec::btpB : btp_codec::btpA);
  }
  _codec.decode(data, p.msgIn(), &params);

  // Pass the BTP raw payload to the upper layers if any
  it = params.find(params::btp_payload);
  if (it != params.cend()) {
    loggers::get_instance().log("btp_layer::receive_data: btp_payload=%s", it->second.c_str());
    OCTETSTRING os(str2oct(CHARSTRING(it->second.c_str())));
    receive_to_all_layers(os, params);
  } else {
    loggers::get_instance().warning("btp_layer::receive_data: No payload to pass to upper layers");
  }
  // Pass it to the ports if any
  // params.log();
  to_all_upper_ports(p, params);
}

btp_layer_factory btp_layer_factory::_f;
