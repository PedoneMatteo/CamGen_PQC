#include <string>
//#include <typeinfo>

#include "loggers.hh"
#include "uppertester_cam_codec.hh"

#include "LibItsCam_TypesAndValues.hh"

#include "cam_codec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_cam_codec::c_utCamInitialize              = 0x00;
unsigned char uppertester_cam_codec::c_utCamInitializeResult        = 0x01;
unsigned char uppertester_cam_codec::c_utCamChangePosition          = 0x02;
unsigned char uppertester_cam_codec::c_utCamChangePositionResult    = 0x03;
unsigned char uppertester_cam_codec::c_utCamChangeCurvature         = 0x30;
unsigned char uppertester_cam_codec::c_utCamChangeSpeed             = 0x31;
unsigned char uppertester_cam_codec::c_utCamSetAccelerationStatus   = 0x32;
unsigned char uppertester_cam_codec::c_utCamSetExteriorLightsStatus = 0x33;
unsigned char uppertester_cam_codec::c_utCamChangeHeading           = 0x34;
unsigned char uppertester_cam_codec::c_utCamSetDriveDirection       = 0x35;
unsigned char uppertester_cam_codec::c_utCamChangeYawRate           = 0x36;
unsigned char uppertester_cam_codec::c_utCamSetStationType          = 0x39;
unsigned char uppertester_cam_codec::c_utCamSetVehicleRole          = 0x3a;
unsigned char uppertester_cam_codec::c_utCamSetEmbarkationStatus    = 0x3b;
unsigned char uppertester_cam_codec::c_utCamSetPtActivation         = 0x3c;
unsigned char uppertester_cam_codec::c_utCamSetDangerousGoods       = 0x3d;
unsigned char uppertester_cam_codec::c_utCamSetLightBarSirene       = 0x3f;
unsigned char uppertester_cam_codec::c_utCamTriggerResult           = 0x21;
unsigned char uppertester_cam_codec::c_utCamEventInd                = 0x23;

int uppertester_cam_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_cam_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsCam_TypesAndValues.UtCamInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamInitialize));
  } else if (std::string(msg.get_descriptor()->name).compare("@LibItsCam_TypesAndValues.UtCamChangePosition") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamChangePosition));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().warning("<<< uppertester_cam_codec::encode: Failed to encode UT message");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_cam_codec::encode: data=", data);
  return 0;
}

int uppertester_cam_codec::encode(const LibItsCam__TypesAndValues::UtCamTrigger &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_cam_codec::encode: ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  // const Record_Type *type;
  const LibItsCam__TypesAndValues::UtCamTrigger &u = static_cast<const LibItsCam__TypesAndValues::UtCamTrigger &>(msg);
  if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_changeCurvature)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamChangeCurvature));
    encoding_buffer.put_string(int2oct(u.changeCurvature(), 2));
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_changeSpeed)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamChangeSpeed));
    encoding_buffer.put_string(int2oct(u.changeSpeed(), 2));
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setAccelerationControlStatus)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetAccelerationStatus));
    // Convert into ASN.1 encoded format
    unsigned char c = *static_cast<const unsigned char *>(u.setAccelerationControlStatus());
    unsigned char r = 0x00;
    for (int i = 0; i < 8; i++, c >>= 1) { // TODO Try to refine the reverse bit operation
      r = (r << 1) | (c & 0x01);
    } // End of 'for' statement
    encoding_buffer.put_c(r);
    // encoding_buffer.put_c(*static_cast<const unsigned char*>(u.setAccelerationControlStatus()));
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setExteriorLightsStatus)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetExteriorLightsStatus));
    // Convert into ASN.1 encoded format
    unsigned char c = *static_cast<const unsigned char *>(u.setExteriorLightsStatus());
    unsigned char r = 0x00;
    for (int i = 0; i < 8; i++, c >>= 1) { // TODO Try to refine the reverse bit operation
      r = (r << 1) | (c & 0x01);
    } // End of 'for' statement
    encoding_buffer.put_c(r);
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_changeHeading)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamChangeHeading));
    encoding_buffer.put_string(int2oct(u.changeHeading(), 2));
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setDriveDirection)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetDriveDirection));
    encoding_buffer.put_c((unsigned char)static_cast<const Enum_Type &>(u.setDriveDirection()).as_int());
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_changeYawRate)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamChangeYawRate));
    const short         i    = u.changeYawRate();
    const unsigned char t[2] = {(unsigned char)((i & 0xff00) >> 8), (unsigned char)(i & 0x00ff)};
    OCTETSTRING         os(2, t);
    encoding_buffer.put_string(os);
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setStationType)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetStationType));
    encoding_buffer.put_string(int2oct(u.setStationType(), 1));
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setVehicleRole)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetVehicleRole));
    encoding_buffer.put_c((unsigned char)static_cast<const Enum_Type &>(u.setVehicleRole()).as_int());
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setEmbarkationStatus)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetEmbarkationStatus));
    encoding_buffer.put_c((u.setEmbarkationStatus() == true) ? 0xff : 0x00);
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setDangerousGoods)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetDangerousGoods));
    encoding_buffer.put_c((unsigned char)static_cast<const Enum_Type &>(u.setDangerousGoods()).as_int());
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setPtActivation)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetPtActivation));
    encoding_buffer.put_c(static_cast<const unsigned char>(u.setPtActivation().ptActivationType()));
    const OCTETSTRING &os = u.setPtActivation().ptActivationData();
    encoding_buffer.put_c(static_cast<const unsigned char>(os.lengthof()));
    encoding_buffer.put_string(os);
  } else if (u.ischosen(LibItsCam__TypesAndValues::UtCamTrigger::ALT_setLightBarSirene)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamSetLightBarSirene));
    loggers::get_instance().log("uppertester_cam_codec::encode: BarSirene=0x%02x - 0x%02x", *static_cast<const unsigned char *>(u.setLightBarSirene()),
                                *static_cast<const unsigned char *>(u.setLightBarSirene()) << 6);
    encoding_buffer.put_c(*static_cast<const unsigned char *>(u.setLightBarSirene()) << 6);
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().warning("<<< uppertester_cam_codec::encode: Failed to encode UT message");
    return -1;
  }
  // encode_(*type, *type->get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_cam_codec::encode: data=", data);
  return 0;
}

int uppertester_cam_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_cam_codec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> uppertester_cam_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_cam_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_cam_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_cam_codec::encode_: -1 result code returned");
          return -1;
        }
      } else if (std::string(r.fld_name(i)).compare("alacarte") == 0) {
        // Add empty field length
        loggers::get_instance().log("uppertester_cam_codec::encode_: alacarte is missing, add 0x00");
        encoding_buffer.put_c(0x00);
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("uppertester_cam_codec::encode_: field to process %s", s.c_str());
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
      loggers::get_instance().log("uppertester_cam_codec::encode_ : detectionTime=%llu", llu);
      std::vector<unsigned char> v;
      for (int i = 0; i < 6; i++) {
        v.insert(v.begin(), static_cast<unsigned char>(llu));
        llu >>= 8;
      } // End of 'for' statement
      OCTETSTRING os(v.size(), v.data());
      loggers::get_instance().log_msg("uppertester_cam_codec::encode_: timeDetection=", os);
      encoding_buffer.put_string(os);
    } else if ((s.rfind(".validityDuration") != string::npos) || (s.rfind(".repetitionDuration") != string::npos)) {
      if (type.is_present()) {
        const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
        const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
        loggers::get_instance().log_msg("uppertester_cam_codec::encode_: i=", i);
        encoding_buffer.put_string(int2oct(i, 3));
      }
    } else if ((s.rfind(".informationQuality") != string::npos) || (s.rfind(".causeCode") != string::npos) || (s.rfind(".subCauseCode") != string::npos)) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      encoding_buffer.put_string(int2oct(i, 1));
    } else if ((s.rfind(".linkedCause") != string::npos) || (s.rfind(".eventHistory") != string::npos)) {
      // Skip them
    } else if ((s.rfind(".transmissionInterval") != string::npos) || (s.rfind(".repetitionInterval") != string::npos)) {
      if (type.is_present()) {
        const OPTIONAL<INTEGER> &o = dynamic_cast<const OPTIONAL<INTEGER> &>(type);
        const INTEGER &          i = static_cast<const INTEGER &>(*o.get_opt_value());
        encoding_buffer.put_string(int2oct(i, 2));
      }
    } else if (s.rfind(".originatingStationID") != string::npos) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      encoding_buffer.put_string(int2oct(i, 4));
    } else if (s.rfind(".sequenceNumber") != string::npos) {
      const INTEGER &i = static_cast<const INTEGER &>(type);
      encoding_buffer.put_string(int2oct(i, 2));
    } else if (s.rfind(".AlacarteContainer") != string::npos) {
      const OPTIONAL<OCTETSTRING> &o  = dynamic_cast<const OPTIONAL<OCTETSTRING> &>(type);
      const OCTETSTRING &          os = static_cast<const OCTETSTRING &>(*o.get_opt_value());
      encoding_buffer.put_string(int2oct(os.lengthof(), 2));
      encoding_buffer.put_string(os);
    } else {
      loggers::get_instance().log("uppertester_cam_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
      type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  loggers::get_instance().log_to_hexa("<<<uppertester_cam_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_cam_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> uppertester_cam_codec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_cam_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_cam_codec::decode(const OCTETSTRING &data, params *params) {

  std::unique_ptr<Base_Type> result;

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr != uppertester_cam_codec::c_utCamEventInd) {
    LibItsCam__TypesAndValues::UtCamResults res;
    if (decode(data, res, params) == 0) {
      result.reset((Base_Type *)res.clone());
    } else {
      result.reset(nullptr);
    }
  } else {
    LibItsCam__TypesAndValues::UtCamEventInd ind;
    decode(data, ind, params);
    result.reset((Base_Type *)ind.clone());
  }

  return result;
}

int uppertester_cam_codec::decode(const OCTETSTRING &data, LibItsCam__TypesAndValues::UtCamResults &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_cam_codec::decode: decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_cam_codec::c_utCamInitializeResult) {
    msg.utCamInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_cam_codec::c_utCamChangePositionResult) {
    msg.utCamChangePositionResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == uppertester_cam_codec::c_utCamTriggerResult) {
    msg.utCamTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
  } else if (*ptr == 0x24) { // TODO Use a constant for "Peer does not support is "Peer does not support the command"
    // Peer does not support the command
    msg.utCamInitializeResult() = 0x00;
  } else {
    loggers::get_instance().warning("uppertester_cam_codec::decode: Unsupported result");
    return -1;
  }

  loggers::get_instance().log_msg("<<< uppertester_cam_codec::decode", msg);
  return 0;
}

int uppertester_cam_codec::decode(const OCTETSTRING &data, LibItsCam__TypesAndValues::UtCamEventInd &msg, params *params) {
  loggers::get_instance().log_to_hexa(">>> uppertester_cam_codec::decode (1): decoding_buffer=", data);

  const unsigned char *ptr = static_cast<const unsigned char *>(data) + 1;
  OCTETSTRING          os(2, ptr);
  const unsigned int   length = (const unsigned int)((*ptr << 8 & 0xff00) | *(ptr + 1));
  ptr += 2;
  loggers::get_instance().log("uppertester_cam_codec::decode (1): CAM message length=%d", length);
  if (data.lengthof() - 3 != (const int)length) {
    loggers::get_instance().warning("uppertester_cam_codec::decode (1) (4): Wrong payload length: %d", length);
    return -1;
  }
  os = OCTETSTRING(length, ptr);
  loggers::get_instance().log_to_hexa("uppertester_cam_codec::decode (1): CAM message=", os);
  cam_codec codec;
  codec.decode(os, msg.camMsg(), params);

  loggers::get_instance().log_msg("<<< uppertester_cam_codec::decode (1)", msg);
  return 0;
}
