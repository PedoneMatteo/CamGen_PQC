#include <string>
//#include <typeinfo>

#include "loggers.hh"
#include "uppertester_mapem_spatem_codec.hh"

#include "LibItsMapemSpatem_TestSystem.hh"
#include "LibItsMapemSpatem_TypesAndValues.hh"

#include "mapem_codec.hh"
#include "spatem_codec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_mapem_spatem_codec::c_utMapemSpatemInitialize       = 0x00;
unsigned char uppertester_mapem_spatem_codec::c_utMapemSpatemInitializeResult = 0x01;
unsigned char uppertester_mapem_spatem_codec::c_utMapemSpatemTrigger          = 0x7A;
unsigned char uppertester_mapem_spatem_codec::c_utMapemSpatemTriggerResult    = 0x7B;
unsigned char uppertester_mapem_spatem_codec::c_utMapemEventIndication        = 0x7C;
unsigned char uppertester_mapem_spatem_codec::c_utSpatemEventIndication       = 0x7D;

int uppertester_mapem_spatem_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsMapemSpatem_TypesAndValues.UtMapemSpatemInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utMapemSpatemInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsMapemSpatem_TypesAndValues.UtMapemSpatemTrigger") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utMapemSpatemTrigger));
    // See ETSI TR 103 099 V1.4.1 Clause C.9.1 GenerateMapemSpatemEvent
    unsigned char                                                  flags = 0x00;
    const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &r     = static_cast<const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &>(msg);
    if (r.intersectionID().is_present()) { // F
      flags |= 0x80;
    }
    if (r.regulatorySpeedLimit().is_present()) { // V
      flags |= 0x40;
    }
    encoding_buffer.put_c(flags);
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().log("<<< uppertester_mapem_spatem_codec::encode: Return -1");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_mapem_spatem_codec::encode: data=", data);
  return 0;
}

int uppertester_mapem_spatem_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_mapem_spatem_codec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> uppertester_mapem_spatem_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d",
                                  r.fld_name(i), r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(),
                                  r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_mapem_spatem_codec::encode_: -1 result code returned");
          return -1;
        }
      } else {
        std::string s(r.fld_descr(i)->name);
        loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_: Padding for %s", field_descriptor.name);
        if (s.rfind(".intersectionID") != string::npos) { // Pad with two bytes
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
        } else if (s.rfind(".regulatorySpeedLimit") != string::npos) { // Pad with three bytes
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
          encoding_buffer.put_c((unsigned char)0x00);
        }
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_: field to process %s", s.c_str());
    loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    if (s.rfind(".intersectionID") != string::npos) {
      if (type.is_present()) {
        const OPTIONAL<INTEGER> &o  = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
        const INTEGER &          iv = dynamic_cast<const INTEGER &>(*o.get_opt_value());
        loggers::get_instance().log_msg("uppertester_mapem_spatem_codec::encode_: iv=", iv);
        OCTETSTRING os = int2oct(iv, 2);
        encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
      }
    } else if (s.rfind(".regulatorySpeedLimit") == 0) {
      if (type.is_present()) {
        const OPTIONAL<DSRC__noCircular::RegulatorySpeedLimit> &o =
          dynamic_cast<const OPTIONAL<DSRC__noCircular::RegulatorySpeedLimit> &>(type);
        const DSRC__noCircular::RegulatorySpeedLimit &rv = dynamic_cast<const DSRC__noCircular::RegulatorySpeedLimit &>(*o.get_opt_value());
        const int                                             t  = rv.type__().as_int();
        encoding_buffer.put_c((unsigned char)t);
        const int   s  = rv.speed();
        OCTETSTRING os = int2oct(s, 2);
        encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
      }
    } else {
      loggers::get_instance().log("uppertester_mapem_spatem_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  loggers::get_instance().log_to_hexa("<<<uppertester_mapem_spatem_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_mapem_spatem_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> uppertester_mapem_spatem_codec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_mapem_spatem_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_mapem_spatem_codec::decode(const OCTETSTRING &data, params *params) {

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if ((*ptr != uppertester_mapem_spatem_codec::c_utMapemEventIndication) && (*ptr != uppertester_mapem_spatem_codec::c_utSpatemEventIndication)) {
    LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults res;
    decode(data, res, params);
    result.reset((Base_Type *)res.clone());
  } else if (*ptr == uppertester_mapem_spatem_codec::c_utMapemEventIndication) {
    LibItsMapemSpatem__TypesAndValues::UtMapemEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  } else {
    LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  }

  return result;
}

int uppertester_mapem_spatem_codec::decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_mapem_spatem_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_mapem_spatem_codec::c_utMapemSpatemInitializeResult) {
    msg.utMapemSpatemInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_mapem_spatem_codec::c_utMapemSpatemTriggerResult) {
    msg.utMapemSpatemTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else {
    TTCN_warning("uppertester_mapem_spatem_codec::decode: Unsupported result");
  }

  loggers::get_instance().log_msg("<<< uppertester_mapem_spatem_codec::decode", msg);
  return 0;
}

int uppertester_mapem_spatem_codec::decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_mapem_spatem_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("uppertester_mapem_spatem_codec::decode: MAPEM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("uppertester_mapem_spatem_codec::decode: MAPEM message=", os);
  mapem_codec codec;
  codec.decode(os, msg.mapeMsg(), params);

  loggers::get_instance().log_msg("<<< uppertester_mapem_spatem_codec::decode", msg);
  return 0;
}

int uppertester_mapem_spatem_codec::decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_mapem_spatem_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  ptr += 2;
  unsigned int length = oct2int(os);
  loggers::get_instance().log("uppertester_mapem_spatem_codec::decode: SPATEM message length=%d", length);
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("uppertester_mapem_spatem_codec::decode: SPATEM message=", os);
  spatem_codec codec;
  codec.decode(os, msg.spateMsg(), params);

  loggers::get_instance().log_msg("<<< uppertester_mapem_spatem_codec::decode", msg);
  return 0;
}
