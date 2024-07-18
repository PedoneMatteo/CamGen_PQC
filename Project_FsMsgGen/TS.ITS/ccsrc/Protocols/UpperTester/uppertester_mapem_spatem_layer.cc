#include "uppertester_mapem_spatem_factory.hh"

#include "LibItsMapemSpatem_TestSystem.hh"
#include "LibItsMapemSpatem_TypesAndValues.hh"

#include "loggers.hh"

uppertester_mapem_spatem_layer::uppertester_mapem_spatem_layer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsMapemSpatem__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_mapem_spatem_layer::uppertester_mapem_spatem_layer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
}

void uppertester_mapem_spatem_layer::sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_mapem_spatem_layer::sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_layer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void uppertester_mapem_spatem_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_mapem_spatem_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_layer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_mapem_spatem_factory uppertester_mapem_spatem_factory::_f;
