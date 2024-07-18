#include <asn1/asn_application.h>

#include "asn1_recode_per.hh"

#include <TTCN3.hh>

extern "C" {
static int asn1c_collect_encoded_data(const void *buffer, size_t size, void *application_specific_key) {
  TTCN_Buffer *tb = (TTCN_Buffer *)application_specific_key;
  tb->put_s(size, (unsigned char *)buffer);
  return 0;
}
}

int asn1_recode_per::recode(const asn_TYPE_descriptor_s &td, int from, int to, TTCN_Buffer &buf) {
  int            rc  = -1;
  void *         ptr = NULL;
  asn_dec_rval_t rc_d;
  rc_d = asn_decode(NULL, (asn_transfer_syntax)from, (asn_TYPE_descriptor_s *)&td, &ptr, buf.get_data(), buf.get_len());
  if (rc_d.code == RC_OK) {
    // Encode as PER
    asn_enc_rval_t rc_e;
    buf.clear();
    rc_e = asn_encode(NULL, (asn_transfer_syntax)to, (asn_TYPE_descriptor_s *)&td, ptr, asn1c_collect_encoded_data, &buf);
    rc   = rc_e.encoded;
  }
  if (ptr) {
    ASN_STRUCT_FREE(td, ptr);
  }
  return rc;
}

int asn1_recode_per::ber2per(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf) { return recode(td, (int)ATS_BER, (int)ATS_UNALIGNED_CANONICAL_PER, buf); }

int asn1_recode_per::per2ber(const asn_TYPE_descriptor_s &td, TTCN_Buffer &buf) { return recode(td, (int)ATS_UNALIGNED_BASIC_PER, (int)ATS_DER, buf); }

/*int asn1_recode_oer::ber2oer(const asn_TYPE_descriptor_s & td, TTCN_Buffer & buf)
{
  return recode(td, (int)ATS_XER, (int)ATS_CANONICAL_OER, buf);
}

int asn1_recode_oer::oer2ber(const asn_TYPE_descriptor_s & td, TTCN_Buffer & buf)
{
  return recodeOer(td, (int)ATS_BASIC_OER, (int)ATS_XER, buf);
  }*/
