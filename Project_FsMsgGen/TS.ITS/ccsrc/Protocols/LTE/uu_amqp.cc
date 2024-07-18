#include "loggers.hh"

#include "uu_amqp.hh"

#include "lte_udp_layer.hh"
#include "lte_tcp_layer.hh"

#include "data_event_observer.hh"

uu_amqp::uu_amqp(): _uu_transport("udp") {
  loggers::get_instance().log(">>> uu_amqp::uu_amqp");
}

uu_amqp::~uu_amqp() {
  loggers::get_instance().log(">>> uu_amqp::~uu_amqp");

  close();
}

void uu_amqp::close() {
  loggers::get_instance().log(">>> uu_amqp::close");

  if (_layer != nullptr) {
    loggers::get_instance().log("uu_amqp::close: Send Disconnect command");
    delete _layer;
  }
}

int uu_amqp::setup(const params &params) {
  loggers::get_instance().log(">>> uu_amqp::setup");

  // Transport layer to the broker
  params::const_iterator it = params.find(params::uu_transport);
  if (it != params.cend()) {
    if (it->second.compare("tcp") == 0) {
      loggers::get_instance().log("uu_amqp::setup: Create TCP layer");
      _uu_transport.assign("tcp");
      _layer = new lte_tcp_layer("TCP", params);
      ((lte_tcp_layer*)_layer)->incoming_packet_observer_attach(this);
    } else {
      loggers::get_instance().log("uu_amqp::setup: Create UDP layer");
      _layer = new lte_udp_layer("UDP", params);
      ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
    }
  } else {
    loggers::get_instance().log("uu_amqp::setup: Create UDP layer");
    _layer = new lte_udp_layer("UDP", params);
    ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
  }
  loggers::get_instance().log("uu_amqp::setup: _layer %p", _layer);

  return 0;
}

int uu_amqp::shutdown() {
  loggers::get_instance().log(">>> uu_amqp::shutdown");

  // Don't delete transport layer

  return 0;
}

void uu_amqp::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> uu_amqp::send_data");
  _layer->send_data(data, params);
}

void uu_amqp::receive_data(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log(">>> uu_amqp::receive_data");

}

void uu_amqp::update(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> uu_amqp::update: ", p_data);

  receive_data(p_data, p_params);
}
