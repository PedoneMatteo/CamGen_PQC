#include <memory>

#include "LibItsMapemSpatem_EncdecDeclarations.hh"

#include "loggers.hh"
#include "mapem_codec.hh"
#include "spatem_codec.hh"
#include "uppertester_mapem_spatem_codec.hh"

namespace LibItsMapemSpatem__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a MapemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__MapemReq(const LibItsMapemSpatem__TypesAndValues::MapemReq &p_sremReq) {
    loggers::get_instance().log_msg(">>> fx__enc__MapemReq: ", p_sremReq);

    mapem_codec codec;
    OCTETSTRING os;
    if (codec.encode(p_sremReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__MapemReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a MapemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__MapemReq(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::MapemReq &p) {
    loggers::get_instance().log(">>> fx__dec__MapemReq");

    mapem_codec                     codec;
    MAPEM__PDU__Descriptions::MAPEM mapem;
    OCTETSTRING                     is = bit2oct(b);

    if (codec.decode(is, mapem) == -1) {
      loggers::get_instance().warning("fx__dec__MapemReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = mapem;

    return 0;
  }

  INTEGER fx__dec__MapemInd(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::MapemInd &p_mapemInd) {
    loggers::get_instance().log(">>> fx__dec__MapemInd");

    mapem_codec                     codec;
    MAPEM__PDU__Descriptions::MAPEM mapem;
    OCTETSTRING                     is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsMapemSpatem__TypesAndValues::MapemInd_gnNextHeader_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderType_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderSubtype_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::MapemInd_gnLifetime_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::MapemInd_gnTrafficClass_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::MapemInd_btpDestinationPort_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::MapemInd_btpInfo_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::MapemInd_ssp_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::MapemInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), mapem) == -1) {
      loggers::get_instance().warning("fx__dec__MapemInd: -1 result code was returned");
      return -1;
    }
    p_mapemInd.msgIn() = mapem;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__MapemInd: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_mapemInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__MapemInd: gnNextHeader=", p_mapemInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__MapemInd: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_mapemInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: gnHeaderType=", p_mapemInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_mapemInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: gnHeaderSubtype=", p_mapemInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_gnLifetime_raw_.fieldlength / 8)) {
      p_mapemInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: gnLifetime=", p_mapemInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_mapemInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: gnTrafficClass=", p_mapemInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_mapemInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: btpDestinationPort=", p_mapemInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_btpInfo_raw_.fieldlength / 8)) {
      p_mapemInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: btpInfo=", p_mapemInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_ssp_raw_.fieldlength / 8)) {
      p_mapemInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: ssp=", p_mapemInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::MapemInd_its__aid_raw_.fieldlength / 8)) {
      p_mapemInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::MapemInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Mapem: its_aid=", p_mapemInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__Mapem: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__MAPEM(const MAPEM__PDU__Descriptions::MAPEM &p_mapem) {
    loggers::get_instance().log_msg(">>> fx__enc__Mapem: ", p_mapem);

    mapem_codec codec;
    OCTETSTRING os;
    codec.encode(p_mapem, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__MAPEM(BITSTRING &b, MAPEM__PDU__Descriptions::MAPEM &p) {
    loggers::get_instance().log(">>> fx__dec__Mapem");
    mapem_codec codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__Mapem: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a SpatemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__SpatemReq(const LibItsMapemSpatem__TypesAndValues::SpatemReq &p_spatemReq) {
    loggers::get_instance().log_msg(">>> fx__enc__SpatemReq: ", p_spatemReq);

    spatem_codec codec;
    OCTETSTRING  os;
    if (codec.encode(p_spatemReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__SpatemReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a SpatemReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__SpatemReq(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::SpatemReq &p) {
    loggers::get_instance().log(">>> fx__dec__SpatemReq");

    spatem_codec                      codec;
    SPATEM__PDU__Descriptions::SPATEM spatem;
    OCTETSTRING                       is = bit2oct(b);

    if (codec.decode(is, spatem) == -1) {
      loggers::get_instance().warning("fx__dec__SpatemReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = spatem;

    return 0;
  }

  INTEGER fx__dec__SpatemInd(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::SpatemInd &p_mapemInd) {
    loggers::get_instance().log(">>> fx__dec__SpatemInd");

    spatem_codec                      codec;
    SPATEM__PDU__Descriptions::SPATEM spatem;
    OCTETSTRING                       is = bit2oct(b);

    // Calculate the size of the lower layers information
    int s = (LibItsMapemSpatem__TypesAndValues::SpatemInd_gnNextHeader_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderType_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderSubtype_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::SpatemInd_gnLifetime_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::SpatemInd_gnTrafficClass_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::SpatemInd_btpDestinationPort_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::SpatemInd_btpInfo_raw_.fieldlength +
             LibItsMapemSpatem__TypesAndValues::SpatemInd_ssp_raw_.fieldlength + LibItsMapemSpatem__TypesAndValues::SpatemInd_its__aid_raw_.fieldlength) /
            8;
    // Decode CA message
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), spatem) == -1) {
      loggers::get_instance().warning("fx__dec__SpatemSpatemInd: -1 result code was returned");
      return -1;
    }
    p_mapemInd.msgIn() = spatem;
    // Decode lower layer data
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__SpatemInd: ", decoding_buffer);
    // gnNextHeader
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnNextHeader_raw_.fieldlength / 8)) {
      p_mapemInd.gnNextHeader().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnNextHeader_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnNextHeader() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__SpatemInd: gnNextHeader=", p_mapemInd.gnNextHeader());
    loggers::get_instance().log_to_hexa("fx__dec__SpatemInd: ", decoding_buffer);
    // gnHeaderType
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderType_raw_.fieldlength / 8)) {
      p_mapemInd.gnHeaderType().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderType_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnHeaderType() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: gnHeaderType=", p_mapemInd.gnHeaderType());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // gnHeaderSubtype
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderSubtype_raw_.fieldlength / 8)) {
      p_mapemInd.gnHeaderSubtype().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnHeaderSubtype_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnHeaderSubtype() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: gnHeaderSubtype=", p_mapemInd.gnHeaderSubtype());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // gnLifetime
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnLifetime_raw_.fieldlength / 8)) {
      p_mapemInd.gnLifetime().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnLifetime_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnLifetime() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: gnLifetime=", p_mapemInd.gnLifetime());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // gnTrafficClass
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnTrafficClass_raw_.fieldlength / 8)) {
      p_mapemInd.gnTrafficClass().set_to_omit();
    } else {
      OCTETSTRING os;
      os.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_gnTrafficClass_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.gnTrafficClass() = oct2int(os);
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: gnTrafficClass=", p_mapemInd.gnTrafficClass());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // btpDestinationPort
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_btpDestinationPort_raw_.fieldlength / 8)) {
      p_mapemInd.btpDestinationPort().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_btpDestinationPort_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.btpDestinationPort() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: btpDestinationPort=", p_mapemInd.btpDestinationPort());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // btpInfo
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_btpInfo_raw_.fieldlength / 8)) {
      p_mapemInd.btpInfo().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_btpInfo_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.btpInfo() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: btpInfo=", p_mapemInd.btpInfo());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // ssp
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_ssp_raw_.fieldlength / 8)) {
      p_mapemInd.ssp().set_to_omit();
    } else {
      BITSTRING bs;
      bs.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_ssp_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.ssp() = bs;
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: ssp=", p_mapemInd.ssp());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);
    // its__aid
    if (decoding_buffer.get_read_len() < static_cast<size_t>(LibItsMapemSpatem__TypesAndValues::SpatemInd_its__aid_raw_.fieldlength / 8)) {
      p_mapemInd.its__aid().set_to_omit();
    } else {
      INTEGER i;
      i.decode(LibItsMapemSpatem__TypesAndValues::SpatemInd_its__aid_descr_, decoding_buffer, TTCN_EncDec::CT_RAW);
      p_mapemInd.its__aid() = i;
    }
    loggers::get_instance().log_msg("fx__dec__Spatem: its_aid=", p_mapemInd.its__aid());
    loggers::get_instance().log_to_hexa("fx__dec__Spatem: ", decoding_buffer);

    return 0;
  }

  BITSTRING fx__enc__SPATEM(const SPATEM__PDU__Descriptions::SPATEM &p_spatem) {
    loggers::get_instance().log_msg(">>> fx__enc__Spatem: ", p_spatem);

    spatem_codec codec;
    OCTETSTRING  os;
    codec.encode(p_spatem, os);
    return oct2bit(os);
  }

  INTEGER fx__dec__SPATEM(BITSTRING &b, SPATEM__PDU__Descriptions::SPATEM &p_spatem) {
    loggers::get_instance().log(">>> fx__dec__Spatem");

    spatem_codec codec;
    OCTETSTRING  is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p_spatem) == -1) {
      loggers::get_instance().warning("fx__dec__Spatem: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__UtMapemSpatemInitialize(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize &p_utMapemSpatemInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtMapemSpatemInitialize: ", p_utMapemSpatemInitialize);

    uppertester_mapem_spatem_codec codec;
    OCTETSTRING                    os;
    codec.encode(p_utMapemSpatemInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtMapemSpatemResults(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &p_utMapemSpatemResults) {
    loggers::get_instance().log_msg(">>> fx__enc__UtMapemSpatemResults: ", p_utMapemSpatemResults);

    /*uppertester_mapem_spatem_codec codec;
    OCTETSTRING os;
    codec.encode(p_utMapemSpatemResults, os);
    return oct2bit(os);*/
    return int2bit(0, 1); // FIXME To be done
  }

  /****************************************************
   * @desc    External function to decode a UtMapemSatemResults type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtMapemSpatemResults(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &p) {
    loggers::get_instance().log(">>> fx__dec__UtMapemSpatemResults");

    uppertester_mapem_spatem_codec codec;
    OCTETSTRING                    is  = bit2oct(b);
    std::unique_ptr<Base_Type>     ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtMapemSpatemResults: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtMapemEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtMapemEventInd(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtMapemEventInd");

    uppertester_mapem_spatem_codec codec;
    OCTETSTRING                    is  = bit2oct(b);
    std::unique_ptr<Base_Type>     ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtMapemEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &>(*ptr);
    return 0;
  }

  /****************************************************
   * @desc    External function to decode a UtSpatemEventInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  INTEGER fx__dec__UtSpatemEventInd(BITSTRING &b, LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &p) {
    loggers::get_instance().log(">>> fx__dec__UtSpatemEventInd");

    uppertester_mapem_spatem_codec codec;
    OCTETSTRING                    is  = bit2oct(b);
    std::unique_ptr<Base_Type>     ptr = codec.decode(is);
    if (ptr == nullptr) {
      loggers::get_instance().warning("fx__dec__UtSpatemEventInd: -1 result code was returned");
      return -1;
    }
    p = static_cast<LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &>(*ptr);
    return 0;
  }

} // namespace LibItsMapemSpatem__EncdecDeclarations
