#include <unistd.h>

#include <Port.hh>

#include "loggers.hh"

#include "uppertester_debug_layer_factory.hh"

uppertester_debug_layer::uppertester_debug_layer(const std::string &p_type, const std::string &param) : layer(p_type), PORT(p_type.c_str()), _params() {
  loggers::get_instance().log(">>> uppertester_debug_layer::uppertester_debug_layer: %s, %s", to_string().c_str(), param.c_str());

  // Setup parameters
  params::convert(_params, param);
}

uppertester_debug_layer::~uppertester_debug_layer() { loggers::get_instance().log(">>> uppertester_debug_layer::~uppertester_debug_layer"); }

void uppertester_debug_layer::send_data(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_debug_layer::send_data: ", p_data);

  receive_data(p_data, p_params);
}

void uppertester_debug_layer::receive_data(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_debug_layer::receive_data: ", p_data);

  OCTETSTRING data;
  if (p_data[0].get_octet() == 0x00) {          // UtInitialize
    data = int2oct(257, 2);                     // '0101'O
  } else if (p_data[0].get_octet() == 0x02) {   // UtChangePosition
    data = int2oct(769, 2);                     // '0301'O
  } else if (p_data[0].get_octet() == 0x04) {   // UtChangePseudonym
    data = int2oct(1281, 2);                    // '0501'O
  } else if (p_data[0].get_octet() == 0x10) {   // UtDenmTrigger
    data = int2oct(4353, 2) + int2oct(1234, 2); // '110104D2'O
  } else if (p_data[0].get_octet() == 0x12) {   // UtDenmUpdate
    data = int2oct(4609, 2) + int2oct(1234, 2); // '110104D2'O
  } else if (p_data[0].get_octet() == 0x14) {   // UtDenmTermination
    // '15xxxx'O
  } else if ((p_data[0].get_octet() >= 0x30) && (p_data[0].get_octet() <= 0x3f)) {
    // UtCamTrigger
    data = int2oct(8449, 2); // '2101'O
  } else {
    data = int2oct(256, 2); // '0100'O
  }

  receive_to_all_layers(data, p_params);
}

uppertester_debug_layer_factory uppertester_debug_layer_factory::_f;
