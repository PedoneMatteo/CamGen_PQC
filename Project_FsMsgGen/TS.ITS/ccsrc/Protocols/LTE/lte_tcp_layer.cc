#include "lte_tcp_layer.hh"

#include "loggers.hh"

lte_tcp_layer::lte_tcp_layer(const std::string &p_type, const std::string &p_param): tcp_layer(p_type, p_param), data_event_observer() { 
  loggers::get_instance().log(">>> lte_tcp_layer::lte_tcp_layer (1)");

}

lte_tcp_layer::lte_tcp_layer(const std::string &p_type, const params &p_param): tcp_layer(p_type, p_param), data_event_observer() { 
  loggers::get_instance().log(">>> lte_tcp_layer::lte_tcp_layer (2)");
  
}

void lte_tcp_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> lte_tcp_layer::receive_data: ", data);

  if (_observers.size() != 0) {
    incoming_packet_notify(data, params);
  } else {
    // Pass the packet to the upper layers
    receive_to_all_layers(data, params);
  }
}

