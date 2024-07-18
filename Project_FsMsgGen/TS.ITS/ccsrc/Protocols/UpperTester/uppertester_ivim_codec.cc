#include <memory>
#include <string>

#include "loggers.hh"
#include "uppertester_ivim_codec.hh"

#include "LibItsIvim_TestSystem.hh"
#include "LibItsIvim_TypesAndValues.hh"

#include "ivim_codec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_ivim_codec::c_utIvimInitialize        = 0x00;
unsigned char uppertester_ivim_codec::c_utIvimInitializeResult  = 0x01;
unsigned char uppertester_ivim_codec::c_utIvimTrigger           = 0x80;
unsigned char uppertester_ivim_codec::c_utIvimTriggerResult     = 0x81;
unsigned char uppertester_ivim_codec::c_utIvimUpdate            = 0x82;
unsigned char uppertester_ivim_codec::c_utIvimUpdateResult      = 0x83;
unsigned char uppertester_ivim_codec::c_utIvimTermination       = 0x84;
unsigned char uppertester_ivim_codec::c_utIvimTerminationResult = 0x85;
unsigned char uppertester_ivim_codec::c_utIvimEventIndication   = 0x86;

int uppertester_ivim_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_ivim_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsIvim_TypesAndValues.UtIvimInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsIvim_TypesAndValues.UtIvimTrigger") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimTrigger));
    // See ETSI TR 103 099 V1.4.1 Clause C.9.1 GenerateIvimEvent
    unsigned short                                   flags = 0x0000;
    const LibItsIvim__TypesAndValues::UtIvimTrigger &r     = static_cast<const LibItsIvim__TypesAndValues::UtIvimTrigger &>(msg);
    if (r.validFrom().is_present()) { // F
      flags |= 0x8000;
    }
    if (r.validTo().is_present()) { // V
      flags |= 0x4000;
    }
    if (r.repetitionInterval().is_present()) { // T
      flags |= 0x2000;
    }
    if (r.zoneIds().is_present()) { // I
      flags |= 0x1000;
    }
    if (r.laneNumber().is_present()) { // L
      flags |= 0x0800;
    }
    if (r.relavanceZoneIds().is_present()) { // Z
      flags |= 0x0400;
    }
    if (r.detectionZoneIds().is_present()) { // D
      flags |= 0x0200;
    }
    if (r.driverAwarenesZoneIds().is_present()) { // A
      flags |= 0x0100;
    }
    if (r.itsRrid().is_present()) { // R
      flags |= 0x0080;
    }
    if (r.directionGic().is_present()) { // G
      flags |= 0x0040;
    }
    if (r.layoutId().is_present()) { // Y
      flags |= 0x0020;
    }
    encoding_buffer.put_c(static_cast<unsigned char>((flags >> 8) & 0xFF));
    encoding_buffer.put_c(static_cast<unsigned char>(flags & 0x00FF));
    loggers::get_instance().log_to_hexa("uppertester_ivim_codec::encode: flags=", encoding_buffer);
    // Continue with encoding
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsIvim_TypesAndValues.UtIvimUpdate") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimUpdate));
    // See ETSI TR 103 099 V1.4.1 Clause C.9.2	UpdateIvimEvent
    unsigned char                                   flags = 0x00;
    const LibItsIvim__TypesAndValues::UtIvimUpdate &r     = static_cast<const LibItsIvim__TypesAndValues::UtIvimUpdate &>(msg);
    if (r.validFrom().is_present()) { // V
      flags |= 0x80;
    }
    if (r.validTo().is_present()) { // R
      flags |= 0x40;
    }
    if (r.timeStamp().is_present()) { // T
      flags |= 0x20;
    }
    if (r.connectedIviStructures().is_present()) { // C
      flags |= 0x10;
    }
    encoding_buffer.put_c(flags);
    loggers::get_instance().log_to_hexa("uppertester_ivim_codec::encode: flags=", encoding_buffer);
    // Continue with encoding
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsIvim_TypesAndValues.UtIvimTermination") == 0) {
    // See ETSI TR 103 099 V1.4.1 Clause C.9.3	TerminateIvimEvent
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimTermination));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().log("<<< uppertester_ivim_codec::encode: Return -1");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_ivim_codec::encode: data=", data);
  return 0;
}

int uppertester_ivim_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_ivim_codec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> uppertester_ivim_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_ivim_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_ivim_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_ivim_codec::encode_: -1 result code returned");
          return -1;
        }
      } else {
        std::string s(r.fld_descr(i)->name);
        loggers::get_instance().log("uppertester_ivim_codec::encode_: Padding for %s", field_descriptor.name);
        if ((s.rfind(".timeStamp") != string::npos) || (s.rfind(".validFrom") != string::npos) || (s.rfind(".validTo") != string::npos) ||
            (s.rfind(".repetitionInterval") != string::npos)) { // Pad with six bytes
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
        } else { // Pad with one bytes
          encoding_buffer.put_c((unsigned char)0x00);
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("uppertester_ivim_codec::encode_: field to process %s", s.c_str());
    if (s.rfind(".iviIdentificationNumber") != string::npos) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      loggers::get_instance().log_msg("uppertester_ivim_codec::encode_: i=", i);
      encoding_buffer.put_string(int2oct(i, 2));
    } else if ((s.rfind(".timeStamp") != string::npos) || (s.rfind(".validTo") != string::npos) || (s.rfind(".validFrom") != string::npos) ||
               (s.rfind(".repetitionInterval") != string::npos)) {
      if (type.is_present()) {
        const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
        const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
        loggers::get_instance().log_msg("uppertester_ivim_codec::encode_: i=", i);
        encoding_buffer.put_string(int2oct(i, 6));
      }
    } else if ((s.rfind(".zoneIds") != string::npos) || (s.rfind(".relavanceZoneId") != string::npos) || (s.rfind(".detectionZoneId") != string::npos) ||
               (s.rfind(".driverAwarenesZoneId") != string::npos)) {
      if (type.is_present()) {
        const OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER> &o = dynamic_cast<const OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER> &>(type);
        const PreGenRecordOf::PREGEN__RECORD__OF__INTEGER &rov         = dynamic_cast<const PreGenRecordOf::PREGEN__RECORD__OF__INTEGER &>(*o.get_opt_value());
        encoding_buffer.put_c(static_cast<unsigned char>(rov.lengthof()));
        for (int i = 0; i < rov.lengthof(); i++) {
          const INTEGER &r = rov[i];
          loggers::get_instance().log_msg("uppertester_ivim_codec::encode_: r=", r);
          encoding_buffer.put_string(int2oct(r, 1));
        } // End of 'for' statement
      }
    } else if (s.rfind(".connectedIviStructures") != string::npos) {
      if (type.is_present()) {
        const OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER> &o = dynamic_cast<const OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER> &>(type);
        const PreGenRecordOf::PREGEN__RECORD__OF__INTEGER &rov         = dynamic_cast<const PreGenRecordOf::PREGEN__RECORD__OF__INTEGER &>(*o.get_opt_value());
        encoding_buffer.put_c(static_cast<unsigned char>(rov.lengthof()));
        for (int i = 0; i < rov.lengthof(); i++) {
          const INTEGER &r = rov[i];
          loggers::get_instance().log_msg("uppertester_ivim_codec::encode_: r=", r);
          encoding_buffer.put_string(int2oct(r, 4));
        } // End of 'for' statement
      }
    } else if ((s.rfind(".laneNumber") != string::npos) || (s.rfind(".itsRrid") != string::npos) || (s.rfind(".directionGic") != string::npos) ||
               (s.rfind(".layoutId") != string::npos)) {
      if (type.is_present()) {
        const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
        const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
        loggers::get_instance().log_msg("uppertester_ivim_codec::encode_: i=", i);
        encoding_buffer.put_string(int2oct(i, 1));
      }
    } else {
      loggers::get_instance().log("uppertester_ivim_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  loggers::get_instance().log_to_hexa("<<< uppertester_ivim_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_ivim_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> uppertester_ivim_codec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_ivim_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_ivim_codec::decode(const OCTETSTRING &data, params *params) {

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr != uppertester_ivim_codec::c_utIvimEventIndication) {
    LibItsIvim__TypesAndValues::UtIvimResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  } else {
    LibItsIvim__TypesAndValues::UtIvimEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  }

  return result;
}

int uppertester_ivim_codec::decode(const OCTETSTRING &data, LibItsIvim__TypesAndValues::UtIvimResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_ivim_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_ivim_codec::c_utIvimInitializeResult) {
    msg.utIvimInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_ivim_codec::c_utIvimTerminationResult) {
    msg.utIvimTerminationResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_ivim_codec::c_utIvimTriggerResult) {
    LibItsIvim__TypesAndValues::UtIvimTriggerResult r;
    ptr += 1;
    r.result() = BOOLEAN(*ptr == 0x01);
    ptr += 1;
    OCTETSTRING os(2, ptr);
    ptr += 2;
    r.iviIdentificationNumber() = oct2int(os);
    msg.utIvimTriggerResult()   = r;
  } else if (*ptr == uppertester_ivim_codec::c_utIvimUpdateResult) {
    LibItsIvim__TypesAndValues::UtIvimUpdateResult r;
    ptr += 1;
    r.result() = BOOLEAN(*ptr == 0x01);
    ptr += 1;
    OCTETSTRING os(2, ptr);
    ptr += 2;
    r.iviIdentificationNumber() = oct2int(os);
    msg.utIvimUpdateResult()    = r;
  } else if (*ptr == uppertester_ivim_codec::c_utIvimTerminationResult) {
    ptr += 1;
    msg.utIvimTerminationResult() = BOOLEAN(*ptr == 0x01);
  } else {
    TTCN_warning("uppertester_ivim_codec::decode: Unsupported result");
  }

  loggers::get_instance().log_msg("<<< uppertester_ivim_codec::decode", msg);
  return 0;
}

int uppertester_ivim_codec::decode(const OCTETSTRING &data, LibItsIvim__TypesAndValues::UtIvimEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_ivim_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("uppertester_ivim_codec::decode: IVIM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("uppertester_ivim_codec::decode: IVIM message=", os);
  ivim_codec codec;
  codec.decode(os, msg.iviMsg(), params);

  loggers::get_instance().log_msg("<<< uppertester_ivim_codec::decode", msg);
  return 0;
}
