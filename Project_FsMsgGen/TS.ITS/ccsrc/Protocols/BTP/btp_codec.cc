#include <string>
//#include <typeinfo>

#include "btp_codec.hh"
#include "loggers.hh"

#include "LibItsBtp_TypesAndValues.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

int btp_codec::encode(const LibItsBtp__TypesAndValues::BtpPacket &msg, OCTETSTRING &data) {
  // loggers::get_instance().log_msg(">>> btp_codec::encode: ", (const Base_Type&)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());

  // loggers::get_instance().log_msg("<<< btp_codec::encode: data=", data);
  return 0;
}

int btp_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  // loggers::get_instance().log(">>> btp_codec::encode_: processing %s/%s", type.get_descriptor()->name, field_descriptor.name);
  // loggers::get_instance().log_msg(">>> btp_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    // loggers::get_instance().log("btp_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      // loggers::get_instance().log("btp_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
      // r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          // loggers::get_instance().warning("btp_codec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else if (std::string(field_descriptor.name).compare("@LibItsBtp_TypesAndValues.BtpPacket.payload") == 0) {
    const OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload> &v = dynamic_cast<const OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload> &>(type);
    // loggers::get_instance().log("btp_codec::encode_: BtpRawPayload present: %d", v.is_present());
    if (v.is_present()) {
      if (encode_(static_cast<const LibItsBtp__TypesAndValues::BtpRawPayload &>(*v.get_opt_value()), LibItsBtp__TypesAndValues::BtpRawPayload_descr_,
                  encoding_buffer) == -1) {
        // loggers::get_instance().warning("btp_codec::encode_: -1 result code returned");
        return -1;
      }
    }
  } else {
    // loggers::get_instance().log("btp_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
  }

  // loggers::get_instance().log_to_hexa("<<<btp_codec::encode_: encoding_buffer=", encoding_buffer);
  return 0;
}

int btp_codec::decode(const OCTETSTRING &data, LibItsBtp__TypesAndValues::BtpPacket &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);
  _params = params;

  // loggers::get_instance().log_to_hexa(">>> btp_codec::decode: decoding_buffer=", decoding_buffer);
  decode_(msg, *msg.get_descriptor(), decoding_buffer);

  // loggers::get_instance().log_msg("<<< btp_codec::decode: ", (const Base_Type&)msg);
  return 0;
}

int btp_codec::decode_(Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &decoding_buffer) {
  // loggers::get_instance().log(">>> btp_codec::decode_: processing %s/%s (%d,%d,%p)", type.get_descriptor()->name, field_descriptor.name,
  // decoding_buffer.get_len(), decoding_buffer.get_pos(), decoding_buffer.get_read_data());

  if (dynamic_cast<Record_Type *>(&type) != NULL) {
    Record_Type &r = (Record_Type &)type;
    // loggers::get_instance().log("btp_codec::decode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      // loggers::get_instance().log("btp_codec::decode_: processing %s/%s/%s", r.fld_name(i), r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name);
      if (decode_(*dynamic_cast<Base_Type *>(r.get_at(i)), *(r.fld_descr(i)), decoding_buffer) == -1) {
        return -1;
      }
    } // End of 'for' statement
  } else {
    // loggers::get_instance().log("btp_codec::decode_ (else): processing type %s/%s - optional:%d", type.get_descriptor()->name, field_descriptor.name,
    // type.is_optional());
    if (std::string(field_descriptor.name).compare("@LibItsBtp_TypesAndValues.BtpPacket.header") == 0) {
      // loggers::get_instance().log("btp_codec::decode_: _type = %d", _type);
      LibItsBtp__TypesAndValues::BtpHeader &b = static_cast<LibItsBtp__TypesAndValues::BtpHeader &>(type);
      if (_type == btp_codec::btpA) {
        decode_(b.btpAHeader(), *b.btpAHeader().get_descriptor(), decoding_buffer);
        if (_params != NULL) {
          (*_params)[params::btp_type]             = std::string("btpA");
          (*_params)[params::btp_destination_port] = std::to_string(static_cast<int>(b.btpAHeader().destinationPort()));
        }
      } else if (_type == btp_codec::btpB) {
        decode_(b.btpBHeader(), *b.btpBHeader().get_descriptor(), decoding_buffer);
        if (_params != NULL) {
          (*_params)[params::btp_type]             = std::string("btpB");
          (*_params)[params::btp_destination_port] = std::to_string(static_cast<int>(b.btpBHeader().destinationPort()));
          (*_params)[params::btp_info]             = std::to_string(static_cast<int>(b.btpBHeader().destinationPortInfo()));
        }
      } else {
        // loggers::get_instance().error("btp_codec::decode_: Invalid BTP type"); // Cannot be reached
      }
      // loggers::get_instance().log_msg("btp_codec::decode_: BtpHeader: ", type);
    } else if (std::string(field_descriptor.name).compare("@LibItsBtp_TypesAndValues.BtpPacket.payload") == 0) {
      if (decoding_buffer.get_len() == decoding_buffer.get_pos()) { // No payload, set it to omit
        // loggers::get_instance().log("btp_codec::decode_: Set OPTIONAL<BtpRawPayload> to omit");
        dynamic_cast<OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload> &>(type).set_to_omit();
      } else {
        // Decode BtpRawPayload data structure
        OCTETSTRING os(decoding_buffer.get_len() - decoding_buffer.get_pos(), decoding_buffer.get_data() + decoding_buffer.get_pos());
        // loggers::get_instance().log_msg("btp_codec::decode_: s=", os);
        if (_params != NULL) {
          (*_params)[params::btp_payload] = std::string(static_cast<const char *>(oct2str(os)));
        }
        if (os.lengthof() != 0) {
          dynamic_cast<OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload> &>(type) = OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload>(os);
          // loggers::get_instance().log_msg("btp_codec::decode_: Set OPTIONAL to ", type);
        } else {
          // loggers::get_instance().log("btp_codec::decode_: Set OPTIONAL to omit");
          dynamic_cast<OPTIONAL<LibItsBtp__TypesAndValues::BtpRawPayload> &>(type).set_to_omit();
        }
      }
    } else {
      type.decode(field_descriptor, decoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  // loggers::get_instance().log_msg("<<< btp_codec::decode_: ", type);
  // loggers::get_instance().log_to_hexa("<<< btp_codec::decode_: decoding_buffer=", decoding_buffer);

  return 0;
}
