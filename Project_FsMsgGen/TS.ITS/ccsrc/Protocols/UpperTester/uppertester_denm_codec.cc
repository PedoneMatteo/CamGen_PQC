#include <string>
//#include <typeinfo>

#include "loggers.hh"
#include "uppertester_denm_codec.hh"

#include "LibItsDenm_TestSystem.hh"
#include "LibItsDenm_TypesAndValues.hh"

#include "denm_codec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_denm_codec::c_utDenmInitialize            = 0x00;
unsigned char uppertester_denm_codec::c_utDenmInitializeResult      = 0x01;
unsigned char uppertester_denm_codec::c_utDenmChangePosition        = 0x02;
unsigned char uppertester_denm_codec::c_utDenmChangePositionResult  = 0x03;
unsigned char uppertester_denm_codec::c_utDenmChangePseudonym       = 0x04;
unsigned char uppertester_denm_codec::c_utDenmChangePseudonymResult = 0x05;
unsigned char uppertester_denm_codec::c_utDenmTrigger               = 0x10;
unsigned char uppertester_denm_codec::c_utDenmTriggerResult         = 0x11;
unsigned char uppertester_denm_codec::c_utDenmUpdate                = 0x12;
unsigned char uppertester_denm_codec::c_utDenmUpdateResult          = 0x13;
unsigned char uppertester_denm_codec::c_utDenmTermination           = 0x14;
unsigned char uppertester_denm_codec::c_utDenmTerminationResult     = 0x15;
unsigned char uppertester_denm_codec::c_utDenmEventIndication       = 0x17;

int uppertester_denm_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_denm_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmInitialize") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmChangePosition") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmChangePosition));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmChangePseudonym") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmChangePseudonym));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmTrigger") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmTrigger));
    // See ETSI TR 103 099 V1.4.1 Clause C.4.1 GenerateDenmEvent
    unsigned char                                    flags = 0x00;
    const LibItsDenm__TypesAndValues::UtDenmTrigger &r     = static_cast<const LibItsDenm__TypesAndValues::UtDenmTrigger &>(msg);
    if (r.validityDuration().is_present()) { // V
      flags |= 0x80;
    }
    if (r.repetitionDuration().is_present()) { // R
      flags |= 0x40;
    }
    if (r.relevanceTrafficDirection().is_present()) { // T
      flags |= 0x10;
    }
    if (r.transmissionInterval().is_present()) { // K
      flags |= 0x04;
    }
    if (r.repetitionInterval().is_present()) { // I
      flags |= 0x02;
    }
    encoding_buffer.put_c(flags);
    loggers::get_instance().log_to_hexa("uppertester_denm_codec::encode: flags=", encoding_buffer);
    // Continue with encoding
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmUpdate") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmUpdate));
    // See ETSI TR 103 099 V1.3.21 (20152016-07) Clause C.4.1GenerateDenmEvent
    unsigned char                                   flags = 0x00;
    const LibItsDenm__TypesAndValues::UtDenmUpdate &r     = static_cast<const LibItsDenm__TypesAndValues::UtDenmUpdate &>(msg);
    if (r.validityDuration().is_present()) { // V
      flags |= 0x80;
    }
    if (r.situation().is_present()) { // S
      flags |= 0x40;
    }
    if (r.relevanceDistance().is_present()) { // D
      flags |= 0x20;
    }
    if (r.relevanceTrafficDirection().is_present()) { // T
      flags |= 0x10;
    }
    // TODO Check TTCN-3 code, it's missing
    /*if (r.trafficClass().is_present()) { // C
      flags |= 0x08;
      }*/
    if (r.transmissionInterval().is_present()) { // K
      flags |= 0x04;
    }
    if (r.repetitionInterval().is_present()) { // I
      flags |= 0x02;
    }
    encoding_buffer.put_c(flags);
    loggers::get_instance().log_to_hexa("uppertester_denm_codec::encode: flags=", encoding_buffer);
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsDenm_TypesAndValues.UtDenmTermination") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_denm_codec::c_utDenmTermination));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().warning("<<< uppertester_denm_codec::encode: Failed to encode UT message");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_denm_codec::encode: data=", data);
  return 0;
}

int uppertester_denm_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_denm_codec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> uppertester_denm_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_denm_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_denm_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_denm_codec::encode_: -1 result code returned");
          return -1;
        }
      } else {
        std::string s(r.fld_descr(i)->name);
        if ((s.rfind(".validityDuration") != string::npos) || (s.rfind(".repetitionDuration") != string::npos) ||
            (s.rfind(".situation") != string::npos)) { // Pad with three bytes
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
        } else if ((s.rfind(".transmissionInterval") != string::npos) || (s.rfind(".repetitionInterval") != string::npos)) { // Pad with two bytes
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
        } else if ((s.rfind(".informationQuality") != string::npos) || (s.rfind(".relevanceTrafficDirection") != string::npos) ||
                   (s.rfind(".relevanceDistance") != string::npos) // Mandatory in DenmTrigger, optional in DenmUpdate
        ) {                                                        // Pad with one byte
          encoding_buffer.put_c((unsigned char)0x00);
        } else if (std::string(r.fld_name(i)).compare("alacarte") == 0) {
          // Add empty field length
          loggers::get_instance().log("uppertester_denm_codec::encode_: alacarte is missing, add 0x00");
          encoding_buffer.put_c(0x00);
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("uppertester_denm_codec::encode_: field to process %s", s.c_str());
    if ((s.rfind(".shape") != string::npos) || (s.rfind(".relevanceDistance") != string::npos) || (s.rfind(".relevanceTrafficDirection") != string::npos)) {
      encoding_buffer.put_c((unsigned char)static_cast<const Enum_Type &>(type).as_int());
    } else if (s.rfind(".payload") != string::npos) {
      const OCTETSTRING & os  = static_cast<const OCTETSTRING &>(type);
      const unsigned char s[] = {(unsigned char)((os.lengthof() & 0x0000FF00) >> 8), (unsigned char)os.lengthof()};
      encoding_buffer.put_s(2, s);
      if (os.lengthof() != 0) {
        encoding_buffer.put_string(os);
      }
    } else if (s.rfind(".detectionTime") != string::npos) {
      unsigned long long llu = static_cast<const INTEGER &>(type).get_long_long_val();
      loggers::get_instance().log("uppertester_denm_codec::encode_ : detectionTime=%llu", llu);
      std::vector<unsigned char> v;
      for (int i = 0; i < 6; i++) {
        v.insert(v.begin(), static_cast<unsigned char>(llu));
        llu >>= 8;
      } // End of 'for' statement
      OCTETSTRING os(v.size(), v.data());
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: timeDetection=", os);
      encoding_buffer.put_string(os);
    } else if ((s.rfind(".validityDuration") != string::npos) || (s.rfind(".repetitionDuration") != string::npos)) {
      const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
      const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: i=", i);
      encoding_buffer.put_string(int2oct(i, 3));
    } else if ((s.rfind(".causeCode") != string::npos) || (s.rfind(".subCauseCode") != string::npos) || (s.rfind(".informationQuality") != string::npos) ||
               (s.rfind(".relevanceDistance") != string::npos)) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      loggers::get_instance().log("uppertester_denm_codec::encode_: o=%02x", *static_cast<const unsigned char *>(int2oct(i, 1)));
      encoding_buffer.put_c(*static_cast<const unsigned char *>(int2oct(i, 1)));
    } else if ((s.rfind(".linkedCause") != string::npos) || (s.rfind(".eventHistory") != string::npos)) {
      // Skip them
    } else if ((s.rfind(".transmissionInterval") != string::npos) || (s.rfind(".repetitionInterval") != string::npos)) {
      const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
      const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
      encoding_buffer.put_string(int2oct(i, 2));
    } else if (s.rfind(".originatingStationID") != string::npos) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: i=", i);
      encoding_buffer.put_string(int2oct(i, 4));
    } else if (s.rfind(".sequenceNumber") != string::npos) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: i=", i);
      encoding_buffer.put_string(int2oct(i, 2));
    } else if (s.rfind(".situation") != string::npos) {
      const OPTIONAL<DENM__PDU__Descriptions::SituationContainer> &o  = dynamic_cast<const OPTIONAL<DENM__PDU__Descriptions::SituationContainer> &>(type);
      const DENM__PDU__Descriptions::SituationContainer &          bt = static_cast<const DENM__PDU__Descriptions::SituationContainer &>(*o.get_opt_value());
      encode_(bt, field_descriptor, encoding_buffer);
    } else if (s.rfind(".AlacarteContainer") != string::npos) {
      const OPTIONAL<OCTETSTRING> &o  = dynamic_cast<const OPTIONAL<OCTETSTRING> &>(type);
      const OCTETSTRING &          os = static_cast<const OCTETSTRING &>(*o.get_opt_value());
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: os=", int2oct(os.lengthof(), 1));
      encoding_buffer.put_string(int2oct(os.lengthof(), 1));
      loggers::get_instance().log_msg("uppertester_denm_codec::encode_: os=", os);
      encoding_buffer.put_string(os);
    } else {
      loggers::get_instance().log("uppertester_denm_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  loggers::get_instance().log_to_hexa("<<< uppertester_denm_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_denm_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> uppertester_denm_codec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_denm_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_denm_codec::decode(const OCTETSTRING &data, params *params) {

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr != uppertester_denm_codec::c_utDenmEventIndication) {
    LibItsDenm__TypesAndValues::UtDenmResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  } else {
    LibItsDenm__TypesAndValues::UtDenmEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  }

  return result;
}

int uppertester_denm_codec::decode(const OCTETSTRING &data, LibItsDenm__TypesAndValues::UtDenmResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_denm_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_denm_codec::c_utDenmInitializeResult) {
    msg.utDenmInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_denm_codec::c_utDenmChangePositionResult) {
    msg.utDenmChangePositionResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_denm_codec::c_utDenmChangePseudonymResult) {
    msg.utDenmChangePseudonymResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_denm_codec::c_utDenmTerminationResult) {
    msg.utDenmTerminationResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_denm_codec::c_utDenmTriggerResult) {
    LibItsDenm__TypesAndValues::UtDenmTriggerResult r;
    ptr += 1;
    r.result() = BOOLEAN(*ptr == 0x01);
    ptr += 1;
    OCTETSTRING os(4, ptr);
    ptr += 4;
    r.actionId().originatingStationID() = oct2int(os);
    os                                  = OCTETSTRING(2, ptr);
    r.actionId().sequenceNumber()       = oct2int(os);
    msg.utDenmTriggerResult()           = r;
  } else if (*ptr == uppertester_denm_codec::c_utDenmUpdateResult) {
    LibItsDenm__TypesAndValues::UtDenmUpdateResult r;
    ptr += 1;
    r.result() = BOOLEAN(*ptr == 0x01);
    ptr += 1;
    OCTETSTRING os(4, ptr);
    ptr += 4;
    r.actionId().originatingStationID() = oct2int(os);
    os                                  = OCTETSTRING(2, ptr);
    r.actionId().sequenceNumber()       = oct2int(os);
    msg.utDenmUpdateResult()            = r;
  } else if (*ptr == 0x24) { // TODO Use a constant for "Peer does not support is "Peer does not support the command"
    // Peer does not support the command
    msg.utDenmInitializeResult() = 0x00;
  } else {
    loggers::get_instance().warning("uppertester_denm_codec::decode: Unsupported result");
  }

  loggers::get_instance().log_msg("<<< uppertester_denm_codec::decode", msg);
  return 0;
}

int uppertester_denm_codec::decode(const OCTETSTRING &data, LibItsDenm__TypesAndValues::UtDenmEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_denm_codec::decode (1): decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("uppertester_denm_codec::decode (1): DENM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("uppertester_denm_codec::decode (1): DENM message=", os);
  denm_codec codec;
  codec.decode(os, msg.denMsg(), params);

  loggers::get_instance().log_msg("<<< uppertester_denm_codec::decode (1)", msg);
  return 0;
}
