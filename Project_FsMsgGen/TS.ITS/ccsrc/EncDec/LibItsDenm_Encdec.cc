#include <memory>

#include "LibItsDenm_EncdecDeclarations.hh"

#include "denm_codec.hh"
#include "loggers.hh"
#include "uppertester_denm_codec.hh"

namespace LibItsDenm__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a DenmReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__DenmReq(const LibItsDenm__TypesAndValues::DenmReq &p_denmReq) {
    loggers::get_instance().log_msg(">>> fx__enc__DenmReq: ", p_denmReq);

    denm_codec  codec;
    OCTETSTRING os;
    if (codec.encode(p_denmReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__DenmReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a DenmReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__DenmReq(BITSTRING &b, LibItsDenm__TypesAndValues::DenmReq &p) {
    loggers::get_instance().log(">>> fx__dec__DenmReq");

    denm_codec                    codec;
    DENM__PDU__Descriptions::DENM denm;
    OCTETSTRING                   is = bit2oct(b);

    if (codec.decode(is, denm) == -1) {
      loggers::get_instance().warning("fx__dec__DenmReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = denm;

    return 0;
  }

  INTEGER fx__dec__DenmInd(BITSTRING &b, LibItsDenm__TypesAndValues::DenmInd &p_denmInd) {
    loggers::get_instance().log(">>> fx__dec__DenmInd");

    denm_codec                    codec;
    DENM__PDU__Descriptions::DENM denm;
    OCTETSTRING                   is = bit2oct(b);

    int s = (LibItsDenm__TypesAndValues::DenmInd_gnNextHeader_raw_.fieldlength + LibItsDenm__TypesAndValues::DenmInd_gnHeaderType_raw_.fieldlength +
             LibItsDenm__TypesAndValues::DenmInd_gnHeaderSubtype_raw_.fieldlength + LibItsDenm__TypesAndValues::DenmInd_gnLifetime_raw_.fieldlength +
             LibItsDenm__TypesAndValues::DenmInd_gnTrafficClass_raw_.fieldlength + LibItsDenm__TypesAndValues::DenmInd_btpDestinationPort_raw_.fieldlength +
             LibItsDenm__TypesAndValues::DenmInd_btpInfo_raw_.fieldlength + LibItsDenm__TypesAndValues::DenmInd_ssp_raw_.fieldlength +
             LibItsDenm__TypesAndValues::DenmInd_its__aid_raw_.fieldlength) /
            8;
    // Decode DEN message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), denm) == -1) {
      loggers::get_instance().warning("fx__dec__DenmInd: -1 result code was returned");
      return -1;
    }
    p_denmInd.msgIn() = denm;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_denmInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: gnNextHeader=", p_denmInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_denmInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: gnHeaderType=", p_denmInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_denmInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: gnHeaderSubType=", p_denmInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_gnLifetime_raw_.fieldlength / 8)) {
      p_denmInd.gnLifetime().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.gnLifetime() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: gnLifetime=", p_denmInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_denmInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: gnTrafficClass=", p_denmInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_denmInd.btpDestinationPort().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.btpDestinationPort() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: btpDestinationPort=", p_denmInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_btpInfo_raw_.fieldlength / 8)) {
      p_denmInd.btpInfo().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.btpInfo() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: btpInfo=", p_denmInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_ssp_raw_.fieldlength / 8)) {
      p_denmInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsDenm__TypesAndValues::DenmInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__DENM: ssp=", p_denmInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsDenm__TypesAndValues::DenmInd_its__aid_raw_.fieldlength / 8)) {
      p_denmInd.its__aid().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsDenm__TypesAndValues::DenmInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_denmInd.its__aid() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__DENM: its_aid=", p_denmInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__DENM: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__DENM(const DENM__PDU__Descriptions::DENM &p_denm) {
    loggers::get_instance().log_msg(">>> fx__enc__DENM: ", p_denm);

    denm_codec  codec;
    OCTETSTRING os;
    codec.encode(p_denm, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__DENM(BITSTRING &b, DENM__PDU__Descriptions::DENM &p) {
    loggers::get_instance().log(">>> fx__dec__DENM");
    denm_codec  codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__DENM: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtDenmInitialize(const LibItsDenm__TypesAndValues::UtDenmInitialize &p_utDenmInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmInitialize: ", p_utDenmInitialize);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utDenmInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtDenmChangePosition(const LibItsDenm__TypesAndValues::UtDenmChangePosition &p_utDenmChangePosition) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmChangePosition: ", p_utDenmChangePosition);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utDenmChangePosition, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtDenmChangePseudonym(const LibItsDenm__TypesAndValues::UtDenmChangePseudonym &p_utDenmChangePseudonym) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmChangePseudonym: ", p_utDenmChangePseudonym);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode((const Record_Type &)p_utDenmChangePseudonym, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtDenmTrigger(const LibItsDenm__TypesAndValues::UtDenmTrigger &p_utDenmTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmTrigger: ", p_utDenmTrigger);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utDenmTrigger, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtDenmUpdate(const LibItsDenm__TypesAndValues::UtDenmUpdate &p_utDenmUpdate) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmUpdate: ", p_utDenmUpdate);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utDenmUpdate, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtDenmTermination(const LibItsDenm__TypesAndValues::UtDenmTermination &p_utDenmTermination) {
    loggers::get_instance().log_msg(">>> fx__enc__UtDenmTermination: ", p_utDenmTermination);

    uppertester_denm_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utDenmTermination, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a UtDenmResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtDenmResults(BITSTRING &b, LibItsDenm__TypesAndValues::UtDenmResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtDenmResults");

    uppertester_denm_codec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtDenmResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsDenm__TypesAndValues::UtDenmResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtDenmEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtDenmEventInd(BITSTRING &b, LibItsDenm__TypesAndValues::UtDenmEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtDenmEventInd");

    uppertester_denm_codec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtDenmEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsDenm__TypesAndValues::UtDenmEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsDenm__EncdecDeclarations
