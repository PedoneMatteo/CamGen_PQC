#include "uppertester_cam_layer_factory.hh"

#include "LibItsCam_TestSystem.hh"
#include "LibItsCam_TypesAndValues.hh"

#include "loggers.hh"

uppertester_cam_layer::uppertester_cam_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsCam__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_cam_layer::uppertester_cam_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  params::const_iterator it = _params.find(params::loopback);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::loopback, "0"));
  }
}

void uppertester_cam_layer::sendMsg(const LibItsCam__TypesAndValues::UtCamInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_cam_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsCam__TypesAndValues::UtCamResults ut_cam_results;
    ut_cam_results.utCamInitializeResult() = BOOLEAN(true);
    to_all_upper_ports(ut_cam_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_cam_layer::sendMsg(const LibItsCam__TypesAndValues::UtCamChangePosition &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_cam_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsCam__TypesAndValues::UtCamResults ut_cam_results;
    ut_cam_results.utCamChangePositionResult() = BOOLEAN(true);
    to_all_upper_ports(ut_cam_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_cam_layer::sendMsg(const LibItsCam__TypesAndValues::UtCamTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_cam_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsCam__TypesAndValues::UtCamResults ut_cam_results;
    ut_cam_results.utCamTriggerResult() = BOOLEAN(true);
    to_all_upper_ports(ut_cam_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_cam_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_cam_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_cam_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_cam_layer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_cam_layer_factory uppertester_cam_layer_factory::_f;
