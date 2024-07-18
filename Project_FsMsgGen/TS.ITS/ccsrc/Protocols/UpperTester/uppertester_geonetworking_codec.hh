/*!
 * \file      uppertester_geonetworking_codec.hh
 * \brief     Header file for ITS UpperTester GeoNetworking codec definition.
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

namespace LibItsGeoNetworking__TypesAndValues {
  class UtGnResults;
  class UtGnEventInd;
} // namespace LibItsGeoNetworking__TypesAndValues

class uppertester_geonetworking_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utGnInitialize;
  static unsigned char c_utGnInitializeResult;
  static unsigned char c_utGnChangePosition;
  static unsigned char c_utGnChangePositionResult;
  static unsigned char c_utGnTriggerResult;
  static unsigned char c_utGnTrigger_GeoUnicast;
  static unsigned char c_utGnTrigger_GeoBroadcast;
  static unsigned char c_utGnTrigger_GeoAnycast;
  static unsigned char c_utGnTrigger_Shb;
  static unsigned char c_utGnTrigger_Tsb;
  static unsigned char c_utGnEventIndication;

private:
  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_geonetworking_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_geonetworking_codec(){};

  virtual int                        encode(const Record_Type &, OCTETSTRING &data);
  virtual int                        encode(const Base_Type &, OCTETSTRING &data);
  virtual int                        decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  virtual std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::UtGnResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsGeoNetworking__TypesAndValues::UtGnEventInd &msg, params *params = NULL);
}; // End of class uppertester_geonetworking_codec
