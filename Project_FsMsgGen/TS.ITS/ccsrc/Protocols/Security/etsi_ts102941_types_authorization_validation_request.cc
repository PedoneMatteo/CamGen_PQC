#include "etsi_ts102941_types_authorization_validation_request.hh"

#include "loggers.hh"

int etsi_ts102941_types_authorization_validation_request::encode(
  const EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest &p_authorization_validation_request, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_types_authorization_validation_request::encode: %s",
                              p_authorization_validation_request.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_authorization_validation_request.encode(*p_authorization_validation_request.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_types_authorization_validation_request::encode: ", p_data);

  return 0;
}

int etsi_ts102941_types_authorization_validation_request::decode(
  const OCTETSTRING &p_data, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest &p_authorization_validation_request, params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_types_authorization_validation_request::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_authorization_validation_request.decode(*p_authorization_validation_request.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_types_authorization_validation_request::decode: ", (const Base_Type &)p_authorization_validation_request);
  return 0;
}
