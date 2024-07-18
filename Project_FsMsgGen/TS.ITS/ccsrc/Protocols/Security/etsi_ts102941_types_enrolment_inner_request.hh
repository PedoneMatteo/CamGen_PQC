#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesEnrolment.hh"

class etsi_ts102941_types_enrolment_inner_request : public codec<EtsiTs102941TypesEnrolment::InnerEcRequest, EtsiTs102941TypesEnrolment::InnerEcRequest> {
public:
  explicit etsi_ts102941_types_enrolment_inner_request() : codec<EtsiTs102941TypesEnrolment::InnerEcRequest, EtsiTs102941TypesEnrolment::InnerEcRequest>(){};
  virtual ~etsi_ts102941_types_enrolment_inner_request(){};

  virtual int encode(const EtsiTs102941TypesEnrolment::InnerEcRequest &p_inner_request, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesEnrolment::InnerEcRequest &p_inner_request, params *p_params = NULL);
}; // End of class etsi_ts102941_types_enrolment_inner_request
