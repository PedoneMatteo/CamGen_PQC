#include "loggers.hh"

#include "uu_raw.hh"

#include "lte_udp_layer.hh"
#include "lte_tcp_layer.hh"

uu_raw::uu_raw(): _uu_transport("udp") {
  loggers::get_instance().log(">>> uu_raw::uu_raw");
}

uu_raw::~uu_raw() {
  loggers::get_instance().log(">>> uu_raw::~uu_raw");

  close();
}

void uu_raw::close() {
  loggers::get_instance().log(">>> uu_raw::close");

  if (_layer != nullptr) {
    loggers::get_instance().log("uu_raw::close: Send Disconnect command");
    delete _layer;
  }
}

int uu_raw::setup(const params &params) {
  loggers::get_instance().log(">>> uu_raw::setup");

  // Transport layer
  params::const_iterator it = params.find(params::uu_transport);
  if (it != params.cend()) {
    if (it->second.compare("tcp") == 0) {
      loggers::get_instance().log("uu_raw::setup: Create TCP layer");
      _uu_transport.assign("tcp");
      _layer = new lte_tcp_layer("TCP", params);
      ((lte_tcp_layer*)_layer)->incoming_packet_observer_attach(this);
    } else {
      loggers::get_instance().log("uu_raw::setup: Create UDP layer");
      _layer = new lte_udp_layer("UDP", params);
      ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
    }
  } else {
    loggers::get_instance().log("uu_raw::setup: Create UDP layer");
    _layer = new lte_udp_layer("UDP", params);
    ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
  }
  loggers::get_instance().log("uu_raw::setup: _layer %p", _layer);

  return 0;
}

int uu_raw::shutdown() {
  loggers::get_instance().log(">>> uu_raw::shutdown");

  // Don't delete transport layer

  return 0;
}

void uu_raw::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> uu_raw::send_data");
  _layer->send_data(data, params);
}

void uu_raw::receive_data(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log(">>> uu_raw::receive_data");

  incoming_packet_notify(p_data, p_params);
}

void uu_raw::update(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> uu_raw::update: ", p_data);

  receive_data(p_data, p_params);
}
