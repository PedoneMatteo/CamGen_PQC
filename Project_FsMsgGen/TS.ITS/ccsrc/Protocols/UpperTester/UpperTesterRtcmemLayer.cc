#include "UpperTesterRtcmemFactory.hh"

#include "LibItsRtcmem_TestSystem.hh"
#include "LibItsRtcmem_TypesAndValues.hh"

#include "loggers.hh"

UpperTesterRtcmemLayer::UpperTesterRtcmemLayer(const std::string &p_type, const std::string &param)
  : t_layer<LibItsRtcmem__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> UpperTesterRtcmemLayer::UpperTesterRtcmemLayer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
}

void UpperTesterRtcmemLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterRtcmemLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterRtcmemLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemUpdate &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode(send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterRtcmemLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTermination &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::sendMsg", send_par);

  // Encode UpperTester PDU
  OCTETSTRING data;
  _codec.encode((const Record_Type &)send_par, data);
  // Update parameters
  send_data(data, p_params);
}

void UpperTesterRtcmemLayer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void UpperTesterRtcmemLayer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemLayer::receive_data: ", data);

  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

UpperTesterRtcmemFactory UpperTesterRtcmemFactory::_f;
