#include <algorithm>
#include <arpa/inet.h>
#include <cctype>

#include "qualcomm_layer_factory.hh"

#include "registration.hh"

#include "loggers.hh"

#include "converter.hh"

qualcomm_layer::qualcomm_layer(const std::string &p_type, const std::string &param) : layer(p_type), _params() {
  loggers::get_instance().log(">>> qualcomm_layer::qualcomm_layer: %s, %s", to_string().c_str(), param.c_str());

  // Setup parameters
  params::convert(_params, param);
  //_params.log();

  // Register this object for LTE layer
  loggers::get_instance().log("qualcomm_layer::qualcomm_layer: Register %s/%p", p_type.c_str(), this);
  registration<qualcomm_layer>::get_instance().add_item(p_type, this);
}

void qualcomm_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> qualcomm_layer::send_data: ", data);

  OCTETSTRING buffer = int2oct(3, 1); // CV2X identifier
  buffer += data;                     // Payload

  loggers::get_instance().log_msg("qualcomm_layer::send_data: Final buffer=", buffer);
  send_to_all_layers(buffer, params);
}

void qualcomm_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> qualcomm_layer::receive_data: Received %d bytes", data.lengthof());
  loggers::get_instance().log_to_hexa("Packet dump", data);

  // Remove CV2X identifier
  data = OCTETSTRING(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
  loggers::get_instance().log_msg("qualcomm_layer::receive_data: payload for upper layer:", data);

  if (_observers.size() != 0) {
    incoming_packet_notify(data, params);
  } else {
    // Pass the packet to the upper layers
    receive_to_all_layers(data, params);
  }
}

qualcomm_layer_factory qualcomm_layer_factory::_f;
