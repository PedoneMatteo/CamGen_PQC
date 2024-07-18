#include "uppertester_denm_layer_factory.hh"

#include "LibItsDenm_TestSystem.hh"
#include "LibItsDenm_TypesAndValues.hh"

#include "loggers.hh"

uppertester_denm_layer::uppertester_denm_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsDenm__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_denm_layer::uppertester_denm_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);

  params::const_iterator it = _params.find(params::loopback);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::loopback, "0"));
  }
  it = _params.find(params::station_id);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::station_id, "12345"));
  }
} // End of ctor

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmResults ut_denm_results;
    ut_denm_results.utDenmInitializeResult() = BOOLEAN(true);
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmChangePosition &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmResults ut_denm_results;
    ut_denm_results.utDenmChangePositionResult() = BOOLEAN(true);
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmChangePseudonym &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmResults ut_denm_results;
    ut_denm_results.utDenmChangePseudonymResult() = BOOLEAN(true);
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmTriggerResult trigger_result(true, ITS__Container::ActionID(std::stoi(_params[params::station_id]), 1));
    LibItsDenm__TypesAndValues::UtDenmResults       ut_denm_results;
    ut_denm_results.utDenmTriggerResult() = trigger_result;
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmUpdate &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmUpdateResult update_result(true, ITS__Container::ActionID(std::stoi(_params[params::station_id]), 1));
    LibItsDenm__TypesAndValues::UtDenmResults      ut_denm_results;
    ut_denm_results.utDenmUpdateResult() = update_result;
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmTermination &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsDenm__TypesAndValues::UtDenmResults ut_denm_results;
    ut_denm_results.utDenmTerminationResult() = BOOLEAN(true);
    to_all_upper_ports(ut_denm_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_denm_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_denm_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_denm_layer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_denm_layer_factory uppertester_denm_layer_factory::_f;
