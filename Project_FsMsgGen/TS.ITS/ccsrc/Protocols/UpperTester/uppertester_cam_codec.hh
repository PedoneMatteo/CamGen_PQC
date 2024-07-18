/*!
 * \file      uppertester_cam_codec.hh
 * \brief     Header file for ITS UpperTester CAM codec definition.
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

namespace LibItsCam__TypesAndValues {
  class UtCamTrigger;
  class UtCamResults;
  class UtCamEventInd;
} // namespace LibItsCam__TypesAndValues

class uppertester_cam_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utCamInitialize;
  static unsigned char c_utCamInitializeResult;
  static unsigned char c_utCamChangePosition;
  static unsigned char c_utCamChangePositionResult;
  static unsigned char c_utCamChangeCurvature;
  static unsigned char c_utCamChangeSpeed;
  static unsigned char c_utCamSetAccelerationStatus;
  static unsigned char c_utCamSetExteriorLightsStatus;
  static unsigned char c_utCamChangeHeading;
  static unsigned char c_utCamSetDriveDirection;
  static unsigned char c_utCamChangeYawRate;
  static unsigned char c_utCamSetStationType;
  static unsigned char c_utCamSetVehicleRole;
  static unsigned char c_utCamSetEmbarkationStatus;
  static unsigned char c_utCamSetPtActivation;
  static unsigned char c_utCamSetDangerousGoods;
  static unsigned char c_utCamSetLightBarSirene;
  static unsigned char c_utCamTriggerResult;
  static unsigned char c_utCamEventInd;

private:
  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_cam_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_cam_codec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  int                        encode(const LibItsCam__TypesAndValues::UtCamTrigger &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsCam__TypesAndValues::UtCamResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsCam__TypesAndValues::UtCamEventInd &msg, params *params = NULL);
}; // End of class uppertester_cam_codec
