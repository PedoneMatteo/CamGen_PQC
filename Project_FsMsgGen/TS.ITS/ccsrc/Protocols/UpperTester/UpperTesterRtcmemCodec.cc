#include <string>
//#include <typeinfo>

#include "UpperTesterRtcmemCodec.hh"
#include "loggers.hh"

#include "RtcmemCodec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char UpperTesterRtcmemCodec::c_utRtcmemInitialize        = 0x00;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemInitializeResult  = 0x01;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemTrigger           = 0xC0;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemTriggerResult     = 0xC1;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemUpdate            = 0xC2;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemUpdateResult      = 0xC3;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemTermination       = 0xC4;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemTerminationResult = 0xC5;
unsigned char UpperTesterRtcmemCodec::c_utRtcmemEventIndication   = 0xC6;

int UpperTesterRtcmemCodec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemCodec::encode: ", (const Record_Type &)msg);
  loggers::get_instance().log(">>> UpperTesterRtcmemCodec::encode: %s", msg.get_descriptor()->name);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsRtcmem_TypesAndValues.UtRtcmemInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsRtcmem_TypesAndValues.UtRtcmemTrigger") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemTrigger));
    const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &trigger = dynamic_cast<const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &>(msg);
    unsigned char                                        flag    = 0x00;
    if (trigger.timeStamp().is_present()) {
      flag |= 0x80;
    }
    if (trigger.anchorPoint().is_present()) {
      flag |= 0x40;
    }
    if (trigger.rtcmHeader().is_present()) {
      flag |= 0x20;
    }
    encoding_buffer.put_c(flag);
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsRtcmem_TypesAndValues.UtRtcmemUpdate") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemUpdate));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsRtcmem_TypesAndValues.UtRtcmemTermination") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemTermination));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().log("<<< UpperTesterRtcmemCodec::encode: Return -1");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< UpperTesterRtcmemCodec::encode: data=", data);
  return 0;
}

int UpperTesterRtcmemCodec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> UpperTesterRtcmemCodec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> UpperTesterRtcmemCodec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("UpperTesterRtcmemCodec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("UpperTesterRtcmemCodec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("UpperTesterRtcmemCodec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("UpperTesterRtcmemCodec::encode_: field to process %s", s.c_str());
    if (s.compare("@LibItsRtcmem_TypesAndValues.UtRtcmemTrigger.msgs") == 0) {
      const PreGenRecordOf::PREGEN__RECORD__OF__OCTETSTRING &rov = dynamic_cast<const PreGenRecordOf::PREGEN__RECORD__OF__OCTETSTRING &>(type);
      encoding_buffer.put_c(static_cast<unsigned char>(rov.lengthof()));
      for (int i = 0; i < rov.lengthof(); i++) {
        const OCTETSTRING &os = rov[i];
        encoding_buffer.put_c(static_cast<unsigned char>(os.lengthof()));
        encoding_buffer.put_os(os);
      } // End of 'for' statement
    } else {
      loggers::get_instance().log("UpperTesterRtcmemCodec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_OER); // HashId8
    }
  }

  loggers::get_instance().log_to_hexa("<<<UpperTesterRtcmemCodec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int UpperTesterRtcmemCodec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> UpperTesterRtcmemCodec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< UpperTesterRtcmemCodec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> UpperTesterRtcmemCodec::decode(const OCTETSTRING &data, params *params) {
  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr != UpperTesterRtcmemCodec::c_utRtcmemEventIndication) {
    LibItsRtcmem__TypesAndValues::UtRtcmemResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  } else {
    LibItsRtcmem__TypesAndValues::UtRtcmemEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  }

  return result;
}

int UpperTesterRtcmemCodec::decode(const OCTETSTRING &data, LibItsRtcmem__TypesAndValues::UtRtcmemResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterRtcmemCodec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == UpperTesterRtcmemCodec::c_utRtcmemInitializeResult) {
    msg.utRtcmemInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterRtcmemCodec::c_utRtcmemTriggerResult) {
    msg.utRtcmemTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterRtcmemCodec::c_utRtcmemUpdateResult) {
    msg.utRtcmemUpdateResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == UpperTesterRtcmemCodec::c_utRtcmemTerminationResult) {
    msg.utRtcmemTerminationResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else {
    TTCN_warning("UpperTesterRtcmemCodec::decode: Unsupported result");
  }

  loggers::get_instance().log_msg("<<< UpperTesterRtcmemCodec::decode", msg);
  return 0;
}

int UpperTesterRtcmemCodec::decode(const OCTETSTRING &data, LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> UpperTesterRtcmemCodec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("UpperTesterRtcmemCodec::decode: RTCMEM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("UpperTesterRtcmemCodec::decode: RTCMEM message=", os);
  RtcmemCodec codec;
  codec.decode(os, msg.rtcmemMsg(), params);

  loggers::get_instance().log_msg("<<< UpperTesterRtcmemCodec::decode", msg);
  return 0;
}
