#include "UpperTesterSremSsemFactory.hh"

#include "LibItsSremSsem_TestSystem.hh"
#include "LibItsSremSsem_TypesAndValues.hh"

#include "loggers.hh"

UpperTesterSremSsemLayer::UpperTesterSremSsemLayer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsSremSsem__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> UpperTesterSremSsemLayer::UpperTesterSremSsemLayer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
}

void UpperTesterSremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterSremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterSremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremUpdate &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterSremSsemLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremCancelation &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterSremSsemLayer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void UpperTesterSremSsemLayer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemLayer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

UpperTesterSremSsemFactory UpperTesterSremSsemFactory::_f;
