#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941MessagesCa.hh"

class etsi_ts102941_data_content_codec : public codec<EtsiTs102941MessagesCa::EtsiTs102941DataContent, EtsiTs102941MessagesCa::EtsiTs102941DataContent> {
public:
  explicit etsi_ts102941_data_content_codec() : codec<EtsiTs102941MessagesCa::EtsiTs102941DataContent, EtsiTs102941MessagesCa::EtsiTs102941DataContent>(){};
  virtual ~etsi_ts102941_data_content_codec(){};

  virtual int encode(const EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_10291_data_content, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_10291_data_content, params *p_params = NULL);
}; // End of class etsi_ts102941_data_content_codec
