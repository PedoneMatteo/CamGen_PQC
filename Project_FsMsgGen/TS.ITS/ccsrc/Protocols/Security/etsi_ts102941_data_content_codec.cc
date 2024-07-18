#include "etsi_ts102941_data_content_codec.hh"

#include "loggers.hh"

int etsi_ts102941_data_content_codec::encode(const EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_10291_data_content, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> etsi_ts102941_data_content_codec::encode: %s", p_etsi_ts_10291_data_content.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_etsi_ts_10291_data_content.encode(*p_etsi_ts_10291_data_content.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("etsi_ts102941_data_content_codec::encode: ", p_data);

  return 0;
}

int etsi_ts102941_data_content_codec::decode(const OCTETSTRING &p_data, EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_10291_data_content,
                                             params *p_params) {
  loggers::get_instance().log_msg(">>> etsi_ts102941_data_content_codec::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_etsi_ts_10291_data_content.decode(*p_etsi_ts_10291_data_content.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< etsi_ts102941_data_content_codec::decode: ", (const Base_Type &)p_etsi_ts_10291_data_content);
  return 0;
}
