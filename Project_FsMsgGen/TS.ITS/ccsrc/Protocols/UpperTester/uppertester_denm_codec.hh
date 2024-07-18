/*!
 * \file      uppertester_denm_codec.hh
 * \brief     Header file for ITS UpperTester DENM codec definition.
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

namespace LibItsDenm__TypesAndValues {
  class UtDenmResults;
  class UtDenmEventInd;
} // namespace LibItsDenm__TypesAndValues

class uppertester_denm_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utDenmInitialize;
  static unsigned char c_utDenmInitializeResult;
  static unsigned char c_utDenmChangePosition;
  static unsigned char c_utDenmChangePositionResult;
  static unsigned char c_utDenmChangePseudonym;
  static unsigned char c_utDenmChangePseudonymResult;
  static unsigned char c_utDenmTrigger;
  static unsigned char c_utDenmTriggerResult;
  static unsigned char c_utDenmUpdate;
  static unsigned char c_utDenmUpdateResult;
  static unsigned char c_utDenmTermination;
  static unsigned char c_utDenmTerminationResult;
  static unsigned char c_utDenmEventIndication;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_denm_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_denm_codec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsDenm__TypesAndValues::UtDenmResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsDenm__TypesAndValues::UtDenmEventInd &msg, params *params = NULL);
}; // End of class uppertester_denm_codec
