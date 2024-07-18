#include "uppertester_ivim_factory.hh"

#include "LibItsIvim_TestSystem.hh"
#include "LibItsIvim_TypesAndValues.hh"

#include "loggers.hh"

uppertester_ivim_layer::uppertester_ivim_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsIvim__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_ivim_layer::uppertester_ivim_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
}

void uppertester_ivim_layer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_ivim_layer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_ivim_layer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimUpdate &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_ivim_layer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimTermination &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_ivim_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_ivim_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_layer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_ivim_factory uppertester_ivim_factory::_f;
