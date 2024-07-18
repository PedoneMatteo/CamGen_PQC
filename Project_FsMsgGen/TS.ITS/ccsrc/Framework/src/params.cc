/*!
 * \file      params.cc
 * \brief     Source file for the parameter dictionary.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include <regex>
#include <stdexcept>

#include "loggers.hh"
#include "params.hh"

const std::string& params::debug = std::string("debug");

const std::string& params::loopback = std::string("loopback");

const std::string& params::mac_src              = std::string("mac_src");
const std::string& params::mac_dst              = std::string("mac_dst");
const std::string& params::mac_bc               = std::string("mac_bc");
const std::string& params::eth_type             = std::string("eth_type");
const std::string& params::filter               = std::string("filter");
const std::string& params::beaconing            = std::string("beaconing");
const std::string& params::timestamp            = std::string("timestamp");
const std::string& params::station_type         = std::string("station_type");
const std::string& params::country              = std::string("country");
const std::string& params::type_of_address      = std::string("type_of_address");
const std::string& params::its_aid              = std::string("its_aid");
const std::string& params::ssp                  = std::string("ssp");
const std::string& params::gn_payload           = std::string("gn_payload");
const std::string& params::gn_next_header       = std::string("gnNextHeader");
const std::string& params::gn_header_type       = std::string("gnHeaderType");
const std::string& params::gn_header_sub_type   = std::string("gnHeaderSubtype");
const std::string& params::gn_lifetime          = std::string("gnLifetime");
const std::string& params::gn_traffic_class     = std::string("gnTrafficClass");
const std::string& params::btp_type             = std::string("btp_type");
const std::string& params::btp_payload          = std::string("btp_payload");
const std::string& params::btp_destination_port = std::string("dst_port");
const std::string& params::btp_info             = std::string("btp_info");

const std::string& params::nic        = std::string("nic");
const std::string& params::latitude   = std::string("latitude");
const std::string& params::longitude  = std::string("longitude");
const std::string& params::ll_address = std::string("ll_address");
const std::string& params::expiry     = std::string("expiry");

const std::string& params::station_id = std::string("station_id");

const std::string& params::device_mode            = std::string("device_mode");
const std::string& params::secured_mode           = std::string("secured_mode");
const std::string& params::encrypted_mode         = std::string("encrypted_mode");
const std::string& params::enable_security_checks = std::string("enable_security_checks");
const std::string& params::force_certificate      = std::string("force_certificate");
const std::string& params::certificate            = std::string("certificate");
const std::string& params::peer_certificate       = std::string("peer_certificate");
const std::string& params::sec_db_path            = std::string("sec_db_path");
const std::string& params::hash                   = std::string("hash");
const std::string& params::signature              = std::string("signature");
const std::string& params::cypher                 = std::string("cypher");
const std::string& params::distanceA              = std::string("distanceA");
const std::string& params::distanceB              = std::string("distanceB");
const std::string& params::angle                  = std::string("angle");

const std::string& params::payload_type = std::string("payload_type");

const std::string& params::next_header     = std::string("next_header");
const std::string& params::header_type     = std::string("header_type");
const std::string& params::header_sub_type = std::string("header_sub_type");

const std::string& params::interface_id = std::string("interface_id");

const std::string& params::uu_protocol  = std::string("uu_protocol");
const std::string& params::uu_transport = std::string("uu_transport");
const std::string& params::pc5_layer = std::string("pc5_layer");
const std::string& params::mqtt_topics  = std::string("mqtt_topics");
const std::string& params::mqtt_protocol  = std::string("mqtt_protocol");
const std::string& params::mqtt_client_id  = std::string("mqtt_client_id");

const std::string& params::server      = std::string("server");
const std::string& params::port        = std::string("port");
const std::string& params::use_ssl     = std::string("use_ssl");
const std::string& params::mutual_auth = std::string("mutual_auth");
const std::string& params::trusted_ca_list = std::string("trusted_ca_list");

const std::string& params::server_mode = std::string("server_mode");
const std::string& params::local_port  = std::string("local_port");

const std::string& params::method       = std::string("method");
const std::string& params::uri          = std::string("uri");
const std::string& params::host         = std::string("host");
const std::string& params::content_type = std::string("content_type");

// const std::string& params::sip_url = std::string("sip_url");
// const std::string& params::sip_version = std::string("sip_version");
// const std::string& params::payload = std::string("payload");

const std::string& params::codecs = std::string("codecs");

void params::convert(params &p_param, const std::string p_parameters) {
  // Sanity checks
  if (p_parameters.length() == 0) {
    return;
  }
  loggers::get_instance().log(">>> params::convert: %s", p_parameters.c_str());
  // Extract parameters
  try {
    std::regex           rgx("(\\w+)=(.*?)(,|$)");
    std::sregex_iterator begin(p_parameters.cbegin(), p_parameters.cend(), rgx);
    std::sregex_iterator end = std::sregex_iterator();
    for (std::sregex_iterator it = begin; it != end; ++it) {
      std::smatch m = *it;
      loggers::get_instance().log("params::convert: %d - %s - %s - %s - %s", m.size(), m[0].str().c_str(), m[1].str().c_str(), m[2].str().c_str(),
                                  m[3].str().c_str());
      p_param.insert(std::pair<std::string, std::string>(m[1].str(), m[2].str()));
    } // End of 'for' statement
  } catch (const std::logic_error &e) {
    p_param.clear();
  }
  loggers::get_instance().log("<<< params::convert");
}

void params::log() const {
  loggers::get_instance().log("params::log");
  if (size() == 0) {
    loggers::get_instance().log("\tEmpty");
  } else {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      loggers::get_instance().log("\t(%s, %s)", it->first.c_str(), it->second.c_str());
    } // End of 'for' statement
  }
}

void params::log() {
  loggers::get_instance().log("params::log");
  if (size() == 0) {
    loggers::get_instance().log("\tEmpty");
  } else {
    for (const_iterator it = cbegin(); it != cend(); ++it) {
      loggers::get_instance().log("\t(%s, %s)", it->first.c_str(), it->second.c_str());
    } // End of 'for' statement
  }
}

void params::reset() {
  loggers::get_instance().log("params::reset");
  for (iterator it = begin(); it != end(); ++it) {
    it->second.clear();
  } // End of 'for' statement
}
