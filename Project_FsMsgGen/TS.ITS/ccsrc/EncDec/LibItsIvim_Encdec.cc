#include <memory>

#include "LibItsIvim_EncdecDeclarations.hh"

#include "ivim_codec.hh"
#include "loggers.hh"
#include "uppertester_ivim_codec.hh"

namespace LibItsIvim__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a IvimReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__IvimReq(const LibItsIvim__TypesAndValues::IvimReq &p_ivimReq) {
    loggers::get_instance().log_msg(">>> fx__enc__IvimReq: ", p_ivimReq);

    ivim_codec  codec;
    OCTETSTRING os;
    if (codec.encode(p_ivimReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__IvimReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a DenmReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__IvimReq(BITSTRING &b, LibItsIvim__TypesAndValues::IvimReq &p) {
    loggers::get_instance().log(">>> fx__dec__IvimReq");

    ivim_codec                    codec;
    IVIM__PDU__Descriptions::IVIM ivim;
    OCTETSTRING                   is = bit2oct(b);

    if (codec.decode(is, ivim) == -1) {
      loggers::get_instance().warning("fx__dec__IvimReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = ivim;

    return 0;
  }

  INTEGER fx__dec__IvimInd(BITSTRING &b, LibItsIvim__TypesAndValues::IvimInd &p_ivimInd) {
    loggers::get_instance().log(">>> fx__dec__IvimInd");

    ivim_codec                    codec;
    IVIM__PDU__Descriptions::IVIM ivim;
    OCTETSTRING                   is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsIvim__TypesAndValues::IvimInd_gnNextHeader_raw_.fieldlength + LibItsIvim__TypesAndValues::IvimInd_gnHeaderType_raw_.fieldlength +
             LibItsIvim__TypesAndValues::IvimInd_gnHeaderSubtype_raw_.fieldlength + LibItsIvim__TypesAndValues::IvimInd_gnLifetime_raw_.fieldlength +
             LibItsIvim__TypesAndValues::IvimInd_gnTrafficClass_raw_.fieldlength + LibItsIvim__TypesAndValues::IvimInd_btpDestinationPort_raw_.fieldlength +
             LibItsIvim__TypesAndValues::IvimInd_btpInfo_raw_.fieldlength + LibItsIvim__TypesAndValues::IvimInd_ssp_raw_.fieldlength +
             LibItsIvim__TypesAndValues::IvimInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), ivim) == -1) {
      loggers::get_instance().warning("fx__dec__IvimInd: -1 result code was returned");
      return -1;
    }
    p_ivimInd.msgIn() = ivim;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_ivimInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsIvim__TypesAndValues::IvimInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: gnNextHeader=", p_ivimInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_ivimInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsIvim__TypesAndValues::IvimInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: gnHeaderType=", p_ivimInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_ivimInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsIvim__TypesAndValues::IvimInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: gnHeaderSubtype=", p_ivimInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_gnLifetime_raw_.fieldlength / 8)) {
      p_ivimInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsIvim__TypesAndValues::IvimInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: gnLifetime=", p_ivimInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_ivimInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsIvim__TypesAndValues::IvimInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: gnTrafficClass=", p_ivimInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_ivimInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsIvim__TypesAndValues::IvimInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: btpDestinationPort=", p_ivimInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_btpInfo_raw_.fieldlength / 8)) {
      p_ivimInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsIvim__TypesAndValues::IvimInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: btpInfo=", p_ivimInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_ssp_raw_.fieldlength / 8)) {
      p_ivimInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsIvim__TypesAndValues::IvimInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: ssp=", p_ivimInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsIvim__TypesAndValues::IvimInd_its__aid_raw_.fieldlength / 8)) {
      p_ivimInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsIvim__TypesAndValues::IvimInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_ivimInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__IVIM: its_aid=", p_ivimInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__IVIM: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__IVIM(const IVIM__PDU__Descriptions::IVIM &p_ivim) {
    loggers::get_instance().log_msg(">>> fx__enc__IVIM: ", p_ivim);

    ivim_codec  codec;
    OCTETSTRING os;
    codec.encode(p_ivim, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__IVIM(BITSTRING &b, IVIM__PDU__Descriptions::IVIM &p) {
    loggers::get_instance().log(">>> fx__dec__IVIM");
    ivim_codec  codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__IVIM: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtIvimInitialize(const LibItsIvim__TypesAndValues::UtIvimInitialize &p_utIvimInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtIvimInitialize: ", p_utIvimInitialize);

    uppertester_ivim_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utIvimInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtIvimTrigger(const LibItsIvim__TypesAndValues::UtIvimTrigger &p_utIvimTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtIvimTrigger: ", p_utIvimTrigger);

    uppertester_ivim_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utIvimTrigger, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtIvimUpdate(const LibItsIvim__TypesAndValues::UtIvimUpdate &p_utIvimUpdate) {
    loggers::get_instance().log_msg(">>> fx__enc__UtIvimUpdate: ", p_utIvimUpdate);

    uppertester_ivim_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utIvimUpdate, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtIvimTermination(const LibItsIvim__TypesAndValues::UtIvimTermination &p_utIvimTermination) {
    loggers::get_instance().log_msg(">>> fx__enc__UtIvimTermination: ", p_utIvimTermination);

    uppertester_ivim_codec codec;
    OCTETSTRING            os;
    codec.encode(p_utIvimTermination, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a UtIvimResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtIvimResults(BITSTRING &b, LibItsIvim__TypesAndValues::UtIvimResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtIvimResults");

    uppertester_ivim_codec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtIvimResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsIvim__TypesAndValues::UtIvimResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtIvimEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtIvimEventInd(BITSTRING &b, LibItsIvim__TypesAndValues::UtIvimEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtIvimEventInd");

    uppertester_ivim_codec     codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtIvimEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsIvim__TypesAndValues::UtIvimEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsIvim__EncdecDeclarations
