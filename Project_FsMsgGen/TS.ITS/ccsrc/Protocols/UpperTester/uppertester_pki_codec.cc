#include <string>
//#include <typeinfo>

#include "loggers.hh"
#include "uppertester_pki_codec.hh"

#include "LibItsPki_TestSystem.hh"
#include "LibItsPki_TypesAndValues.hh"

//#include "pki_codec.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

unsigned char uppertester_pki_codec::c_utPkiInitialize                  = 0x00;
unsigned char uppertester_pki_codec::c_utPkiInitializeResult            = 0x01;
unsigned char uppertester_pki_codec::c_utPkiTriggerEnrolmentRequest     = 0xD0;
unsigned char uppertester_pki_codec::c_utPkiTriggerAuthorizationRequest = 0xD1;
unsigned char uppertester_pki_codec::c_utPkiTriggerResult               = 0xD2;
unsigned char uppertester_pki_codec::c_utPkiTriggerInd                  = 0xD3;

int uppertester_pki_codec::encode(const Record_Type &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_pki_codec::encode (1): ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  if (std::string(msg.get_descriptor()->name).compare("@LibItsPki_TypesAndValues.UtPkiInitialize") == 0) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiInitialize));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().warning("<<< uppertester_pki_codec::encode: Failed to encode UT message");
    return -1;
  }
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_pki_codec::encode: data=", data);
  return 0;
}

int uppertester_pki_codec::encode(const LibItsPki__TypesAndValues::UtPkiTrigger &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_pki_codec::encode (2): ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  // const Record_Type *type;
  const LibItsPki__TypesAndValues::UtPkiTrigger &u = static_cast<const LibItsPki__TypesAndValues::UtPkiTrigger &>(msg);
  if (u.ischosen(LibItsPki__TypesAndValues::UtPkiTrigger::ALT_triggerEnrolmentRequest)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiTriggerEnrolmentRequest));
    /*encoding_buffer.put_c(static_cast<const unsigned char>(u.triggerEnrolmentRequest().cannonicalId().lengthof())); // Length of the string
    encoding_buffer.put_string(u.triggerEnrolmentRequest().cannonicalId());
    encoding_buffer.put_os(u.triggerEnrolmentRequest().encAlgorithm()); // One byte
    encoding_buffer.put_os(u.triggerEnrolmentRequest().privateKey()); // 32 bytes
    encoding_buffer.put_os(u.triggerEnrolmentRequest().compressedPublickey()); //33 bytes
    */
  } else if (u.ischosen(LibItsPki__TypesAndValues::UtPkiTrigger::ALT_triggerAuthorizationRequest)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiTriggerAuthorizationRequest));
  } else { // Error
    data = OCTETSTRING(0, nullptr);
    loggers::get_instance().warning("<<< uppertester_pki_codec::encode: Failed to encode UT message");
    return -1;
  }
  // encode_(*type, *type->get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_pki_codec::encode: data=", data);
  return 0;
}

int uppertester_pki_codec::encode(const LibItsPki__TypesAndValues::UtPkiTriggerInd &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> uppertester_pki_codec::encode (3): ", (const Record_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  // const Record_Type *type;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiTriggerInd));
  encoding_buffer.put_c(static_cast<const unsigned char>(msg.state()));
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  loggers::get_instance().log_msg("<<< uppertester_pki_codec::encode: data=", data);
  return 0;
}

int uppertester_pki_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  loggers::get_instance().log(">>> uppertester_pki_codec::encode_: processing %s/%s/%p", type.get_descriptor()->name, field_descriptor.name,
                              dynamic_cast<const Record_Type *>(&type));
  loggers::get_instance().log_msg(">>> uppertester_pki_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    loggers::get_instance().log("uppertester_pki_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      loggers::get_instance().log("uppertester_pki_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
                                  r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          loggers::get_instance().warning("uppertester_pki_codec::encode_: -1 result code returned");
          return -1;
        }
      } else if (std::string(r.fld_name(i)).compare("alacarte") == 0) {
        // Add empty field length
        loggers::get_instance().log("uppertester_pki_codec::encode_: alacarte is missing, add 0x00");
        encoding_buffer.put_c(0x00);
      }
    } // End of 'for' statement
  } else {
    std::string s(field_descriptor.name);
    loggers::get_instance().log("uppertester_pki_codec::encode_: field to process %s", s.c_str());

    loggers::get_instance().log("uppertester_pki_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
  }

  loggers::get_instance().log_to_hexa("<<<uppertester_pki_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int uppertester_pki_codec::decode(const OCTETSTRING &data, Record_Type &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  loggers::get_instance().log_to_hexa(">>> uppertester_pki_codec::decode: decoding_buffer=", decoding_buffer);
  // decode_(msg, *msg.get_descriptor(), decoding_buffer);

  loggers::get_instance().log_msg("<<< uppertester_pki_codec::decode: ", (const Record_Type &)msg);
  return 0;
}

std::unique_ptr<Base_Type> uppertester_pki_codec::decode(const OCTETSTRING &data, params *params) {

  std::unique_ptr<Base_Type> result;

  const unsigned char *                   ptr = static_cast<const unsigned char *>(data);
  if (*ptr == uppertester_pki_codec::c_utPkiInitializeResult) {
    LibItsPki__TypesAndValues::UtPkiResults msg;
    msg.utPkiInitializeResult() = BOOLEAN(*(ptr + 1) == 0x01);
    result.reset((Base_Type *)msg.clone());
  } else if (*ptr == uppertester_pki_codec::c_utPkiTriggerResult) {
    LibItsPki__TypesAndValues::UtPkiResults msg;
    msg.utPkiTriggerResult() = BOOLEAN(*(ptr + 1) == 0x01);
    result.reset((Base_Type *)msg.clone());
  } else if (*ptr == uppertester_pki_codec::c_utPkiTriggerInd) {
    LibItsPki__TypesAndValues::UtPkiTriggerInd msg;
    msg.state() = INTEGER(static_cast<int>(*(ptr + 1)));
    result.reset((Base_Type *)msg.clone());
 } else {
    result.reset(nullptr);
  }

  return result;
}
