#include <memory>

#include "LibItsSremSsem_EncdecDeclarations.hh"

#include "SremCodec.hh"
#include "SsemCodec.hh"
#include "UpperTesterSremSsemCodec.hh"
#include "loggers.hh"

namespace LibItsSremSsem__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a SremReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__SremReq(const LibItsSremSsem__TypesAndValues::SremReq &p_sremReq) {
    loggers::get_instance().log_msg(">>> fx__enc__SremReq: ", p_sremReq);

    SremCodec   codec;
    OCTETSTRING os;
    if (codec.encode(p_sremReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__SremReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a SremReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__SremReq(BITSTRING &b, LibItsSremSsem__TypesAndValues::SremReq &p) {
    loggers::get_instance().log(">>> fx__dec__SremReq");

    SremCodec                     codec;
    SREM__PDU__Descriptions::SREM srem;
    OCTETSTRING                   is = bit2oct(b);

    if (codec.decode(is, srem) == -1) {
      loggers::get_instance().warning("fx__dec__SremReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = srem;

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a SremInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__SremInd(const LibItsSremSsem__TypesAndValues::SremInd &p_sremInd) {
    loggers::get_instance().log_msg(">>> fx__enc__SremInd: ", p_sremInd);

    SremCodec   codec;
    OCTETSTRING os;
    if (codec.encode(p_sremInd.msgIn(), os) == -1) {
      loggers::get_instance().warning("fx__enc__SremInd: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a SremInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__SremInd(BITSTRING &b, LibItsSremSsem__TypesAndValues::SremInd &p_sremInd) {
    loggers::get_instance().log(">>> fx__dec__SremInd");

    SremCodec                     codec;
    SREM__PDU__Descriptions::SREM srem;
    OCTETSTRING                   is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_btpInfo_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_ssp_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), srem) == -1) {
      loggers::get_instance().warning("fx__dec__SremInd: -1 result code was returned");
      return -1;
    }
    p_sremInd.msgIn() = srem;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_sremInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: gnNextHeader=", p_sremInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_sremInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: gnHeaderType=", p_sremInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_sremInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: gnHeaderSubtype=", p_sremInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_raw_.fieldlength / 8)) {
      p_sremInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: gnLifetime=", p_sremInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_sremInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: gnTrafficClass=", p_sremInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_sremInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: btpDestinationPort=", p_sremInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_btpInfo_raw_.fieldlength / 8)) {
      p_sremInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: btpInfo=", p_sremInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_ssp_raw_.fieldlength / 8)) {
      p_sremInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsSremSsem__TypesAndValues::SremInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: ssp=", p_sremInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_its__aid_raw_.fieldlength / 8)) {
      p_sremInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_sremInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SremInd: its_aid=", p_sremInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__SremInd: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__SREM(const SREM__PDU__Descriptions::SREM &p_srem) {
    loggers::get_instance().log_msg(">>> fx__enc__SREM: ", p_srem);

    SremCodec   codec;
    OCTETSTRING os;
    codec.encode(p_srem, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__SREM(BITSTRING &b, SREM__PDU__Descriptions::SREM &p) {
    loggers::get_instance().log(">>> fx__dec__SREM");
    SremCodec   codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__Srem: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a SsemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__SsemReq(const LibItsSremSsem__TypesAndValues::SsemReq &p_ssemReq) {
    loggers::get_instance().log_msg(">>> fx__enc__SsemReq: ", p_ssemReq);

    SsemCodec   codec;
    OCTETSTRING os;
    if (codec.encode(p_ssemReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__SsemReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a SsemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__SsemReq(BITSTRING &b, LibItsSremSsem__TypesAndValues::SsemReq &p) {
    loggers::get_instance().log(">>> fx__dec__SsemReq");

    SsemCodec                     codec;
    SSEM__PDU__Descriptions::SSEM ssem;
    OCTETSTRING                   is = bit2oct(b);

    if (codec.decode(is, ssem) == -1) {
      loggers::get_instance().warning("fx__dec__SsemReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = ssem;

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a SsemInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__SsemInd(const LibItsSremSsem__TypesAndValues::SsemInd &p_ssemInd) {
    loggers::get_instance().log_msg(">>> fx__enc__SsemInd: ", p_ssemInd);

    SsemCodec   codec;
    OCTETSTRING os;
    if (codec.encode(p_ssemInd.msgIn(), os) == -1) {
      loggers::get_instance().warning("fx__enc__SsemInd: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a SsemInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__SsemInd(BITSTRING &b, LibItsSremSsem__TypesAndValues::SsemInd &p_ssemInd) {
    loggers::get_instance().log(">>> fx__dec__SsemInd");

    SsemCodec                     codec;
    SSEM__PDU__Descriptions::SSEM ssem;
    OCTETSTRING                   is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_btpInfo_raw_.fieldlength + LibItsSremSsem__TypesAndValues::SremInd_ssp_raw_.fieldlength +
             LibItsSremSsem__TypesAndValues::SremInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), ssem) == -1) {
      loggers::get_instance().warning("fx__dec__SsemSsemInd: -1 result code was returned");
      return -1;
    }
    p_ssemInd.msgIn() = ssem;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_ssemInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: gnNextHeader=", p_ssemInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_ssemInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: gnHeaderType=", p_ssemInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_ssemInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: gnHeaderSubtype=", p_ssemInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_raw_.fieldlength / 8)) {
      p_ssemInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: gnLifetime=", p_ssemInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_ssemInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsSremSsem__TypesAndValues::SremInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: gnTrafficClass=", p_ssemInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_ssemInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: btpDestinationPort=", p_ssemInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_btpInfo_raw_.fieldlength / 8)) {
      p_ssemInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: btpInfo=", p_ssemInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_ssp_raw_.fieldlength / 8)) {
      p_ssemInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsSremSsem__TypesAndValues::SremInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: ssp=", p_ssemInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsSremSsem__TypesAndValues::SremInd_its__aid_raw_.fieldlength / 8)) {
      p_ssemInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsSremSsem__TypesAndValues::SremInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ssemInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__SsemInd: its_aid=", p_ssemInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__SsemInd: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__SSEM(const SSEM__PDU__Descriptions::SSEM &p_ssem) {
    loggers::get_instance().log_msg(">>> fx__enc__SSEM: ", p_ssem);

    SsemCodec   codec;
    OCTETSTRING os;
    codec.encode(p_ssem, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__SSEM(BITSTRING &b, SSEM__PDU__Descriptions::SSEM &p_ssem) {
    loggers::get_instance().log(">>> fx__dec__SSEM");
    SsemCodec   codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p_ssem) == -1) {
      loggers::get_instance().warning("fx__dec__Ssem: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtSremInitialize(const LibItsSremSsem__TypesAndValues::UtSremInitialize &p_utSremInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtSremInitialize: ", p_utSremInitialize);

    UpperTesterSremSsemCodec codec;
    OCTETSTRING              os;
    codec.encode(p_utSremInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtSremTrigger(const LibItsSremSsem__TypesAndValues::UtSremTrigger &p_utSremTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtSremTrigger: ", p_utSremTrigger);

    UpperTesterSremSsemCodec codec;
    OCTETSTRING              os;
    codec.encode(p_utSremTrigger, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a UtSremResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtSremResults(BITSTRING &b, LibItsSremSsem__TypesAndValues::UtSremResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtSremResults");

    UpperTesterSremSsemCodec   codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtSremResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsSremSsem__TypesAndValues::UtSremResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtSremEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtSremEventInd(BITSTRING &b, LibItsSremSsem__TypesAndValues::UtSremEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtSremEventInd");

    UpperTesterSremSsemCodec   codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtSremEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsSremSsem__TypesAndValues::UtSremEventInd &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtSsemEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtSsemEventInd(BITSTRING &b, LibItsSremSsem__TypesAndValues::UtSsemEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtSsemEventInd");

    UpperTesterSremSsemCodec   codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtSsemEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsSremSsem__TypesAndValues::UtSsemEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsSremSsem__EncdecDeclarations
