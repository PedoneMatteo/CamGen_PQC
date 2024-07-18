#include "etsi_ts102941_types_enrolment_inner_response.hh"

#include "loggers.hh"

int etsi_ts102941_types_enrolment_inner_response::encode(const EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_types_enrolment_inner_response::encode: %s", p_inner_response.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_inner_response.encode(*p_inner_response.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_types_enrolment_inner_response::encode: ", p_data);

  return 0;
}

int etsi_ts102941_types_enrolment_inner_response::decode(const OCTETSTRING &p_data, EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response,
                                                         params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_types_enrolment_inner_response::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_inner_response.decode(*p_inner_response.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_types_enrolment_inner_response::decode: ", (const Base_Type &)p_inner_response);
  return 0;
}
