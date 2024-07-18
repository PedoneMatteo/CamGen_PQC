#include <string>
//#include <typeinfo>

#include "UpperTesterSremSsemCodec.hh"
#include "loggers.hh"

#include "LibItsSremSsem_TestSystem.hh"
#include "LibItsSremSsem_TypesAndValues.hh"

#include "SremCodec.hh"
#include "SsemCodec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char UpperTesterSremSsemCodec::c_utSremInitialize        = 0x00;
unsigned char UpperTesterSremSsemCodec::c_utSremInitializeResult  = 0x01;
unsigned char UpperTesterSremSsemCodec::c_utSremTrigger           = 0xA0;
unsigned char UpperTesterSremSsemCodec::c_utSremTriggerResult     = 0xA1;
unsigned char UpperTesterSremSsemCodec::c_utSremUpdate            = 0xA2;
unsigned char UpperTesterSremSsemCodec::c_utSremUpdateResult      = 0xA3;
unsigned char UpperTesterSremSsemCodec::c_utSremCancelation       = 0xA4;
unsigned char UpperTesterSremSsemCodec::c_utSremCancelationResult = 0xA5;
unsigned char UpperTesterSremSsemCodec::c_utSremEventIndication   = 0xA6;
unsigned char UpperTesterSremSsemCodec::c_utSsemEventIndication   = 0xA7;

int UpperTesterSremSsemCodec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemCodec::encode: ", (const Record_Type &)msg);
  loggers::get_instance().log(">>> UpperTesterSremSsemCodec::encode: %s", msg.get_descriptor()->name);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsSremSsem_TypesAndValues.UtSremInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsSremSsem_TypesAndValues.UtSremTrigger") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremTrigger));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsSremSsem_TypesAndValues.UtSremUpdate") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremUpdate));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsSremSsem_TypesAndValues.UtSremCancelation") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremCancelation));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().log("<<< UpperTesterSremSsemCodec::encode: Return -1");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< UpperTesterSremSsemCodec::encode: data=", data);
  return 0;
}

int UpperTesterSremSsemCodec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> UpperTesterSremSsemCodec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemCodec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("UpperTesterSremSsemCodec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("UpperTesterSremSsemCodec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d",
                                  r.fld_name(i), r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(),
                                  r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("UpperTesterSremSsemCodec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("UpperTesterSremSsemCodec::encode_: field to process %s", s.c_str());
    loggers::get_instance().log("UpperTesterSremSsemCodec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_OER);
  }

  loggers::get_instance().log_to_hexa("<<<UpperTesterSremSsemCodec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int UpperTesterSremSsemCodec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  loggers::get_instance().log_msg(">>> UpperTesterSremSsemCodec::decode (1): data=", data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa("UpperTesterSremSsemCodec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< UpperTesterSremSsemCodec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> UpperTesterSremSsemCodec::decode(const OCTETSTRING &data, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterSremSsemCodec::decode (2): decoding_buffer=", data);

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == UpperTesterSremSsemCodec::c_utSsemEventIndication) {
    LibItsSremSsem__TypesAndValues::UtSsemEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  } else if (*ptr == UpperTesterSremSsemCodec::c_utSremEventIndication) {
    LibItsSremSsem__TypesAndValues::UtSremEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  } else {
    LibItsSremSsem__TypesAndValues::UtSremResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  }

  return result;
}

int UpperTesterSremSsemCodec::decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSremResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterSremSsemCodec::decode (3): decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == UpperTesterSremSsemCodec::c_utSremInitializeResult) {
    msg.utSremInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterSremSsemCodec::c_utSremTriggerResult) {
    msg.utSremTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterSremSsemCodec::c_utSremUpdateResult) {
    msg.utSremUpdateResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterSremSsemCodec::c_utSremCancelationResult) {
    msg.utSremCancelationResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == 0x24) { // TODO Use a constant for "Peer does not support is "Peer does not support the command"
    // Peer does not support the command
    msg.utSremInitializeResult() = 0x00;
  } else {
    loggers::get_instance().warning("UpperTesterSremSsemCodec::decode (3): Unsupported result");
    return -1;
  }

  loggers::get_instance().log_msg("<<< UpperTesterSremSsemCodec::decode (3): ", msg);
  return 0;
}

int UpperTesterSremSsemCodec::decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSremEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterSremSsemCodec::decode (4): decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("UpperTesterSremSsemCodec::decode: SREM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("UpperTesterSremSsemCodec::decode: SREM message=", os);
  SremCodec codec;
  codec.decode(os, msg.sreMsg(), params);

  loggers::get_instance().log_msg("<<< UpperTesterSremSsemCodec::decode (4): ", msg);
  return 0;
}

int UpperTesterSremSsemCodec::decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSsemEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterSremSsemCodec::decode (5): decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("UpperTesterSremSsemCodec::decode: SSEM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("UpperTesterSremSsemCodec::decode: SSEM message=", os);
  SsemCodec codec;
  codec.decode(os, msg.sseMsg(), params);

  loggers::get_instance().log_msg("<<< UpperTesterSremSsemCodec::decode (5): ", msg);
  return 0;
}
