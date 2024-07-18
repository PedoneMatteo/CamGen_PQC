#include "loggers.hh"

#include "uu_raw.hh"
#include "uu_mqtt.hh"
#include "uu_amqp.hh"

#include "lte_layer_factory.hh"

#include "registration.hh"

lte_layer::lte_layer(const std::string &p_type, const std::string &params)
  : _uu_transport("udp"), _uu_protocol(""), _pc5_layer_id(""), _uu_layer{0}, _pc5_layer{0}, _pc5{0}, _time_key("lte_layer::Handle_Fd_Event_Readable") {
  loggers::get_instance().log(">>> lte_layer::lte_layer: %s, %s", to_string().c_str(), params.c_str());

  params::convert(_params, params);

  // Uu interface
  params::const_iterator it = _params.find(params::uu_transport);
  if (it != _params.cend()) {
    if (it->second.compare("tcp") == 0) {
      _uu_transport.assign("tcp");
    }
  }
  it = _params.find(params::uu_protocol);
  if (it == _params.cend()) {
    // No protocol specify, use raw V2X message
    setup_raw_v2x();
  } else if (it->second.compare("mqtt") == 0) {
    // Use MQTT
    setup_mqtt_v2x();
  } else if (it->second.compare("amqp") == 0) {
    // Use AMQP
    setup_amqp_v2x();
  } else {
    // No protocol specify, use raw V2X message
    setup_raw_v2x();
  }
  loggers::get_instance().log("lte_layer::lte_layer: Uu interface up: %p", _uu_layer);
  _uu_layer->incoming_packet_observer_attach(this);

  // PC#5 interface
  it = _params.find(params::pc5_layer);
  if (it == _params.cend()) {
    _pc5_layer_id.assign("QUALCOMM");
  } else {
    _pc5_layer_id = it->second;
  }
  std::string s(_pc5_layer_id + std::string("/") + std::string("ETH(") + params::mac_src + "=" + _params[params::mac_src] + std::string(")/PCAP(" + params::mac_src + "=" + _params[params::mac_src] + "," + params::nic + "=" + _params[params::nic] + "," + params::filter + "=" + _params[params::filter] + ")"));
  loggers::get_instance().log("lte_layer::lte_layer: PC5 layers string: '%s'", s.c_str());
  _pc5_layer = layer_stack_builder::get_instance()->create_layer_stack(s.c_str());
  loggers::get_instance().log("lte_layer::lte_layer: _pc5_layer= %p", _pc5_layer);
  _pc5 = registration<qualcomm_layer>::get_instance().get_item(_pc5_layer_id);
  if (_pc5 == NULL) {
    loggers::get_instance().error("lte_layer::lte_layer: Failed to get %s layer", _pc5_layer_id.c_str());
  }
  loggers::get_instance().log("cam_layer::disable_secured_mode: Got QUALCOMM layer %p", _pc5);
  _pc5->incoming_packet_observer_attach(this);
  loggers::get_instance().log("lte_layer::lte_layer PC5 interface up");
}

lte_layer::~lte_layer() {
  loggers::get_instance().log(">>> lte_layer::~lte_layer");

  close();
}

void lte_layer::close() {
  loggers::get_instance().log(">>> lte_layer::close");

  // Uu interface
  if (_uu_layer != nullptr) {
    _uu_layer->shutdown();
    delete _uu_layer;
  }
  // PC#5 interface
  if (_pc5_layer != nullptr) {
    _pc5->incoming_packet_observer_detach(this);
    delete _pc5_layer;
    _pc5_layer = nullptr;
  }
}

void lte_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> lte_layer::send_data: ", data);

  params::const_iterator it = params.find(std::string("its_aid"));
  /*its_aid=137:  SPAT
    its_aid=138:  MAP
    its_aid=139:  IVIM
    its_aid=140:  SREM/SSEM
    its_aid=141:  GeoMet
    its_aid=128:  RTCMEM
  */
  loggers::get_instance().log("lte_layer::send_data: %s", it->second.c_str());
  if (it == params.cend()) {
    _uu_layer->send_data(data, params);
  } else if ((it->second.compare("36") == 0) || (it->second.compare("141") == 0)) { // CAM and Beacons over PC#5
     _pc5_layer->send_data(data, params);
  } else {
    _uu_layer->send_data(data, params);
  }
}

void lte_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> lte_layer::receive_data: ", data);

  receive_to_all_layers(data, params);
}

void lte_layer::Handle_Fd_Event_Readable(int fd) {
  loggers::get_instance().log(">>> lte_layer::Handle_Fd_Event_Readable: %d", fd);

}

void lte_layer::update(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> lte_layer::update: ", p_data);
  p_params.log();
  
  receive_data(p_data, p_params);
}

void lte_layer::setup_raw_v2x() {
  loggers::get_instance().log(">>> lte_layer::setup_raw_v2x");

  _uu_protocol.assign("raw");
  _uu_layer = new uu_raw();
  _uu_layer->setup(_params);
  loggers::get_instance().log("<<< lte_layer::setup_raw_v2x: %p", _uu_layer);
}

void lte_layer::setup_mqtt_v2x() {
  loggers::get_instance().log(">>> lte_layer::setup_mqtt_v2x");

  _uu_protocol.assign("mqtt");
  _uu_layer = new uu_mqtt();
  _uu_layer->setup(_params);
}

void lte_layer::setup_amqp_v2x() {
  loggers::get_instance().log(">>> lte_layer::setup_amqp_v2x");

  _uu_protocol.assign("amqp");
  _uu_layer = new uu_amqp();
  _uu_layer->setup(_params);
}

lte_layer_factory lte_layer_factory::_f;
