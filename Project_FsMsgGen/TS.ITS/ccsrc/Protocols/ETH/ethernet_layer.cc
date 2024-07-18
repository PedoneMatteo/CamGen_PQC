#include "ethernet_layer_factory.hh"

#include "loggers.hh"

ethernet_layer::ethernet_layer(const std::string & p_type, const std::string & param) : layer(p_type), _params() {
  loggers::get_instance().log(">>> ethernet_layer::ethernet_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  params::const_iterator it = _params.find("mac_src");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("mac_src"), "000000000000"));
  }
  it = _params.find("mac_bc");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("mac_bc"), "FFFFFFFFFFFF"));
  }
  it = _params.find("eth_type");
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("eth_type"), "8947"));
  }
  //_params.log();
}

void ethernet_layer::send_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log_msg(">>> ethernet_layer::send_data: ", data);

  OCTETSTRING eth;
  // Destination MAC address
  params::const_iterator it = params.find(params::mac_dst); // Find in provided parameters, params
  if (it != params.cend()) {
    eth = str2oct(CHARSTRING(it->second.c_str()));
  } else {
    it = _params.find(params::mac_dst);
    if (it != _params.cend()) {
      eth = str2oct(CHARSTRING(it->second.c_str()));
    } else {
      eth = str2oct(CHARSTRING(_params[params::mac_bc].c_str()));
    }
  }
  // Source MAC address
  it = params.find(params::mac_src); // Find in provided parameters, params
  if (it != params.cend()) {
    eth += str2oct(CHARSTRING(it->second.c_str()));
  } else {
    eth += str2oct(CHARSTRING(_params[params::mac_src].c_str()));
  }
  // Ethernet type
  it = params.find(params::eth_type); // Find in layer parameters
  if (it != params.cend()) {
    eth += str2oct(CHARSTRING(it->second.c_str()));
  } else {
    eth += str2oct(CHARSTRING(_params[params::eth_type].c_str()));
  }
  
  eth += data;
  send_to_all_layers(eth, params);
}

void ethernet_layer::receive_data(OCTETSTRING& data, params& params) {
  loggers::get_instance().log_msg(">>> ethernet_layer::receive_data: ", data);

  // Extract dest MAC Address
  OCTETSTRING dst = OCTETSTRING(6, static_cast<const unsigned char *>(data));
  //loggers::get_instance().log_msg("ethernet_layer::receive_data: dst: ", dst);
  // Extract source MAC Address
  OCTETSTRING src = OCTETSTRING(6, 6 + static_cast<const unsigned char *>(data));
  //loggers::get_instance().log_msg("ethernet_layer::receive_data: src: ", src);
  // Extract ethertype
  OCTETSTRING proto = OCTETSTRING(2, 2 + static_cast<const unsigned char *>(data));
  //loggers::get_instance().log_msg("ethernet_layer::receive_data: proto: ", proto);
  data = OCTETSTRING(data.lengthof() - 14, 14 + static_cast<const unsigned char *>(data));
  // Update params
  CHARSTRING s = oct2str(dst);
  params.insert(std::pair<std::string, std::string>(params::mac_dst, std::string(static_cast<const char *>(s))));
  s = oct2str(src);
  params.insert(std::pair<std::string, std::string>(params::mac_src, std::string(static_cast<const char *>(s))));
  //loggers::get_instance().log_msg("ethernet_layer::receive_data: payload for upper layer:", data);
  
  receive_to_all_layers(data, params);
}

ethernet_layer_factory ethernet_layer_factory::_f;
