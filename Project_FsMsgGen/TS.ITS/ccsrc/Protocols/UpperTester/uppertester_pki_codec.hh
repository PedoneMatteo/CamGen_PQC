/*!
 * \file      uppertester_pki_codec.hh
 * \brief     Header file for ITS UpperTester PKI codec definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <memory>

#include "codec.hh"
#include "params.hh"

class Base_Type;
class Record_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsPki__TypesAndValues {
  class UtPkiTrigger;
  class UtPkiResults;
  class UtPkiTriggerInd;
} // namespace LibItsPki__TypesAndValues

class uppertester_pki_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utPkiInitialize;
  static unsigned char c_utPkiInitializeResult;
  static unsigned char c_utPkiTriggerResult;
  static unsigned char c_utPkiTriggerEnrolmentRequest;
  static unsigned char c_utPkiTriggerAuthorizationRequest;
  static unsigned char c_utPkiTriggerInd;

private:
  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_pki_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_pki_codec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  int                        encode(const LibItsPki__TypesAndValues::UtPkiTrigger &, OCTETSTRING &data);
  int                        encode(const LibItsPki__TypesAndValues::UtPkiTriggerInd &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);
}; // End of class uppertester_pki_codec
