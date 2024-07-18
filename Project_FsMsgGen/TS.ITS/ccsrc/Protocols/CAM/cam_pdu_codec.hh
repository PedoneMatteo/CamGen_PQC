#pragma once

#include "codec.hh"
#include "params.hh"
#include "per_codec.hh"

class BITSTRING;   //! Forward declaration of TITAN class
class OCTETSTRING; //! Forward declaration of TITAN class

namespace CAM__PDU__Descriptions { //! Forward declaration of asn1c CAM class
  class CAM;
}

class cam_pdu_codec : public per_codec<CAM__PDU__Descriptions::CAM> {
public:
  explicit cam_pdu_codec(){};
  virtual int encode(const CAM__PDU__Descriptions::CAM &cam, BITSTRING &data);
  virtual int decode(const BITSTRING &data, CAM__PDU__Descriptions::CAM &);
};
