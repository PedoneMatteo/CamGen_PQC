#include <string>
//#include <typeinfo>

#include "loggers.hh"
#include "uppertester_geonetworking_codec.hh"

#include "LibItsGeoNetworking_TestSystem.hh"
#include "LibItsGeoNetworking_TypesAndValues.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_geonetworking_codec::c_utGnInitialize           = 0x00;
unsigned char uppertester_geonetworking_codec::c_utGnInitializeResult     = 0x01;
unsigned char uppertester_geonetworking_codec::c_utGnChangePosition       = 0x02;
unsigned char uppertester_geonetworking_codec::c_utGnChangePositionResult = 0x03;
unsigned char uppertester_geonetworking_codec::c_utGnTriggerResult        = 0x41;
unsigned char uppertester_geonetworking_codec::c_utGnTrigger_GeoUnicast   = 0x50;
unsigned char uppertester_geonetworking_codec::c_utGnTrigger_GeoBroadcast = 0x51;
unsigned char uppertester_geonetworking_codec::c_utGnTrigger_GeoAnycast   = 0x52;
unsigned char uppertester_geonetworking_codec::c_utGnTrigger_Shb          = 0x53;
unsigned char uppertester_geonetworking_codec::c_utGnTrigger_Tsb          = 0x54;
unsigned char uppertester_geonetworking_codec::c_utGnEventIndication      = 0x55;

int uppertester_geonetworking_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.UtGnInitialize") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.UtGnChangePosition") == 0) {
    encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnChangePosition));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());

  loggers::get_instance().log_msg("<<< uppertester_geonetworking_codec::encode: data=", data);
  return 0;
}

int uppertester_geonetworking_codec::encode(const Base_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log(">>> uppertester_geonetworking_codec::encode: %s", msg.get_descriptor()->name);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;

  Base_Type *type = nullptr;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.UtGnTrigger") == 0) {
    const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &t = static_cast<const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &>(msg);
    loggers::get_instance().log("uppertester_geonetworking_codec::encode: After cast: %s", t.get_descriptor()->name);
    if (t.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnTrigger::ALT_geoUnicast)) {
      encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnTrigger_GeoUnicast));
      type = (Base_Type *)&(t.geoUnicast());
    } else if (t.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnTrigger::ALT_geoBroadcast)) {
      encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnTrigger_GeoBroadcast));
      type = (Base_Type *)&(t.geoBroadcast());
    } else if (t.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnTrigger::ALT_geoAnycast)) {
      encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnTrigger_GeoAnycast));
      type = (Base_Type *)&(t.geoAnycast());
    } else if (t.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnTrigger::ALT_shb)) {
      encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnTrigger_Shb));
      type = (Base_Type *)&(t.shb());
    } else if (t.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnTrigger::ALT_tsb)) {
      encoding_buffer.put_c(static_cast<unsigned char>(uppertester_geonetworking_codec::c_utGnTrigger_Tsb));
      type = (Base_Type *)&(t.tsb());
    } // else, nothing to do
  } else {
    data = OCTETSTRING(0, nullptr);
    return -1;
  }
  encode_(*type, *(type->get_descriptor()), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());

  loggers::get_instance().log_msg("<<< uppertester_geonetworking_codec::encode: data=", data);

  return 0;
}

int uppertester_geonetworking_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_geonetworking_codec::encode_: processing %s/%s", type.get_descriptor()->name, field_descriptor.name);
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_geonetworking_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_geonetworking_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d",
                                  r.fld_name(i), r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(),
                                  r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_geonetworking_codec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    if (s.rfind(".shape") != string::npos) {
      encoding_buffer.put_c((unsigned char)static_cast<const Enum_Type &>(type).as_int());
    } else if (s.rfind(".payload") != string::npos) {
      const OCTETSTRING & os  = static_cast<const OCTETSTRING &>(type);
      const unsigned char s[] = {(unsigned char)((os.lengthof() & 0x0000FF00) >> 8), (unsigned char)os.lengthof()};
      encoding_buffer.put_s(2, s);
      if (os.lengthof() != 0) {
        encoding_buffer.put_string(os);
      }
    } else {
      loggers::get_instance().log("uppertester_geonetworking_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  loggers::get_instance().log_to_hexa("<<<uppertester_geonetworking_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_geonetworking_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::decode (1): ", data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa("uppertester_geonetworking_codec::decode: decoding_buffer=", decoding_buffer);
  //  decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_geonetworking_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_geonetworking_codec::decode(const OCTETSTRING &data, params *params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::decode (2): ", data);

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if ((*ptr == uppertester_geonetworking_codec::c_utGnInitializeResult) || (*ptr == uppertester_geonetworking_codec::c_utGnChangePositionResult) ||
      (*ptr == uppertester_geonetworking_codec::c_utGnTriggerResult)) {
    LibItsGeoNetworking__TypesAndValues::UtGnResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  } else if ((*ptr == uppertester_geonetworking_codec::c_utGnEventIndication)) {
    LibItsGeoNetworking__TypesAndValues::UtGnEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  } else {
    loggers::get_instance().warning("uppertester_geonetworking_codec::decode (2): Unsupported UpperTester command 0x%02x", *ptr);
  }

  return result;
}

int uppertester_geonetworking_codec::decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::UtGnResults &msg, params *params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::decode (3): ", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_geonetworking_codec::c_utGnInitializeResult) {
    msg.utGnInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_geonetworking_codec::c_utGnChangePositionResult) {
    msg.utGnChangePositionResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_geonetworking_codec::c_utGnTriggerResult) {
    msg.utGnTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == 0x24) { // TODO Use a constant for "Peer does not support is "Peer does not support the command"
    // Peer does not support the command
    msg.utGnInitializeResult() = 0x00;
  } else {
    loggers::get_instance().warning("uppertester_geonetworking_codec::decode: Unsupported result");
    return -1;
  }

  loggers::get_instance().log_msg("<<< uppertester_geonetworking_codec::decode", msg);
  return 0;
}

int uppertester_geonetworking_codec::decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::UtGnEventInd &msg, params *params) {
  loggers::get_instance().log_msg(">>> uppertester_geonetworking_codec::decode (4): ", data);

  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  const unsigned int length = (const unsigned int)((data[1].get_octet() << 8 & 0xff00) | data[2].get_octet());
  if (data.lengthof() - 3 != (const int)length) {
    loggers::get_instance().warning("uppertester_geonetworking_codec::decode (4): Wrong payload length: %d", length);
    return -1;
  }

  msg.rawPayload() = OCTETSTRING(length, 3 + static_cast<const unsigned char *>(data));

  loggers::get_instance().log_msg("<<< uppertester_geonetworking_codec::decode (4): ", msg);
  return 0;
}
