/*!
 * \file      LibItsGeoNetworking_EncdecDeclarations.cc
 * \brief     Implementation file for GeoNetworking 'encvalue'/'decvalue'.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#include "LibItsGeoNetworking_EncdecDeclarations.hh"

#include "geonetworking_codec.hh"
#include "loggers.hh"
#include "uppertester_geonetworking_codec.hh"

namespace LibItsGeoNetworking__EncdecDeclarations {

  /****************************************************
   * \brief External function to encode a GeoNetworkingReq type
   * \param[in] p_geoNetworkingReq The value to encode
   * \return  encoded value
   ****************************************************/
  BITSTRING fx__enc__GeoNetworkingReq(LibItsGeoNetworking__TypesAndValues::GeoNetworkingReq const &p_geoNetworkingReq) {
    loggers::get_instance().log_msg(">>> fx__enc__GeoNetworkingReq: ", p_geoNetworkingReq);

    float       duration;
    std::string tag("fx__enc__GeoNetworkingReq");
    loggers::get_instance().set_start_time(tag);
    geonetworking_codec codec;
    OCTETSTRING         os;
    if (codec.encode(p_geoNetworkingReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__GeoNetworkingReq: -1 result code was returned");
      return int2bit(0, 1);
    }
    TTCN_Buffer encoding_buffer(os);
    for (int i = 1; i < p_geoNetworkingReq.get_count(); i++) {
      loggers::get_instance().log("fx__enc__GeoNetworkingReq: processing %s/%s/%s - %d - %d", p_geoNetworkingReq.fld_name(i),
                                  p_geoNetworkingReq.fld_descr(i)->name, p_geoNetworkingReq.get_at(i)->get_descriptor()->name,
                                  p_geoNetworkingReq.get_at(i)->is_optional(), p_geoNetworkingReq.get_at(i)->is_present());
      if (p_geoNetworkingReq.get_at(i)->is_optional() && p_geoNetworkingReq.get_at(i)->is_present()) {
        p_geoNetworkingReq.get_at(i)->get_opt_value()->encode(*p_geoNetworkingReq.fld_descr(i), encoding_buffer, TTCN_EncDec::CT_RAW);
      } else {
        p_geoNetworkingReq.get_at(i)->encode(*p_geoNetworkingReq.fld_descr(i), encoding_buffer, TTCN_EncDec::CT_RAW);
      }
    } // End of 'for' statement
    loggers::get_instance().set_stop_time(tag, duration);

    return oct2bit(OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data()));
  } // End of function fx__enc__GeoNetworkingReq

  /****************************************************
   * @desc    External function to decode a GeoNetworkingReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__GeoNetworkingReq(BITSTRING &b, LibItsGeoNetworking__TypesAndValues::GeoNetworkingReq &p) {
    loggers::get_instance().log(">>> fx__dec__GeoNetworkingReq");
    geonetworking_codec                                   codec;
    LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu pdu;
    OCTETSTRING                                           is = bit2oct(b);

    // Calculate the size of the lower layers information
    unsigned int s = (LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd_macDestinationAddress_raw_.fieldlength +
                      LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd_its__aid_raw_.fieldlength) /
                     8;
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), pdu) == -1) {
      loggers::get_instance().warning("fx__dec__GeoNetworkingReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = pdu;
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__GeoNetworkingInd: Lower layer: ", decoding_buffer);
    for (int i = 1; i < p.get_count(); i++) {
      loggers::get_instance().log("fx__dec__GeoNetworkingReq: processing %s/%s/%s", p.fld_name(i), p.fld_descr(i)->name, p.get_at(i)->get_descriptor()->name);
      p.get_at(i)->decode(*p.fld_descr(i), decoding_buffer, TTCN_EncDec::CT_RAW);
    } // End of 'for' statement

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a GeoNetworkingInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__GeoNetworkingInd(LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd const &p_geoNetworkingInd) {
    loggers::get_instance().log_msg(">>> fx__enc__GeoNetworkingInd: ", p_geoNetworkingInd);

    geonetworking_codec codec;
    OCTETSTRING         os;
    if (codec.encode(p_geoNetworkingInd.msgIn(), os) == -1) {
      loggers::get_instance().warning("fx__enc__GeoNetworkingInd: -1 result code was returned");
      return int2bit(0, 1);
    }
    TTCN_Buffer encoding_buffer(os);
    for (int i = 1; i < p_geoNetworkingInd.get_count(); i++) {
      loggers::get_instance().log("fx__enc__GeoNetworkingInd: processing %s/%s/%s - %d - %d", p_geoNetworkingInd.fld_name(i),
                                  p_geoNetworkingInd.fld_descr(i)->name, p_geoNetworkingInd.get_at(i)->get_descriptor()->name,
                                  p_geoNetworkingInd.get_at(i)->is_optional(), p_geoNetworkingInd.get_at(i)->is_present());
      if (p_geoNetworkingInd.get_at(i)->is_optional() && p_geoNetworkingInd.get_at(i)->is_present()) {
        p_geoNetworkingInd.get_at(i)->get_opt_value()->encode(*p_geoNetworkingInd.fld_descr(i), encoding_buffer, TTCN_EncDec::CT_RAW);
      } else {
        p_geoNetworkingInd.get_at(i)->encode(*p_geoNetworkingInd.fld_descr(i), encoding_buffer, TTCN_EncDec::CT_RAW);
      }
    } // End of 'for' statement

    return oct2bit(OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data()));
  }

  /****************************************************
   * @desc    External function to decode a GeoNetworkingInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__GeoNetworkingInd(BITSTRING &b, LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd &p_geoNetworkingInd) {
    loggers::get_instance().log(">>> fx__dec__GeoNetworkingInd");

    float       duration;
    std::string tag("fx__dec__GeoNetworkingInd");
    loggers::get_instance().set_start_time(tag);

    geonetworking_codec                                   codec;
    LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu pdu;
    OCTETSTRING                                           is = bit2oct(b);

    // Calculate the size of the lower layers information
    unsigned int s =
      (LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd_macDestinationAddress_raw_.fieldlength +
       LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd_ssp_raw_.fieldlength + LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd_its__aid_raw_.fieldlength) /
      8;
    if (codec.decode(OCTETSTRING(is.lengthof() - s, static_cast<const unsigned char *>(is)), pdu) == -1) {
      loggers::get_instance().warning("fx__dec__GeoNetworkingInd: -1 result code was returned");
      return -1;
    }
    p_geoNetworkingInd.msgIn() = pdu;
    TTCN_Buffer decoding_buffer(OCTETSTRING(s, static_cast<const unsigned char *>(is) + is.lengthof() - s));
    loggers::get_instance().log_to_hexa("fx__dec__GeoNetworkingInd: Lower layer: ", decoding_buffer);
    for (int i = 1; i < p_geoNetworkingInd.get_count(); i++) {
      loggers::get_instance().log("fx__dec__GeoNetworkingInd: processing %s/%s/%s - %d - %d", p_geoNetworkingInd.fld_name(i),
                                  p_geoNetworkingInd.fld_descr(i)->name, p_geoNetworkingInd.get_at(i)->get_descriptor()->name,
                                  p_geoNetworkingInd.get_at(i)->is_optional(), p_geoNetworkingInd.get_at(i)->is_present());
      if (p_geoNetworkingInd.get_at(i)->is_optional()) {
        loggers::get_instance().log("fx__dec__GeoNetworkingInd: Bytes remaining: %d - field lenth: %d", decoding_buffer.get_len() - decoding_buffer.get_pos(),
                                    p_geoNetworkingInd.fld_descr(i)->raw->fieldlength / 8);
        if (std::string(p_geoNetworkingInd.fld_name(i)).compare("ssp") == 0) {
          if ((decoding_buffer.get_len() - decoding_buffer.get_pos()) >= (unsigned int)p_geoNetworkingInd.fld_descr(i)->raw->fieldlength / 8) {
            loggers::get_instance().log("fx__dec__GeoNetworkingInd: decoding %s", p_geoNetworkingInd.fld_descr(i)->name);
            BITSTRING ssp;
            ssp.decode(*p_geoNetworkingInd.fld_descr(i), decoding_buffer, TTCN_EncDec::CT_RAW);
            loggers::get_instance().log_msg("fx__dec__GeoNetworkingInd: ssp=", ssp);
            dynamic_cast<OPTIONAL<BITSTRING> &>(*p_geoNetworkingInd.get_at(i)) = OPTIONAL<BITSTRING>(ssp);
          } else {
            loggers::get_instance().log("fx__dec__GeoNetworkingInd: set %s to omit", p_geoNetworkingInd.fld_descr(i)->name);
            dynamic_cast<OPTIONAL<BITSTRING> &>(*p_geoNetworkingInd.get_at(i)).set_to_omit();
          }
        } else if (std::string(p_geoNetworkingInd.fld_name(i)).compare("its_aid") == 0) {
          loggers::get_instance().log("fx__dec__GeoNetworkingInd: Bytes remaining: %d - its_aid lenth: %d",
                                      decoding_buffer.get_len() - decoding_buffer.get_pos(), p_geoNetworkingInd.fld_descr(i)->raw->fieldlength / 8);
          if ((decoding_buffer.get_len() - decoding_buffer.get_pos()) >= (unsigned int)p_geoNetworkingInd.fld_descr(i)->raw->fieldlength / 8) {
            INTEGER its_aid;
            its_aid.decode(*p_geoNetworkingInd.fld_descr(i), decoding_buffer, TTCN_EncDec::CT_RAW);
            loggers::get_instance().log_msg("fx__dec__GeoNetworkingInd: its_aid=", its_aid);
            dynamic_cast<OPTIONAL<INTEGER> &>(*p_geoNetworkingInd.get_at(i)) = OPTIONAL<INTEGER>(its_aid);
          } else {
            loggers::get_instance().log("fx__dec__GeoNetworkingInd: set %s to omit", p_geoNetworkingInd.fld_descr(i)->name);
            dynamic_cast<OPTIONAL<INTEGER> &>(*p_geoNetworkingInd.get_at(i)).set_to_omit();
          }
        } else {
          loggers::get_instance().log("fx__dec__GeoNetworkingInd(1): decoding %s", p_geoNetworkingInd.fld_descr(i)->name);
          p_geoNetworkingInd.get_at(i)->decode(*p_geoNetworkingInd.fld_descr(i), decoding_buffer, TTCN_EncDec::CT_RAW);
        }
      } else {
        loggers::get_instance().log("fx__dec__GeoNetworkingInd(2): decoding %s", p_geoNetworkingInd.fld_descr(i)->name);
        p_geoNetworkingInd.get_at(i)->decode(*p_geoNetworkingInd.fld_descr(i), decoding_buffer, TTCN_EncDec::CT_RAW);
      }
    } // End of 'for' statement
    loggers::get_instance().set_stop_time(tag, duration);

    loggers::get_instance().log_msg("<<< fx__dec__GeoNetworkingInd: ", p_geoNetworkingInd);
    return 0;
  }

  /****************************************************
   * @desc    External function to encode a GeoNetworkingPdu type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__GeoNetworkingPdu(LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu const &p_geoNetworkingPdu) {
    loggers::get_instance().log_msg(">>> fx__enc__GeoNetworkingPdu: ", p_geoNetworkingPdu);

    geonetworking_codec codec;
    OCTETSTRING         os;
    codec.encode(p_geoNetworkingPdu, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a GeoNetworkingPdu type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__GeoNetworkingPdu(BITSTRING &b, LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &p) {
    loggers::get_instance().log(">>> fx__dec__GeoNetworkingPdu");
    geonetworking_codec codec;
    OCTETSTRING         is = bit2oct(b);

    if (codec.decode(OCTETSTRING(is.lengthof(), static_cast<const unsigned char *>(is)), p) == -1) {
      loggers::get_instance().warning("fx__dec__GeoNetworkingPdu: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a GnNonSecuredPacket type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__GnNonSecuredPacket(const LibItsGeoNetworking__TypesAndValues::GnNonSecuredPacket &p_gnNonSecuredPacket) {
    loggers::get_instance().log_msg(">>> fx__enc__GnNonSecuredPacket: ", p_gnNonSecuredPacket);

    geonetworking_codec codec;
    OCTETSTRING         os;
    codec.encode(p_gnNonSecuredPacket, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a GnNonSecuredPacket type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__GnNonSecuredPacket(BITSTRING &b, LibItsGeoNetworking__TypesAndValues::GnNonSecuredPacket &p) { return -1; }

  BITSTRING fx__enc__UtGnInitialize(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &p_utGnInitialize) {
    loggers::get_instance().log_msg(">>> fx__enc__UtGnInitialize: ", p_utGnInitialize);

    uppertester_geonetworking_codec codec;
    OCTETSTRING                     os;
    codec.encode(p_utGnInitialize, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtGnChangePosition(const LibItsGeoNetworking__TypesAndValues::UtGnChangePosition &p_utGnChangePosition) {
    loggers::get_instance().log_msg(">>> fx__enc__UtGnChangePosition: ", p_utGnChangePosition);

    uppertester_geonetworking_codec codec;
    OCTETSTRING                     os;
    codec.encode(p_utGnChangePosition, os);
    return oct2bit(os);
  }

  BITSTRING fx__enc__UtGnTrigger(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &p_utGnTrigger) {
    loggers::get_instance().log_msg(">>> fx__enc__UtGnTrigger: ", p_utGnTrigger);

    uppertester_geonetworking_codec codec;
    OCTETSTRING                     os;
    codec.encode(p_utGnTrigger, os);
    return oct2bit(os);
  }

} // namespace LibItsGeoNetworking__EncdecDeclarations
