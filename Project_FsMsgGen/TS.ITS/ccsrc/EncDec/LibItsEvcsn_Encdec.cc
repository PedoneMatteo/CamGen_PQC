#include "LibItsEvcsn_EncdecDeclarations.hh"

namespace LibItsEvcsn__EncdecDeclarations {
  /****************************************************
   * @desc    External function to encode a EvcsnReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__EvcsnReq(const LibItsEvcsn__TestSystem::EvcsnReq &p) {
    // set error behavior
    TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_WARNING);

    TTCN_Buffer TTCN_buf;
    TTCN_buf.clear();

    // encode message in BER (CER variant, but can be any)
    p.msgOut().encode(EVCSN__PDU__Descriptions::EvcsnPdu_descr_, TTCN_buf, TTCN_EncDec::CT_BER, BER_ENCODE_CER);

    OCTETSTRING encodedData(TTCN_buf.get_len(), TTCN_buf.get_data());

    // insert BER2PER recoding here!

    return oct2bit(encodedData);
  }

  /****************************************************
   * @desc    External function to decode a DenmReq type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  /* TODO RGy function not implemented! (decvalue() not used by test suite @29-06-2016)
  INTEGER fx__dec__EvcsnReq(BITSTRING& b, LibItsEvcsn__TestSystem::EvcsnReq& p)
  {
    return -1;
  }
  */
} // namespace LibItsEvcsn__EncdecDeclarations
