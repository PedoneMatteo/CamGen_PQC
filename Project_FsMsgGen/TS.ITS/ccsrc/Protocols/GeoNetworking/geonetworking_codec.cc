#include <string>
//#include <typeinfo>

#include "geonetworking_codec.hh"
#include "loggers.hh"

#include "etsi_ts103097_data_codec.hh"

#include "LibItsGeoNetworking_TypesAndValues.hh"

template <typename T_type> class OPTIONAL;
class TTCN_EncDec;

int geonetworking_codec::encode(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &msg, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> geonetworking_codec::encode (1): ", (const Base_Type &)msg);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;

  _ec.reset();

  encode_(msg, *msg.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  // Overwrite the payload length
  if (_ec.get_length() != (unsigned int)-1) {
    loggers::get_instance().log("geonetworking_codec::encode: length=%d - plLength_position = %d - plLength = %d", data.lengthof(), _ec.get_length_position(),
                                _ec.get_length());
    unsigned char b[] = {(unsigned char)((_ec.get_length() & 0x0000FF00) >> 8), (unsigned char)_ec.get_length()};
    OCTETSTRING   rpl(sizeof(b), b);
    loggers::get_instance().log_msg("geonetworking_codec::encode: rpl=", rpl);
    data = replace(data, _ec.get_length_position(), rpl.lengthof(), rpl);
    loggers::get_instance().log_msg("geonetworking_codec::encode: after replace: ", data);
  }
  loggers::get_instance().log_msg("geonetworking_codec::encode: After encoding: ", data);

  loggers::get_instance().log("<<< geonetworking_codec::encode");
  return 0;
}

int geonetworking_codec::encode(const LibItsGeoNetworking__TypesAndValues::GnNonSecuredPacket &p_gnNonSecuredPacket, OCTETSTRING &data) {
  loggers::get_instance().log_msg(">>> geonetworking_codec::encode (2): ", (const Base_Type &)p_gnNonSecuredPacket);

  TTCN_EncDec::clear_error();
  TTCN_Buffer encoding_buffer;
  encode_(p_gnNonSecuredPacket, *p_gnNonSecuredPacket.get_descriptor(), encoding_buffer);
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data());
  // Overwrite the payload length
  if (_ec.get_length() != (unsigned int)-1) {
    loggers::get_instance().log("geonetworking_codec::encode: length=%d - plLength_position = %d - plLength = %d", data.lengthof(), _ec.get_length_position(),
                                _ec.get_length());
    unsigned char b[] = {(unsigned char)((_ec.get_length() & 0x0000FF00) >> 8), (unsigned char)_ec.get_length()};
    OCTETSTRING   rpl(sizeof(b), b);
    loggers::get_instance().log_msg("geonetworking_codec::encode: rpl=", rpl);
    data = replace(data, _ec.get_length_position(), rpl.lengthof(), rpl);
    loggers::get_instance().log_msg("geonetworking_codec::encode: after replace: ", data);
  }
  loggers::get_instance().log_msg("geonetworking_codec::encode: After encoding: ", data);

  loggers::get_instance().log("<<< geonetworking_codec::encode");
  return 0;
}

int geonetworking_codec::encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer) {
  // loggers::get_instance().log(">>> geonetworking_codec::encode_: processing %s/%s", type.get_descriptor()->name, field_descriptor.name);
  // loggers::get_instance().log_msg(">>> geonetworking_codec::encode_: ", type);

  if (dynamic_cast<const Record_Type *>(&type) != NULL) {
    const Record_Type &r = (const Record_Type &)type;
    // loggers::get_instance().log("geonetworking_codec::encode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      // loggers::get_instance().log("geonetworking_codec::encode_: processing %s/%s/%s - %d (1 ==> use dynamic_cast<const OPTIONAL<...>) - %d", r.fld_name(i),
      // r.fld_descr(i)->name, r.get_at(i)->get_descriptor()->name, r.get_at(i)->is_optional(), r.get_at(i)->is_present());
      if (r.get_at(i)->is_present()) {
        if ((_ec.get_basic_header() == 0x02) && (std::string(r.fld_descr(i)->name).compare("@LibItsGeoNetworking_TypesAndValues.GeoNetworkingPacket.packet") ==
                                                 0)) { // Encoding secured GeoNetworking packet, unsecured GN message was already encoded using encvalue
          // loggers::get_instance().log("geonetworking_codec::encode_: Skip it");
          continue;
        }
        if (encode_(*r.get_at(i), *r.fld_descr(i), encoding_buffer) == -1) {
          // loggers::get_instance().warning("geonetworking_codec::encode_: -1 result code returned");
          return -1;
        }
      }
    } // End of 'for' statement
  } else if (std::string(type.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.ExtendedHeader") == 0) {
    const OPTIONAL<LibItsGeoNetworking__TypesAndValues::ExtendedHeader> &v =
      dynamic_cast<const OPTIONAL<LibItsGeoNetworking__TypesAndValues::ExtendedHeader> &>(type);
    // loggers::get_instance().log("geonetworking_codec::encode_: ExtendedHeader present: %d", v.is_present());
    if (v.is_present()) {
      if (encode_extendedHeader(static_cast<const LibItsGeoNetworking__TypesAndValues::ExtendedHeader &>(*v.get_opt_value()), encoding_buffer) == -1) {
        loggers::get_instance().warning("geonetworking_codec::encode_: -1 result code returned");
        return -1;
      }
    }
  } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.GnNonSecuredPacket.payload") == 0) {
    const OPTIONAL<LibItsGeoNetworking__TypesAndValues::GnRawPayload> &v =
      dynamic_cast<const OPTIONAL<LibItsGeoNetworking__TypesAndValues::GnRawPayload> &>(type);
    // loggers::get_instance().log("geonetworking_codec::encode_: GnRawPayload present: %d", v.is_present());
    if (v.is_present()) {
      const LibItsGeoNetworking__TypesAndValues::GnRawPayload &p = static_cast<const LibItsGeoNetworking__TypesAndValues::GnRawPayload &>(*v.get_opt_value());
      if (encode_(p, *p.get_descriptor() /*LibItsGeoNetworking__TypesAndValues::GnRawPayload_descr_*/, encoding_buffer) == -1) {
        loggers::get_instance().warning("geonetworking_codec::encode_: -1 result code returned");
        return -1;
      }
      // Store playload length and continue encoding
      _ec.set_length(p.lengthof());
      // loggers::get_instance().log("geonetworking_codec::encode_: Storing payload length %d for %s", _ec.get_length(), p.get_descriptor()->name);
    }
  } else if (std::string(field_descriptor.name).compare("@EtsiTs103097Module.EtsiTs103097Data") == 0) {
    const EtsiTs103097Module::EtsiTs103097Data &ieee_1609dot2_data = static_cast<const EtsiTs103097Module::EtsiTs103097Data &>(type);
    // loggers::get_instance().log_msg("geonetworking_codec::encode_: Encode secured message= ", ieee_1609dot2_data);
    etsi_ts103097_data_codec codec;
    OCTETSTRING              os;
    codec.encode(ieee_1609dot2_data, os);
    if (os.lengthof() == 0) {
      loggers::get_instance().warning("geonetworking_codec::encode_: Failed to encode EtsiTs103097Data");
      return -1;
    }
    // loggers::get_instance().log_msg("geonetworking_codec::encode_: ", os);
    encoding_buffer.put_string(os);
  } else {
    // loggers::get_instance().log("geonetworking_codec::encode_ (else): processing type %s/%s", type.get_descriptor()->name, field_descriptor.name);
    if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.BasicHeader.nextHeader") == 0) {
      // Store NextHeader field value and continue encoding
      _ec.set_basic_header((unsigned int)dynamic_cast<const Enum_Type &>(type).as_int());
      // loggers::get_instance().log("geonetworking_codec::encode_: Storing basic_header %d for %s", _ec.get_next_header(), field_descriptor.name);
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.nextHeader") == 0) {
      // Store NextHeader field value and continue encoding
      _ec.set_next_header((unsigned int)dynamic_cast<const Enum_Type &>(type).as_int());
      // loggers::get_instance().log("geonetworking_codec::encode_: Storing next_header %d for %s", _ec.get_next_header(), field_descriptor.name);
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.headerType") == 0) {
      // Store HeaderType field value and continue encoding
      _ec.set_header_type((unsigned int)dynamic_cast<const Enum_Type &>(type).as_int());
      // loggers::get_instance().log("geonetworking_codec::encode_: Storing header_type %d for %s", _ec.get_header_type(), field_descriptor.name);
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.plLength") == 0) {
      // Store TTCN_Buffer position and continue encoding
      _ec.set_length_position(encoding_buffer.get_len());
      // loggers::get_instance().log("geonetworking_codec::encode_: Storing position %d for %s", _ec.get_length_position(), field_descriptor.name);
    }
    type.encode(field_descriptor, encoding_buffer, TTCN_EncDec::CT_RAW);
  }

  // loggers::get_instance().log_to_hexa("<<< geonetworking_codec::encode_: ", encoding_buffer);
  return 0;
}

int geonetworking_codec::decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &msg, params *params) {
  TTCN_EncDec::clear_error();
  TTCN_Buffer decoding_buffer(data);

  _dc.reset();

  _params = params;

  loggers::get_instance().log_to_hexa(">>> geonetworking_codec::decode: decoding_buffer=", decoding_buffer);
  decode_(msg, *msg.get_descriptor(), decoding_buffer);
  loggers::get_instance().log("geonetworking_codec::decode: After decoding, payload: plLength=%d", _dc.get_length());
  // Set layer parameters
  if (_params != NULL) {
    (*_params)[params::gn_next_header]     = std::to_string((unsigned int)_dc.get_next_header());
    (*_params)[params::gn_header_type]     = std::to_string((unsigned int)_dc.get_header_type());
    (*_params)[params::gn_header_sub_type] = std::to_string((unsigned int)_dc.get_header_sub_type());
    (*_params)[params::gn_traffic_class]   = std::to_string((unsigned int)_dc.get_traffic_class());
    (*_params)[params::gn_lifetime]        = std::to_string(_dc.get_lifetime());
    params->log();
  }

  loggers::get_instance().log_msg("<<< geonetworking_codec::decode: ", (const Base_Type &)msg);
  return 0;
}

int geonetworking_codec::decode_(Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &decoding_buffer) {
  // loggers::get_instance().log(">>> geonetworking_codec::decode_: processing %s/%s (%d,%d,%p)", type.get_descriptor()->name, field_descriptor.name,
  // decoding_buffer.get_len(), decoding_buffer.get_pos(), decoding_buffer.get_read_data());

  if (dynamic_cast<Record_Type *>(&type) != NULL) {
    Record_Type &r = (Record_Type &)type;
    // loggers::get_instance().log("geonetworking_codec::decode_: processing Record_Type %s", r.get_descriptor()->name);
    for (int i = 0; i < r.get_count(); i++) {
      // loggers::get_instance().log("geonetworking_codec::decode_: processing %s/%s/%s", r.fld_name(i), r.fld_descr(i)->name,
      // r.get_at(i)->get_descriptor()->name);
      if (decode_(*dynamic_cast<Base_Type *>(r.get_at(i)), *(r.fld_descr(i)), decoding_buffer) == -1) {
        loggers::get_instance().warning("geonetworking_codec::decode_: -1 result code returned");
        return -1;
      }
    } // End of 'for' statement
    // Save data for upper layer
    if (std::string(r.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.Lifetime") == 0) {
      LibItsGeoNetworking__TypesAndValues::Lifetime &l    = static_cast<LibItsGeoNetworking__TypesAndValues::Lifetime &>(r);
      unsigned int                                   base = 0;
      switch (l.ltBase().as_int()) {
        case LibItsGeoNetworking__TypesAndValues::LtBase::e__50ms:
          base = 50;
          break;
        case LibItsGeoNetworking__TypesAndValues::LtBase::e__1s:
          base = 1000;
          break;
        case LibItsGeoNetworking__TypesAndValues::LtBase::e__10s:
          base = 10000;
          break;
        case LibItsGeoNetworking__TypesAndValues::LtBase::e__100s:
          base = 100000;
          break;
      } // End of 'switch' statement
      _dc.set_lifetime(base * static_cast<unsigned int>(l.multiplier()));
      // loggers::get_instance().log("geonetworking_codec::decode_: Lifetime calculus: %d - %s", _dc.get_lifetime(),
      // std::to_string(_dc.get_lifetime()).c_str());
    } else if (std::string(type.get_descriptor()->name).compare("@LibItsGeoNetworking_TypesAndValues.TrafficClass") == 0) {
      LibItsGeoNetworking__TypesAndValues::TrafficClass &t = static_cast<LibItsGeoNetworking__TypesAndValues::TrafficClass &>(r);
      _dc.set_traffic_class((t.scf().as_int() << 7) | (t.channelOffload() << 6) | (t.tcId() & 0x3f)); // ETSI EN 302 636-4-1 chapter 8.7.5
    }
  } else if (std::string(type.get_descriptor()->name).compare("@Ieee1609Dot2.Ieee1609Dot2Data") == 0) {
    dynamic_cast<OPTIONAL<Ieee1609Dot2::Ieee1609Dot2Data> &>(type).set_to_omit(); // TODO Add security support
  } else {
    // loggers::get_instance().log("geonetworking_codec::decode_ (else): processing type %s/%s - optional:%d", type.get_descriptor()->name,
    // field_descriptor.name, type.is_optional());
    if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.nextHeader") == 0) {
      // Decode NextHeader data structure
      type.decode(field_descriptor, decoding_buffer, TTCN_EncDec::CT_RAW);
      // Store NextHeader field value and continue decoding
      _dc.set_next_header((unsigned int)dynamic_cast<const Enum_Type &>(type).as_int());
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.headerTST") == 0) {
      decode_headerTST(static_cast<LibItsGeoNetworking__TypesAndValues::HeaderTST &>(type), decoding_buffer);
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.GnNonSecuredPacket.extendedHeader") == 0) {
      // Decode ExtendedHeader data structure
      LibItsGeoNetworking__TypesAndValues::ExtendedHeader e;
      decode_extendedHeader(e, decoding_buffer);
      if (e.is_bound()) {
        dynamic_cast<OPTIONAL<LibItsGeoNetworking__TypesAndValues::ExtendedHeader> &>(type) = OPTIONAL<LibItsGeoNetworking__TypesAndValues::ExtendedHeader>(e);
        // loggers::get_instance().log_msg("geonetworking_codec::decode_: Set OPTIONAL to ", type);
      } else {
        // loggers::get_instance().log("geonetworking_codec::decode_: Set OPTIONAL to omit");
        dynamic_cast<OPTIONAL<LibItsGeoNetworking__TypesAndValues::ExtendedHeader> &>(type).set_to_omit();
      }
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.GnNonSecuredPacket.payload") == 0) {
      // Decode raw payload data structure
      OCTETSTRING s(decoding_buffer.get_len() - decoding_buffer.get_pos(), decoding_buffer.get_data() + decoding_buffer.get_pos());
      // loggers::get_instance().log_msg("geonetworking_codec::decode_: s=", s);
      const unsigned char *p = static_cast<const unsigned char *>(s);
      if (p == NULL) {
        loggers::get_instance().warning("geonetworking_codec::encode_: Static cast error");
        return -1;
      }
      // Align the payload length with the specified plLenght value
      // loggers::get_instance().log("geonetworking_codec::decode_: Payload: Decoded payload length: %d", _dc.get_length());
      OCTETSTRING os;
      if (_dc.get_length() != 0) {
        if ((unsigned char)s.lengthof() <= _dc.get_length()) {
          os                             = OCTETSTRING(s.lengthof(), p);
          (*_params)[params::gn_payload] = static_cast<const char *>(oct2str(os));
        } else {
          os                             = OCTETSTRING(_dc.get_length(), p);
          (*_params)[params::gn_payload] = static_cast<const char *>(oct2str(os));
        }
      } else {
        os                             = OCTETSTRING(0, nullptr);
        (*_params)[params::gn_payload] = "";
      }
      // loggers::get_instance().log_msg("geonetworking_codec::decode_: Payload: os: ", os);
      // loggers::get_instance().log("geonetworking_codec::decode_: Payload: '%s'", (*_params)[params::gn_payload].c_str());
      if (os.lengthof() != 0) {
        dynamic_cast<OPTIONAL<LibItsGeoNetworking__TypesAndValues::GnRawPayload> &>(type) = OPTIONAL<LibItsGeoNetworking__TypesAndValues::GnRawPayload>(os);
        // loggers::get_instance().log_msg("geonetworking_codec::decode_: Set OPTIONAL<GnRawPayload> to ", type);
      } else {
        // loggers::get_instance().log("geonetworking_codec::decode_: Set OPTIONAL<GnRawPayload> to omit");
        dynamic_cast<OPTIONAL<LibItsGeoNetworking__TypesAndValues::GnRawPayload> &>(type).set_to_omit();
      }
    } else if (std::string(field_descriptor.name).compare("@LibItsGeoNetworking_TypesAndValues.CommonHeader.plLength") == 0) {
      // Decode plLength data structure
      type.decode(field_descriptor, decoding_buffer, TTCN_EncDec::CT_RAW);
      // Store playload length and continue decoding
      _dc.set_length((unsigned int)static_cast<int>(dynamic_cast<const INTEGER &>(type)));
    } else {
      type.decode(field_descriptor, decoding_buffer, TTCN_EncDec::CT_RAW);
    }
  }

  // loggers::get_instance().log_msg("<<< geonetworking_codec::decode_: ", type);
  // loggers::get_instance().log_to_hexa("<<< geonetworking_codec::decode_: decoding_buffer=", decoding_buffer);

  return 0;
}

int geonetworking_codec::encode_extendedHeader(const LibItsGeoNetworking__TypesAndValues::ExtendedHeader &u, TTCN_Buffer &encoding_buffer) {
  // loggers::get_instance().log(">>> geonetworking_codec::encode_extendedHeader: processing %s", u.get_descriptor()->name);

  int result = -1;
  switch (u.get_selection()) {
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::UNBOUND_VALUE:
      TTCN_error("geonetworking_codec::encode_extendedHeader: Invalid selection in unbound union");
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_geoUnicastHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.geoUnicastHeader().get_descriptor()->name);
      result = encode_(u.geoUnicastHeader(), *u.geoUnicastHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_tsbHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.tsbHeader().get_descriptor()->name);
      result = encode_(u.tsbHeader(), *u.tsbHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_shbHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.shbHeader().get_descriptor()->name);
      result = encode_(u.shbHeader(), *u.shbHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_geoBroadcastHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.geoBroadcastHeader().get_descriptor()->name);
      result = encode_(u.geoBroadcastHeader(), *u.geoBroadcastHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_geoAnycastHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.geoAnycastHeader().get_descriptor()->name);
      result = encode_(u.geoAnycastHeader(), *u.geoAnycastHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_beaconHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.beaconHeader().get_descriptor()->name);
      result = encode_(u.beaconHeader(), *u.beaconHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_anyHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.anyHeader().get_descriptor()->name);
      result = encode_(u.anyHeader(), *u.anyHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_lsRequestHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.lsRequestHeader().get_descriptor()->name);
      result = encode_(u.lsRequestHeader(), *u.lsRequestHeader().get_descriptor(), encoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_lsReplyHeader:
      // loggers::get_instance().log("geonetworking_codec::encode_extendedHeader: processing type %s", u.lsReplyHeader().get_descriptor()->name);
      result = encode_(u.lsReplyHeader(), *u.lsReplyHeader().get_descriptor(), encoding_buffer);
      break;
    default:
      TTCN_error("geonetworking_codec::encode_extendedHeader: Invalid selection in union is_bound");
  } // End of 'switch' statement

  return result;
}

int geonetworking_codec::decode_headerTST(LibItsGeoNetworking__TypesAndValues::HeaderTST &u, TTCN_Buffer &decoding_buffer) {
  // loggers::get_instance().log(">>> geonetworking_codec::decode_headerTST: processing %s", u.get_descriptor()->name);

  // Decode HeaderTST data structure
  _dc.set_header_type(*(decoding_buffer.get_data() + decoding_buffer.get_pos()) >> 4);
  _dc.set_header_sub_type(*(decoding_buffer.get_data() + decoding_buffer.get_pos()) & 0x0F);
  // loggers::get_instance().log("geonetworking_codec::decode_: headerTST selectors: _dc.header_type = %d - _dc.header_sub_type = %d", _dc.get_header_type(),
  // _dc.get_header_sub_type()); LibItsGeoNetworking__TypesAndValues::HeaderTST & u = dynamic_cast<LibItsGeoNetworking__TypesAndValues::HeaderTST &>(u);
  switch (_dc.get_header_type()) {
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoUnicast:
      decode_(u.geoUnicastHdr(), *u.geoUnicastHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__topologicallyScopedBroadcast:
      decode_(u.tsbHdr(), *u.tsbHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoBroadcast:
      decode_(u.geoBroadcastHdr(), *u.geoBroadcastHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoAnycast:
      decode_(u.geoAnycastHdr(), *u.geoAnycastHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__beacon:
      decode_(u.beaconHdr(), *u.beaconHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__locationService:
      decode_(u.lsHdr(), *u.lsHdr().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__any:
      decode_(u.anyHdr(), *u.anyHdr().get_descriptor(), decoding_buffer);
      break;
    default:
      TTCN_error("geonetworking_codec::decode_: Not processed, _dc.header_type = %d - _dc.header_sub_type = %d", _dc.get_header_type(),
                 _dc.get_header_sub_type());
  } // End of 'switch' statement

  return 0;
}

int geonetworking_codec::decode_extendedHeader(LibItsGeoNetworking__TypesAndValues::ExtendedHeader &u, TTCN_Buffer &decoding_buffer) {
  // loggers::get_instance().log(">>> geonetworking_codec::decode_extendedHeader: processing %s", u.get_descriptor()->name);

  switch (_dc.get_header_type()) {
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoUnicast:
      decode_(u.geoUnicastHeader(), *u.geoUnicastHeader().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__topologicallyScopedBroadcast:
      if (_dc.get_header_sub_type() == LibItsGeoNetworking__TypesAndValues::HeaderSubTypeTSB::e__multiHop) {
        decode_(u.tsbHeader(), *u.tsbHeader().get_descriptor(), decoding_buffer);
      } else if (_dc.get_header_sub_type() == LibItsGeoNetworking__TypesAndValues::HeaderSubTypeTSB::e__singleHop) {
        decode_(u.shbHeader(), *u.shbHeader().get_descriptor(), decoding_buffer);
      } else {
        TTCN_error("geonetworking_codec::decode_headerTST: TSB subtype not processed, See ETSI EN 302 636-4-1 Clause 9.7.4 Encoding of the HT and HST fields");
      }
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoBroadcast:
      decode_(u.geoBroadcastHeader(), *u.geoBroadcastHeader().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__geoAnycast:
      decode_(u.geoAnycastHeader(), *u.geoAnycastHeader().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__beacon:
      decode_(u.beaconHeader(), *u.beaconHeader().get_descriptor(), decoding_buffer);
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__locationService:
      if (_dc.get_header_sub_type() == LibItsGeoNetworking__TypesAndValues::HeaderSubTypeLs::e__lsRequest) {
        decode_(u.lsRequestHeader(), *u.lsRequestHeader().get_descriptor(), decoding_buffer);
      } else if (_dc.get_header_sub_type() == LibItsGeoNetworking__TypesAndValues::HeaderSubTypeLs::e__lsReply) {
        decode_(u.lsReplyHeader(), *u.lsReplyHeader().get_descriptor(), decoding_buffer);
      } else {
        TTCN_error("geonetworking_codec::decode_headerTST: Location service subtype not processed, See ETSI EN 302 636-4-1 Clause 9.7.4 Encoding of the HT and "
                   "HST fields");
      }
      break;
    case LibItsGeoNetworking__TypesAndValues::HeaderType::e__any:
      decode_(u.anyHeader(), *u.anyHeader().get_descriptor(), decoding_buffer);
      break;
    default:
      TTCN_error("geonetworking_codec::decode_headerTST: Not processed, _dc.header_type = %d - _dc.header_sub_type = %d, See ETSI EN 302 636-4-1 Clause 9.7.4 "
                 "Encoding of the HT and HST fields",
                 _dc.get_header_type(), _dc.get_header_sub_type());
  } // End of 'switch' statement

  return 0;
}
