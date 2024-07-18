#include "ieee_1609dot2_base_types_public_encryption_key.hh"

#include "loggers.hh"

int ieee_1609dot2_base_types_public_encryption_key::encode(const Ieee1609Dot2BaseTypes::PublicEncryptionKey &p_public_encryption_key, OCTETSTRING &p_data) {
  loggers::get_instance().log(">>> ieee_1609dot2_base_types_public_encryption_key::encode: %s", p_public_encryption_key.get_descriptor()->name);

  BITSTRING b;
  TTCN_EncDec::clear_error();
  TTCN_Buffer buffer;
  p_public_encryption_key.encode(*p_public_encryption_key.get_descriptor(), buffer, TTCN_EncDec::CT_OER);
  p_data = OCTETSTRING(buffer.get_len(), buffer.get_data());
  loggers::get_instance().log_msg("ieee_1609dot2_base_types_public_encryption_key::encode: ", p_data);

  return 0;
}

int ieee_1609dot2_base_types_public_encryption_key::decode(const OCTETSTRING &p_data, Ieee1609Dot2BaseTypes::PublicEncryptionKey &p_public_encryption_key,
                                                           params *p_params) {
  loggers::get_instance().log_msg(">>> ieee_1609dot2_base_types_public_encryption_key::decode: ", p_data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(p_data);
  //  _params = params;
  p_public_encryption_key.decode(*p_public_encryption_key.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_OER);

  loggers::get_instance().log_msg("<<< ieee_1609dot2_base_types_public_encryption_key::decode: ", (const Base_Type &)p_public_encryption_key);
  return 0;
}
