#include "uppertester_geonetworking_factory.hh"

#include "LibItsGeoNetworking_TestSystem.hh"
#include "LibItsGeoNetworking_TypesAndValues.hh"

#include "loggers.hh"

uppertester_geonetworking_layer::uppertester_geonetworking_layer(const std::string &p_type, const std::string &p_param)
  : t_layer<LibItsGeoNetworking__TestSystem::UpperTesterPort>(p_type), _params(), _codec() {
  loggers::get_instance().log(">>> uppertester_geonetworking_layer::uppertester_geonetworking_layer: %s, %s", to_string().c_str(), p_param.c_str());
  // Setup parameters
  params::convert(_params, p_param);
  params::const_iterator it = _params.find(params::loopback);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::loopback, "0"));
  }
}

void uppertester_geonetworking_layer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsGeoNetworking__TypesAndValues::UtGnResults ut_gn_results;
    ut_gn_results.utGnInitializeResult() = BOOLEAN(true);
    to_all_upper_ports(ut_gn_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(static_cast<const Record_Type &>(send_par), data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_geonetworking_layer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnChangePosition &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsGeoNetworking__TypesAndValues::UtGnResults ut_gn_results;
    ut_gn_results.utGnChangePositionResult() = BOOLEAN(true);
    to_all_upper_ports(ut_gn_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(static_cast<const Record_Type &>(send_par), data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_geonetworking_layer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsGeoNetworking__TypesAndValues::UtGnResults ut_gn_results;
    ut_gn_results.utGnTriggerResult() = BOOLEAN(true);
    to_all_upper_ports(ut_gn_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode(send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_geonetworking_layer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtAutoInteropTrigger &send_par, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::sendMsg", send_par);

  if (_params[params::loopback].compare("1") == 0) {
    LibItsGeoNetworking__TypesAndValues::UtGnResults ut_gn_results;
    ut_gn_results.utGnTriggerResult() = BOOLEAN(true);
    to_all_upper_ports(ut_gn_results, p_params);
  } else {
    // Encode UpperTester PDU
    OCTETSTRING data;
    _codec.encode((const Record_Type &)send_par, data);
    // Update parameters
    send_data(data, p_params);
  }
}

void uppertester_geonetworking_layer::send_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::send_data: ", data);
  // params.log();
  send_to_all_layers(data, p_params);
}

void uppertester_geonetworking_layer::receive_data(OCTETSTRING &data, params &p_params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_layer::receive_data: ", data);
  std::unique_ptr<Base_Type> r = _codec.decode(data);
  if (r.get() != nullptr) {
    // Pass it to the ports if any
    to_all_upper_ports(*r, p_params);
  }
}

uppertester_geonetworking_factory uppertester_geonetworking_factory::_f;
