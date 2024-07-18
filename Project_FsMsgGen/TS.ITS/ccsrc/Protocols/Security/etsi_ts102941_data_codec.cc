#include "etsi_ts102941_data_codec.hh"

#include "loggers.hh"

int etsi_ts102941_data_codec::encode(const EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_102941_data, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_data_codec::encode: %s", p_etsi_ts_102941_data.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_etsi_ts_102941_data.encode(*p_etsi_ts_102941_data.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_data_codec::encode: ", p_data);

  return 0;
}

int etsi_ts102941_data_codec::decode(const OCTETSTRING &p_data, EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_102941_data, params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_data_codec::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_etsi_ts_102941_data.decode(*p_etsi_ts_102941_data.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_data_codec::decode: ", (const Base_Type &)p_etsi_ts_102941_data);
  return 0;
}
