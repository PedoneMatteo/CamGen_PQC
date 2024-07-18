#include <memory>

#include "LibItsCam_EncdecDeclarations.hh"

#include "cam_codec.hh"
#include "loggers.hh"
#include "uppertester_cam_codec.hh"

namespace LibItsCam__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a CamReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__CamReq(const LibItsCam__TypesAndValues::CamReq &p_camReq) {
    loggers::get_instance().log_msg(">>> fx__enc__CamReq: ", p_camReq);

    cam_codec   codec;
    OCTETSTRING os;
    if (codec.encode(p_camReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__CamReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a CamReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__CamReq(BITSTRING &b, LibItsCam__TypesAndValues::CamReq &p) {
    loggers::get_instance().log(">>> fx__dec__CamReq");

    cam_codec                   codec;
    CAM__PDU__Descriptions::CAM cam;
    OCTETSTRING                 is = bit2oct(b);

    if (codec.decode(is, cam) == -1) {
      loggers::get_instance().warning("fx__dec__CamReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = cam;

    return 0;
  }

  INTEGER fx__dec__CamInd(BITSTRING &b, LibItsCam__TypesAndValues::CamInd &p_camInd) {
    loggers::get_instance().log(">>> fx__dec__CamInd");

    cam_codec                   codec;
    CAM__PDU__Descriptions::CAM cam;
    OCTETSTRING                 is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsCam__TypesAndValues::CamInd_gnNextHeader_raw_.fieldlength + LibItsCam__TypesAndValues::CamInd_gnHeaderType_raw_.fieldlength +
             LibItsCam__TypesAndValues::CamInd_gnHeaderSubtype_raw_.fieldlength + LibItsCam__TypesAndValues::CamInd_gnLifetime_raw_.fieldlength +
             LibItsCam__TypesAndValues::CamInd_gnTrafficClass_raw_.fieldlength + LibItsCam__TypesAndValues::CamInd_btpDestinationPort_raw_.fieldlength +
             LibItsCam__TypesAndValues::CamInd_btpInfo_raw_.fieldlength + LibItsCam__TypesAndValues::CamInd_ssp_raw_.fieldlength +
             LibItsCam__TypesAndValues::CamInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), cam) == -1) {
      loggers::get_instance().warning("fx__dec__CamInd: -1 result code was returned");
      return -1;
    }
    p_camInd.msgIn() = cam;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_camInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsCam__TypesAndValues::CamInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__CAM: gnNextHeader=", p_camInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_camInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsCam__TypesAndValues::CamInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__CAM: gnHeaderType=", p_camInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_camInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsCam__TypesAndValues::CamInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__CAM: gnHeaderSubtype=", p_camInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_gnLifetime_raw_.fieldlength / 8)) {
      p_camInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsCam__TypesAndValues::CamInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__CAM: gnLifetime=", p_camInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_camInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsCam__TypesAndValues::CamInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__CAM: gnTrafficClass=", p_camInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_camInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsCam__TypesAndValues::CamInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__CAM: btpDestinationPort=", p_camInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_btpInfo_raw_.fieldlength / 8)) {
      p_camInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsCam__TypesAndValues::CamInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__CAM: btpInfo=", p_camInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_ssp_raw_.fieldlength / 8)) {
      p_camInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsCam__TypesAndValues::CamInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__CAM: ssp=", p_camInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsCam__TypesAndValues::CamInd_its__aid_raw_.fieldlength / 8)) {
      p_camInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsCam__TypesAndValues::CamInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_camInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__CAM: its_aid=", p_camInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__CAM: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__CAM(const CAM__PDU__Descriptions::CAM &p_cam) {
    loggers::get_instance().log_msg(">>> fx__enc__CAM: ", p_cam);

    cam_codec   codec;
    OCTETSTRING os;
    codec.encode(p_cam, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__CAM(BITSTRING &b, CAM__PDU__Descriptions::CAM &p) {
    loggers::get_instance().log(">>> fx__dec__CAM");
    cam_codec   codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__CAM: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtCamInitialize(const LibItsCam__TypesAndValues::UtCamInitialize &p_utCamInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtCamInitialize: ", p_utCamInitialize);

    uppertester_cam_codec codec;
    OCTETSTRING           os;
    codec.encode(p_utCamInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtCamChangePosition(const LibItsCam__TypesAndValues::UtCamChangePosition &p_utCamChangePosition) {
    loggers::get_instance().log_msg(">>> fx__enc__UtCamChangePosition: ", p_utCamChangePosition);

    uppertester_cam_codec codec;
    OCTETSTRING           os;
    codec.encode(p_utCamChangePosition, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtCamTrigger(const LibItsCam__TypesAndValues::UtCamTrigger &p_utCamTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtCamTrigger: ", p_utCamTrigger);

    uppertester_cam_codec codec;
    OCTETSTRING           os;
    codec.encode(p_utCamTrigger, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a UtCamResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtCamResults(BITSTRING &b, LibItsCam__TypesAndValues::UtCamResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtCamResults");

    uppertester_cam_codec      codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtCamResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsCam__TypesAndValues::UtCamResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtCamEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtCamEventInd(BITSTRING &b, LibItsCam__TypesAndValues::UtCamEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtCamEventInd");

    uppertester_cam_codec      codec;
    OCTETSTRING                is  = bit2oct(b);
    std::unique_ptr<Base_Type> ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtCamEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsCam__TypesAndValues::UtCamEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsCam__EncdecDeclarations
