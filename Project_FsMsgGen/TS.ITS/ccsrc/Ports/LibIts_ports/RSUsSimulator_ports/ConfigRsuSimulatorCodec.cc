#include <string>
//#include <typeinfo>

#include "ConfigRsuSimulatorCodec.hh"
#include "loggers.hh"

#include "ItsRSUsSimulator_TestSystem.hh"

int ConfigRsuSimulatorCodec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorCodec::encode: ", (const Base_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());

  loggers::get_instance().log_msg("<<< ConfigRsuSimulatorCodec::encode: data=", data);
  return 0;
}

int ConfigRsuSimulatorCodec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorCodec::encode_: processing %s/%s", type.get_descriptor()->name, field_descriptor.name);
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorCodec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("ConfigRsuSimulatorCodec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("ConfigRsuSimulatorCodec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("ConfigRsuSimulatorCodec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else {
    loggers::get_instance().log("ConfigRsuSimulatorCodec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
  }

  loggers::get_instance().log_to_hexa("<<<ConfigRsuSimulatorCodec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int ConfigRsuSimulatorCodec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> ConfigRsuSimulatorCodec::decode: decoding_buffer=", decoding_buffer);
  //  decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< ConfigRsuSimulatorCodec::decode: ", (const Base_Type &)msg);
  return 0;
}
