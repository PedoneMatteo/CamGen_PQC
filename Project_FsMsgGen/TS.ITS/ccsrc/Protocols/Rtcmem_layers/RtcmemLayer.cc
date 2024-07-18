#include "RtcmemLayer.hh"

#include "LibItsRtcmem_TestSystem.hh"
#include "geonetworking_layer_factory.hh"

#include "RtcmemLayerFactory.hh"

#include "registration.hh"

#include "loggers.hh"

RtcmemLayer::RtcmemLayer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsRtcmem__TestSystem::RtcmemPort>(p_type), _params(), _rtcmem_codec() {
  loggers::get_instance().log(">>> RtcmemLayer::RtcmemLayer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  _params.insert(std::make_pair<std::string, std::string>("its_aid", "128"));     // ETSI TS 102 965 V1.2.1 (2015-06)
  _params.insert(std::make_pair<std::string, std::string>("payload_type", "13")); // RTCME message id - See ETSI TS 103 301

  // Register this object for AdapterControlPort
  loggers::get_instance().log("RtcmemLayer::RtcmemLayer: Register %s/%p", p_type.c_str(), this);
  registration<RtcmemLayer>::get_instance().add_item(p_type, this);
}

void RtcmemLayer::sendMsg(const LibItsRtcmem__TypesAndValues::RtcmemReq &p, params &params) {
  loggers::get_instance().log_msg(">>> RtcmemLayer::sendMsg: ", p);

  // Encode rtcmem PDU
  OCTETSTRING data;
  if (_rtcmem_codec.encode(p.msgOut(), data) == -1) {
    loggers::get_instance().warning("RtcmemLayer::sendMsg: Encoding failure");
    return;
  }
  send_data(data, _params);
}

void RtcmemLayer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> RtcmemLayer::send_data: ", data);
  send_to_all_layers(data, params);
}

void RtcmemLayer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> RtcmemLayer::receive_data: ", data);

  // Decode the RTCMEM payload
  LibItsRtcmem__TypesAndValues::RtcmemInd p;
  _rtcmem_codec.decode(data, p.msgIn());
  if (!p.msgIn().is_bound()) {
    // Discard it
    return;
  } // else, continue

  // Process lower layer data
  // recvTime
  params::const_iterator it = params.find(params::timestamp);
  if (it != params.cend()) {
    p.recvTime().set_long_long_val(std::stoll(it->second));
  } else {
    p.recvTime().set_to_omit();
  }
  // gnNextHeader
  it = params.find(params::gn_next_header);
  if (it != params.cend()) {
    p.gnNextHeader() = std::stoi(it->second);
  } else {
    p.gnNextHeader().set_to_omit();
  }
  // gnHeaderType
  it = params.find(params::gn_header_type);
  if (it != params.cend()) {
    p.gnHeaderType() = std::stoi(it->second);
  } else {
    p.gnHeaderType().set_to_omit();
  }
  // gnHeaderSubtype
  it = params.find(params::gn_header_sub_type);
  if (it != params.cend()) {
    p.gnHeaderSubtype() = std::stoi(it->second);
  } else {
    p.gnHeaderSubtype().set_to_omit();
  }
  // gnLifetime
  it = params.find(params::gn_lifetime);
  if (it != params.cend()) {
    p.gnLifetime() = std::stoi(it->second);
  } else {
    p.gnLifetime().set_to_omit();
  }
  // gnTrafficClass
  it = params.find(params::gn_traffic_class);
  if (it != params.cend()) {
    p.gnTrafficClass() = std::stoi(it->second);
  } else {
    p.gnTrafficClass().set_to_omit();
  }
  // btpDestinationPort
  it = params.find(params::btp_destination_port);
  if (it != params.cend()) {
    p.btpDestinationPort() = std::stoi(it->second);
  } else {
    p.btpDestinationPort().set_to_omit();
  }
  // btpInfo
  it = params.find(params::btp_info);
  if (it != params.cend()) {
    p.btpInfo() = std::stoi(it->second);
  } else {
    p.btpInfo().set_to_omit();
  }
  // ssp
  it = params.find(params::ssp);
  if (it != params.cend()) {
    loggers::get_instance().log("RtcmemLayer::receive_data: ssp=%s", it->second.c_str());
    p.ssp() = oct2bit(str2oct(CHARSTRING(it->second.c_str())));
  } else {
    p.ssp().set_to_omit();
  }
  // its_aid
  it = params.find(params::its_aid);
  if (it != params.cend()) {
    p.its__aid() = std::stoi(it->second);
  } else {
    p.its__aid().set_to_omit();
  }

  // Pass it to the ports if any
  to_all_upper_ports(p, params);
}

int RtcmemLayer::enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security) {
  loggers::get_instance().log(">>> RtcmemLayer::enable_secured_mode: '%s' - %x", p_certificate_id.c_str(), p_enforce_security);

  geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
  if (p == NULL) {
    return -1;
  }
  
  loggers::get_instance().log("RtcmemLayer::enable_secured_mode: Got GN layer %p", p);
  return p->enable_secured_mode(p_certificate_id, p_enforce_security);
}

int RtcmemLayer::disable_secured_mode() {
  loggers::get_instance().log(">>> RtcmemLayer::disable_secured_mode");

  geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
  if (p == NULL) {
    return -1;
  }
  
  loggers::get_instance().log("RtcmemLayer::disable_secured_mode: Got GN layer %p", p);
  return p->disable_secured_mode();
}

RtcmemLayerFactory RtcmemLayerFactory::_f;
