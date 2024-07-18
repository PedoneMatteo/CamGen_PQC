
#include "LibItsHttp_MessageBodyTypes.hh"

#include "http_codec.hh"

#include "loggers.hh"

namespace LibItsHttp__EncdecDeclarations {

  BITSTRING fx__enc__http__message(const LibItsHttp__TypesAndValues::HttpMessage& p) {
    loggers::get_instance().log_msg(">>> fx__enc__http__message: ", (const Base_Type&)p);

    OCTETSTRING os;
    http_codec codec;
    codec.encode(p, os);

    return oct2bit(os);
  }
  INTEGER fx__dec__http__message(BITSTRING& pdu, LibItsHttp__TypesAndValues::HttpMessage& p) {
    loggers::get_instance().log_msg(">>> fx__dec__http__message: ", pdu);

    OCTETSTRING os = bit2oct(pdu);
    http_codec codec;
    codec.decode(os, p);

    return 0;
  }
} // End of namespace LibItsHttp__EncdecDeclarations
