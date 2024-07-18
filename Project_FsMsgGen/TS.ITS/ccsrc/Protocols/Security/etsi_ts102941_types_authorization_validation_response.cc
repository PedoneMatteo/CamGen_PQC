#include "etsi_ts102941_types_authorization_validation_response.hh"

#include "loggers.hh"

int etsi_ts102941_types_authorization_validation_response::encode(
  const EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse &p_authorization_validation_response, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_types_authorization_validation_response::encode: %s",
                              p_authorization_validation_response.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_authorization_validation_response.encode(*p_authorization_validation_response.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_types_authorization_validation_response::encode: ", p_data);

  return 0;
}

int etsi_ts102941_types_authorization_validation_response::decode(
  const OCTETSTRING &p_data, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse &p_authorization_validation_response, params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_types_authorization_validation_response::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_authorization_validation_response.decode(*p_authorization_validation_response.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_types_authorization_validation_response::decode: ",
                                  (const Base_Type &)p_authorization_validation_response);
  return 0;
}
