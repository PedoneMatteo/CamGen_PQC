#pragma once

#include "cam_pdu_codec.hh"
#include "codec.hh"
#include "params.hh"

class BITSTRING;   //! Forward declaration of TITAN class
class OCTETSTRING; //! Forward declaration of TITAN class

namespace CAM__PDU__Descriptions { //! Forward declaration of asn1c CAM class
  class CAM;
}

class cam_codec : public codec<CAM__PDU__Descriptions::CAM, CAM__PDU__Descriptions::CAM> {
  cam_pdu_codec asn_codec;

public:
  explicit cam_codec() : codec<CAM__PDU__Descriptions::CAM, CAM__PDU__Descriptions::CAM>(), asn_codec(){};
  virtual ~cam_codec(){};

  virtual int encode(const CAM__PDU__Descriptions::CAM &cam, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, CAM__PDU__Descriptions::CAM &, params *params = NULL);
};
