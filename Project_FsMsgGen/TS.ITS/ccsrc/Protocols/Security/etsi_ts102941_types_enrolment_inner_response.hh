#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesEnrolment.hh"

class etsi_ts102941_types_enrolment_inner_response : public codec<EtsiTs102941TypesEnrolment::InnerEcResponse, EtsiTs102941TypesEnrolment::InnerEcResponse> {
public:
  explicit etsi_ts102941_types_enrolment_inner_response() : codec<EtsiTs102941TypesEnrolment::InnerEcResponse, EtsiTs102941TypesEnrolment::InnerEcResponse>(){};
  virtual ~etsi_ts102941_types_enrolment_inner_response(){};

  virtual int encode(const EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response, params *p_params = NULL);
}; // End of class etsi_ts102941_types_enrolment_inner_response
