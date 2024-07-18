#include <memory>

#include "LibItsRtcmem_EncdecDeclarations.hh"

#include "RtcmemCodec.hh"
#include "UpperTesterRtcmemCodec.hh"
#include "loggers.hh"

namespace LibItsRtcmem__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a RtcmemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__RtcmemReq(const LibItsRtcmem__TypesAndValues::RtcmemReq &p_rtcmemReq) {
    loggers::get_instance().log_msg(">>> fx__enc__RtcmemReq: ", p_rtcmemReq);

    RtcmemCodec codec;
    OCTETSTRING os;
    if (codec.encode(p_rtcmemReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__RtcmemReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a DenmReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__RtcmemReq(BITSTRING &b, LibItsRtcmem__TypesAndValues::RtcmemReq &p) {
    loggers::get_instance().log(">>> fx__dec__RtcmemReq");

    RtcmemCodec                       codec;
    RTCMEM__PDU__Descriptions::RTCMEM rtcmem;
    OCTETSTRING                       is = bit2oct(b);

    if (codec.decode(is, rtcmem) == -1) {
      loggers::get_instance().warning("fx__dec__RtcmemReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = rtcmem;

    return 0;
  }

  INTEGER fx__dec__RtcmemInd(BITSTRING &b, LibItsRtcmem__TypesAndValues::RtcmemInd &p_rtcmemInd) {
    loggers::get_instance().log(">>> fx__dec__RtcmemInd");

    RtcmemCodec                       codec;
    RTCMEM__PDU__Descriptions::RTCMEM rtcmem;
    OCTETSTRING                       is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsRtcmem__TypesAndValues::RtcmemInd_gnNextHeader_raw_.fieldlength + LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderType_raw_.fieldlength +
             LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderSubtype_raw_.fieldlength + LibItsRtcmem__TypesAndValues::RtcmemInd_gnLifetime_raw_.fieldlength +
             LibItsRtcmem__TypesAndValues::RtcmemInd_gnTrafficClass_raw_.fieldlength + LibItsRtcmem__TypesAndValues::RtcmemInd_btpDestinationPort_raw_.fieldlength +
             LibItsRtcmem__TypesAndValues::RtcmemInd_btpInfo_raw_.fieldlength + LibItsRtcmem__TypesAndValues::RtcmemInd_ssp_raw_.fieldlength +
             LibItsRtcmem__TypesAndValues::RtcmemInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), rtcmem) == -1) {
      loggers::get_instance().warning("fx__dec__RtcmemInd: -1 result code was returned");
      return -1;
    }
    p_rtcmemInd.msgIn() = rtcmem;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_rtcmemInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: gnNextHeader=", p_rtcmemInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_rtcmemInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: gnHeaderType=", p_rtcmemInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_rtcmemInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: gnHeaderSubtype=", p_rtcmemInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_gnLifetime_raw_.fieldlength / 8)) {
      p_rtcmemInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: gnLifetime=", p_rtcmemInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_rtcmemInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: gnTrafficClass=", p_rtcmemInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_rtcmemInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: btpDestinationPort=", p_rtcmemInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_btpInfo_raw_.fieldlength / 8)) {
      p_rtcmemInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: btpInfo=", p_rtcmemInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_ssp_raw_.fieldlength / 8)) {
      p_rtcmemInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: ssp=", p_rtcmemInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsRtcmem__TypesAndValues::RtcmemInd_its__aid_raw_.fieldlength / 8)) {
      p_rtcmemInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsRtcmem__TypesAndValues::RtcmemInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_rtcmemInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__RTCMEM: its_aid=", p_rtcmemInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__RTCMEM: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__RTCMEM(const RTCMEM__PDU__Descriptions::RTCMEM &p_rtcmem) {
    loggers::get_instance().log_msg(">>> fx__enc__RTCMEM: ", p_rtcmem);

    RtcmemCodec codec;
    OCTETSTRING os;
    codec.encode(p_rtcmem, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__RTCMEM(BITSTRING &b, RTCMEM__PDU__Descriptions::RTCMEM &p) {
    loggers::get_instance().log(">>> fx__dec__RTCMEM");
    RtcmemCodec codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__RTCMEM: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtRtcmemInitialize(const LibItsRtcmem__TypesAndValues::UtRtcmemInitialize &p_utRtcmemInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtRtcmemInitialize: ", p_utRtcmemInitialize);

    UpperTesterRtcmemCodec codec;
    OCTETSTRING            os;
    codec.encode(p_utRtcmemInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtRtcmemTrigger(const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &p_utRtcmemTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtRtcmemTrigger: ", p_utRtcmemTrigger);

    UpperTesterRtcmemCodec codec;
    OCTETSTRING            os;
    codec.encode(p_utRtcmemTrigger, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtRtcmemUpdate(const LibItsRtcmem__TypesAndValues::UtRtcmemUpdate &p_utRtcmemUpdate) {
    loggers::get_instance().log_msg(">>> fx__enc__UtRtcmemUpdate: ", p_utRtcmemUpdate);

    UpperTesterRtcmemCodec codec;
    OCTETSTRING            os;
    codec.encode(p_utRtcmemUpdate, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtRtcmemTermination(const LibItsRtcmem__TypesAndValues::UtRtcmemTermination &p_utRtcmemTermination) {
    loggers::get_instance().log_msg(">>> fx__enc__UtRtcmemTermination: ", p_utRtcmemTermination);

    UpperTesterRtcmemCodec codec;
    OCTETSTRING            os;
    codec.encode(p_utRtcmemTermination, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a UtRtcmemResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtRtcmemResults(BITSTRING &b, LibItsRtcmem__TypesAndValues::UtRtcmemResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtRtcmemResults");

    UpperTesterRtcmemCodec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtRtcmemResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsRtcmem__TypesAndValues::UtRtcmemResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtRtcmemEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtRtcmemEventInd(BITSTRING &b, LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtRtcmemEventInd");

    UpperTesterRtcmemCodec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtRtcmemEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsRtcmem__EncdecDeclarations
