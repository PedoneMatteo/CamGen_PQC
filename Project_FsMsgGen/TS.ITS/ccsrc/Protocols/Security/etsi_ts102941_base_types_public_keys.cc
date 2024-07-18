#include "etsi_ts102941_base_types_public_keys.hh"

#include "loggers.hh"

int etsi_ts102941_base_types_public_keys::encode(const EtsiTs102941BaseTypes::PublicKeys &p_public_keys, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_base_types_public_keys::encode: %s", p_public_keys.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_public_keys.encode(*p_public_keys.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_base_types_public_keys::encode: ", p_data);

  return 0;
}

int etsi_ts102941_base_types_public_keys::decode(const OCTETSTRING &p_data, EtsiTs102941BaseTypes::PublicKeys &p_public_keys, params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_base_types_public_keys::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_public_keys.decode(*p_public_keys.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_base_types_public_keys::decode: ", (const Base_Type &)p_public_keys);
  return 0;
}
