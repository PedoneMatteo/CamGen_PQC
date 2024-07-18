#include "LibItsBtp_EncdecDeclarations.hh"

#include "btp_codec.hh"
#include "loggers.hh"

namespace LibItsBtp__EncdecDeclarations {

  /****************************************************
   * @desc    External function to encode a BtpReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__BtpReq(LibItsBtp__TypesAndValues::BtpReq const &p_btpReq) {
    loggers::get_instance().log_msg(">>> fx__enc__BtpReq: ", p_btpReq);

    btp_codec   codec;
    OCTETSTRING os;
    if (codec.encode(p_btpReq.msgOut(), os) == -1) {
      loggers::get_instance().warning("fx__enc__BtpReq: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a BtpReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__BtpReq(BITSTRING &b, LibItsBtp__TypesAndValues::BtpReq &p) {
    loggers::get_instance().log(">>> fx__dec__BtpReq");
    btp_codec                            codec;
    LibItsBtp__TypesAndValues::BtpPacket pdu;
    OCTETSTRING                          is = bit2oct(b);

    if (codec.decode(is, pdu) == -1) {
      loggers::get_instance().warning("fx__dec__BtpReq: -1 result code was returned");
      return -1;
    }
    p.msgOut() = pdu;

    return 0;
  }

  /****************************************************
   * @desc    External function to encode a BtpInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__BtpInd(LibItsBtp__TypesAndValues::BtpInd const &p_btpInd) {
    loggers::get_instance().log_msg(">>> fx__enc__BtpInd: ", p_btpInd);

    btp_codec   codec;
    OCTETSTRING os;
    if (codec.encode(p_btpInd.msgIn(), os) == -1) {
      loggers::get_instance().warning("fx__enc__BtpInd: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a BtpInd type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__BtpInd(BITSTRING &b, LibItsBtp__TypesAndValues::BtpInd &p_btpInd) {
    loggers::get_instance().log(">>> fx__dec__BtpInd");
    btp_codec                            codec;
    LibItsBtp__TypesAndValues::BtpPacket pdu;
    OCTETSTRING                          is = bit2oct(b);

    // Calculate the size of the lower layers information
    if (codec.decode(is, pdu) == -1) {
      loggers::get_instance().warning("fx__dec__BtpInd: -1 result code was returned");
      return -1;
    }
    p_btpInd.msgIn() = pdu;

    loggers::get_instance().log_msg("<<< fx__dec__BtpInd: ", p_btpInd);
    return 0;
  }

  /****************************************************
   * @desc    External function to encode a BtpPacket type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__BtpPacket(LibItsBtp__TypesAndValues::BtpPacket const &p_btpPacket) {
    loggers::get_instance().log_msg(">>> fx__enc__BtpPacket: ", p_btpPacket);

    btp_codec   codec;
    OCTETSTRING os;
    codec.encode(p_btpPacket, os);
    return oct2bit(os);
  }

  /****************************************************
   * @desc    External function to decode a BtpPacket type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__BtpPacket(BITSTRING &b, LibItsBtp__TypesAndValues::BtpPacket &p_btpPacket) {
    loggers::get_instance().log(">>> fx__dec__BtpPacket");
    btp_codec   codec;
    OCTETSTRING is = bit2oct(b);

    if (codec.decode(is, p_btpPacket) == -1) {
      loggers::get_instance().warning("fx__dec__BtpPacket: -1 result code was returned");
      return -1;
    }
    return 0;
  }

} // namespace LibItsBtp__EncdecDeclarations
