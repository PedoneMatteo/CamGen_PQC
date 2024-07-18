#include "uppertester_pki_layer_factory.hh"

#include "LibItsPki_TestSystem.hh"
#include "LibItsPki_TypesAndValues.hh"

#include "loggers.hh"

uppertester_pki_layer::uppertester_pki_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsPki__TestSystem::UpperTesterPkiPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_pki_layer::uppertester_pki_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
  params::const_iterator it = _params.find(params::loopback);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::loopback, "0"));
  }
}

void uppertester_pki_layer::sendMsg(const LibItsPki__TypesAndValues::UtPkiInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_pki_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsPki__TypesAndValues::UtPkiResults ut_pki_results;
    ut_pki_results.utPkiInitializeResult() = BOOLEAN(true);
    to_all_upper_ports(ut_pki_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_pki_layer::sendMsg(const LibItsPki__TypesAndValues::UtPkiTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_pki_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsPki__TypesAndValues::UtPkiResults ut_pki_results;
    ut_pki_results.utPkiTriggerResult() = BOOLEAN(true);
    to_all_upper_ports(ut_pki_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_pki_layer::sendMsg(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_pki_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsPki__TypesAndValues::UtPkiResults ut_pki_results;
    ut_pki_results.utPkiTriggerResult() = BOOLEAN(true);
    to_all_upper_ports(ut_pki_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_pki_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_pki_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_pki_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_pki_layer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_pki_layer_factory uppertester_pki_layer_factory::_f;
