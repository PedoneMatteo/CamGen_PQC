#include "LibItsSecurity_EncdecDeclarations.hh"

#include "etsi_ts103097_certificate_codec.hh"
#include "etsi_ts103097_data_codec.hh"
#include "etsi_ts103097_tobesigned_certificate_codec.hh"
#include "etsi_ts103097_tobesigned_data_codec.hh"

#include "loggers.hh"

namespace LibItsSecurity__EncdecDeclarations {

  BITSTRING fx__enc__CertificateBase(Ieee1609Dot2::CertificateBase const &p_cert) {
    loggers::get_instance().log_msg(">>> fx__enc__CertificateBase: ", p_cert);

    etsi_ts103097_certificate_codec codec;
    OCTETSTRING                     os;
    if (codec.encode(p_cert, os) == -1) {
      loggers::get_instance().warning("fx__enc__CertificateBase: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__CertificateBase(BITSTRING &b, Ieee1609Dot2::CertificateBase &p_cert) {
    loggers::get_instance().log_msg(">>> fx__dec__CertificateBase: ", b);

    etsi_ts103097_certificate_codec codec;
    OCTETSTRING                     is = bit2oct(b);
    if (codec.decode(is, p_cert) == -1) {
      loggers::get_instance().warning("fx__dec__CertificateBase: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__CertificateBase: ", p_cert);
    return 0;
  }

  BITSTRING fx__enc__ToBeSignedCertificate(const Ieee1609Dot2::ToBeSignedCertificate &p_toBeSignedCertificate) {
    loggers::get_instance().log_msg(">>> fx__enc__ToBeSignedCertificate: ", p_toBeSignedCertificate);

    etsi_ts103097_tobesigned_certificate_codec codec;
    OCTETSTRING                                os;
    if (codec.encode(p_toBeSignedCertificate, os) == -1) {
      loggers::get_instance().warning("fx__enc__ToBeSignedCertificate: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  BITSTRING fx__enc__Ieee1609Dot2Data(const Ieee1609Dot2::Ieee1609Dot2Data &p__ieee1609Dot2Data) {
    loggers::get_instance().log_msg(">>> fx__enc__Ieee1609Dot2Data: ", p__ieee1609Dot2Data);

    etsi_ts103097_data_codec codec;
    OCTETSTRING              os;
    if (codec.encode(p__ieee1609Dot2Data, os) == -1) {
      loggers::get_instance().warning("fx__enc__Ieee1609Dot2Data: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__Ieee1609Dot2Data(BITSTRING &p__data, Ieee1609Dot2::Ieee1609Dot2Data &p__ieee1609Dot2Data) {
    loggers::get_instance().log_msg(">>> fx__dec__Ieee1609Dot2Data: ", p__data);

    etsi_ts103097_data_codec codec;
    OCTETSTRING              is = bit2oct(p__data);
    if (codec.decode(is, p__ieee1609Dot2Data) == -1) {
      loggers::get_instance().warning("fx__dec__Ieee1609Dot2Data: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__Ieee1609Dot2Data: ", p__ieee1609Dot2Data);
    return 0;
  }

  BITSTRING fx__enc__Ieee1609Dot2ToBeSignedData(const Ieee1609Dot2::ToBeSignedData &p__toBeSignedData) {
    loggers::get_instance().log_msg(">>> fx__enc__Ieee1609Dot2ToBeSignedData: ", p__toBeSignedData);

    etsi_ts103097_tobesigned_data_codec codec;
    OCTETSTRING                         os;
    if (codec.encode(p__toBeSignedData, os) == -1) {
      loggers::get_instance().warning("fx__enc__Ieee1609Dot2ToBeSignedData: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__Ieee1609Dot2ToBeSignedData(BITSTRING &p__data, Ieee1609Dot2::ToBeSignedData &p__toBeSignedData) {
    loggers::get_instance().log_msg(">>> fx__dec__Ieee1609Dot2ToBeSignedData: ", p__data);

    etsi_ts103097_tobesigned_data_codec codec;
    OCTETSTRING                         is = bit2oct(p__data);
    if (codec.decode(is, p__toBeSignedData) == -1) {
      loggers::get_instance().warning("fx__dec__Ieee1609Dot2ToBeSignedData: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__Ieee1609Dot2ToBeSignedData: ", p__toBeSignedData);
    return 0;
  }

  BITSTRING fx__enc__SspCAM(const LibItsSecurity__TypesAndValues::SspCAM &p__ssp) {
    loggers::get_instance().log_msg(">>> fx__enc__SspCAM: ", p__ssp);

    TTCN_Buffer encoding_buffer;
    p__ssp.encode(*p__ssp.get_descriptor(), encoding_buffer, TTCN_EncDec::CT_RAW);
    return oct2bit(OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data()));
  }

  INTEGER fx__dec__SspCAM(BITSTRING &p__data, LibItsSecurity__TypesAndValues::SspCAM &p__ssp) {
    loggers::get_instance().log_msg(">>> fx__dec__SspCAM: ", p__data);

    TTCN_Buffer decoding_buffer(bit2oct(p__data));
    p__ssp.decode(*p__ssp.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
    return 0;
  }

  BITSTRING fx__enc__SspDENM(const LibItsSecurity__TypesAndValues::SspDENM &p__ssp) {
    loggers::get_instance().log_msg(">>> fx__enc__SspDENM: ", p__ssp);

    TTCN_Buffer encoding_buffer;
    p__ssp.encode(*p__ssp.get_descriptor(), encoding_buffer, TTCN_EncDec::CT_RAW);
    return oct2bit(OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data()));
  }

  INTEGER fx__dec__SspDENM(BITSTRING &p__data, LibItsSecurity__TypesAndValues::SspDENM &p__ssp) {
    loggers::get_instance().log_msg(">>> fx__dec__SspDENM: ", p__data);

    TTCN_Buffer decoding_buffer(bit2oct(p__data));
    p__ssp.decode(*p__ssp.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
    return 0;
  }

} // namespace LibItsSecurity__EncdecDeclarations
