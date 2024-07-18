#include "SremSsemTypes.hh"

#include "SremSsemLayerFactory.hh"
#include "geonetworking_layer_factory.hh"

#include "registration.hh"

#include "loggers.hh"

SremSsemLayer::SremSsemLayer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsSremSsem__TestSystem::SremSsemPort>(p_type), _params(), _srem_codec(), _ssem_codec() {
  loggers::get_instance().log(">>> SremSsemLayer::SremSsemLayer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  _params.insert(std::make_pair<std::string, std::string>("its_aid", "140")); // ETSI TS 102 965 V1.2.1 (2015-06)
  _params.insert(std::make_pair<std::string, std::string>("payload_type", "2"));
  _params.insert(std::make_pair<std::string, std::string>("dst_port", "2007"));

  // Register this object for AdapterControlPort
  loggers::get_instance().log("SremSsemLayer::SremSsemLayer: Register %s/%p", p_type.c_str(), this);
  registration<SremSsemLayer>::get_instance().add_item(p_type, this);
}

void SremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::SremReq &p, params &params) {
  loggers::get_instance().log_msg(">>> SremSsemLayer::sendMsg: ", p);

  // Encode srem PDU
  OCTETSTRING data;
  if (_srem_codec.encode(p.msgOut(), data) == -1) {
    loggers::get_instance().warning("SremSsemLayer::sendMsg: Encoding failure");
    return;
  }

  _params[params::btp_destination_port] = "2007";
  send_data(data, _params);
}

void SremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::SsemReq &p, params &params) {
  loggers::get_instance().log_msg(">>> SremSsemLayer::sendMsg: ", p);

  // Encode ssem PDU
  OCTETSTRING data;
  if (_ssem_codec.encode(p.msgOut(), data) == -1) {
    loggers::get_instance().warning("SremSsemLayer::sendMsg: Encoding failure");
    return;
  }
  _params[params::btp_destination_port] = "2008";
  send_data(data, _params);
}

void SremSsemLayer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> SremSsemLayer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, params);
}

void SremSsemLayer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> SremSsemLayer::receive_data: ", data);

  // Sanity check
  if (*(static_cast<const unsigned char *>(data) + 1) == 0x09) { // Check that received packet has SRE message id
    // Decode the SREM payload
    LibItsSremSsem__TypesAndValues::SremInd p;
    _srem_codec.decode(data, p.msgIn());
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
      loggers::get_instance().log("SremSsemayer::receive_data: ssp=%s", it->second.c_str());
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
  } else if (*(static_cast<const unsigned char *>(data) + 1) == 0x0A) { // Check that received packet has SSE message id
    // Decode the SSEM payload
    LibItsSremSsem__TypesAndValues::SsemInd p;
    _ssem_codec.decode(data, p.msgIn());
    if (!p.msgIn().is_bound()) {
      // Discard it
      return;
    } // else, continue
    // Process lower layer data
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
      loggers::get_instance().log("SremSsemLayer::receive_data: ssp=%s", it->second.c_str());
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
  } else {
    // Not a DENM message, discard it
    loggers::get_instance().warning("SremSsemLayer::receive_data: Wrong message id: 0x%02x", *(static_cast<const unsigned char *>(data) + 1));
    return;
  }
}

int SremSsemLayer::enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security) {
  loggers::get_instance().log(">>> SremSsemLayer::enable_secured_mode: '%s' - %x", p_certificate_id.c_str(), p_enforce_security);

  geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
  if (p == NULL) {
    return -1;
  }
  
  loggers::get_instance().log("SremSsemLayer::enable_secured_mode: Got GN layer %p", p);
  return p->enable_secured_mode(p_certificate_id, p_enforce_security);
}

int SremSsemLayer::disable_secured_mode() {
  loggers::get_instance().log(">>> SremSsemLayer::disable_secured_mode");

  geonetworking_layer *p = registration<geonetworking_layer>::get_instance().get_item(std::string("GN"));
  if (p == NULL) {
    return -1;
  }
  
  loggers::get_instance().log("SremSsemLayer::disable_secured_mode: Got GN layer %p", p);
  return p->disable_secured_mode();
}

SremSsemLayerFactory SremSsemLayerFactory::_f;
