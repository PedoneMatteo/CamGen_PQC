#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TrustLists.hh"

class etsi_ts102941_trust_lists_ctl_format : public codec<EtsiTs102941TrustLists::CtlFormat, EtsiTs102941TrustLists::CtlFormat> {
public:
  explicit etsi_ts102941_trust_lists_ctl_format() : codec<EtsiTs102941TrustLists::CtlFormat, EtsiTs102941TrustLists::CtlFormat>(){};
  virtual ~etsi_ts102941_trust_lists_ctl_format(){};

  virtual int encode(const EtsiTs102941TrustLists::CtlFormat &p_ctl_format, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TrustLists::CtlFormat &p_ctl_format, params *p_params = NULL);
}; // End of class etsi_ts102941_trust_lists_ctl_format
