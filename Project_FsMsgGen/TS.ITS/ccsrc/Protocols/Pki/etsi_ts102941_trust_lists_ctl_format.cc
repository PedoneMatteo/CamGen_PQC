#include "etsi_ts102941_trust_lists_ctl_format.hh"

#include "loggers.hh"

int etsi_ts102941_trust_lists_ctl_format::encode(const EtsiTs102941TrustLists::CtlFormat &p_ctl_format, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_trust_lists_ctl_format::encode: %s", p_ctl_format.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_ctl_format.encode(*p_ctl_format.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_trust_lists_ctl_format::encode: ", p_data);

  return 0;
}

int etsi_ts102941_trust_lists_ctl_format::decode(const OCTETSTRING &p_data, EtsiTs102941TrustLists::CtlFormat &p_ctl_format,
                                                           params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_trust_lists_ctl_format::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_ctl_format.decode(*p_ctl_format.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_trust_lists_ctl_format::decode: ", (const Base_Type &)p_ctl_format);
  return 0;
}
