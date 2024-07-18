#include "ConfigRsuSimulatorLayerFactory.hh"

#include "loggers.hh"

#include "ItsRSUsSimulator_TestSystem.hh"

#include "LibCommon_BasicTypesAndValues.hh"

#include "UpperTesterRtcmemCodec.hh"
#include "UpperTesterSremSsemCodec.hh"
#include "uppertester_cam_codec.hh"
#include "uppertester_denm_codec.hh"
#include "uppertester_geonetworking_codec.hh"
#include "uppertester_ivim_codec.hh"
#include "uppertester_mapem_spatem_codec.hh"

#include "LibItsCam_EncdecDeclarations.hh"
#include "LibItsDenm_EncdecDeclarations.hh"
#include "LibItsIvim_EncdecDeclarations.hh"
#include "LibItsMapemSpatem_EncdecDeclarations.hh"
#include "LibItsRtcmem_EncdecDeclarations.hh"
#include "LibItsSremSsem_EncdecDeclarations.hh"

ConfigRsuSimulatorLayer::ConfigRsuSimulatorLayer(const std::string &p_type, const std::string &param)
  : t_layer<ItsRSUsSimulator__TestSystem::ConfigRsuSimulatorPort>(p_type), _params(), _codec(), _codec_cam() {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::ConfigRsuSimulatorLayer: %s, %s", to_string().c_str(), param.c_str());
  // Setup parameters
  params::convert(_params, param);
}

void ConfigRsuSimulatorLayer::sendMsg(const ItsRSUsSimulator__TestSystem::CfInitialize &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  // Encode ConfigRsuSimulator PDU
  OCTETSTRING data;
  if (_codec.encode(static_cast<const Record_Type &>(send_par), data) == -1) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Encoding failure");
    return;
  }

  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnResults::ALT_utGnInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_geonetworking_codec::c_utGnInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utGnInitializeResult()));
  } else if (send_par.ischosen(LibItsGeoNetworking__TypesAndValues::UtGnResults::ALT_utGnTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_geonetworking_codec::c_utGnTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utGnTriggerResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtGnResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());

  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_geonetworking_codec::c_utGnEventIndication));
  OCTETSTRING l = int2oct(send_par.rawPayload().lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(send_par.rawPayload().lengthof(), static_cast<const unsigned char *>(send_par.rawPayload()));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsCam__TypesAndValues::UtCamResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsCam__TypesAndValues::UtCamResults::ALT_utCamInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utCamInitializeResult()));
  } else if (send_par.ischosen(LibItsCam__TypesAndValues::UtCamResults::ALT_utCamTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utCamTriggerResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtCamResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsCam__TypesAndValues::UtCamEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_cam_codec::c_utCamEventInd));
  BITSTRING   bs = LibItsCam__EncdecDeclarations::fx__enc__CAM(send_par.camMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsDenm__TypesAndValues::UtDenmResults::ALT_utDenmInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_denm_codec::c_utDenmInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utDenmInitializeResult()));
  } else if (send_par.ischosen(LibItsDenm__TypesAndValues::UtDenmResults::ALT_utDenmTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_denm_codec::c_utDenmTriggerResult));
    const LibItsDenm__TypesAndValues::UtDenmTriggerResult &r = send_par.utDenmTriggerResult();
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(r.result()));
    OCTETSTRING os = int2oct(r.actionId().originatingStationID(), 4);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
    os = int2oct(r.actionId().sequenceNumber(), 2);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  } else if (send_par.ischosen(LibItsDenm__TypesAndValues::UtDenmResults::ALT_utDenmUpdateResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_denm_codec::c_utDenmUpdateResult));
    const LibItsDenm__TypesAndValues::UtDenmUpdateResult &r = send_par.utDenmUpdateResult();
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(r.result()));
    OCTETSTRING os = int2oct(r.actionId().originatingStationID(), 4);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
    os = int2oct(r.actionId().sequenceNumber(), 2);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  } else if (send_par.ischosen(LibItsDenm__TypesAndValues::UtDenmResults::ALT_utDenmTerminationResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_denm_codec::c_utDenmTerminationResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utDenmTerminationResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtDenmResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsDenm__TypesAndValues::UtDenmEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_denm_codec::c_utDenmEventIndication));
  BITSTRING   bs = LibItsDenm__EncdecDeclarations::fx__enc__DENM(send_par.denMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsPki__TypesAndValues::UtPkiResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsPki__TypesAndValues::UtPkiResults::ALT_utPkiInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utPkiInitializeResult()));
  } else if (send_par.ischosen(LibItsPki__TypesAndValues::UtPkiResults::ALT_utPkiTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utPkiTriggerResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtPkiResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_pki_codec::c_utPkiTriggerInd));
  encoding_buffer.put_c(static_cast<const unsigned char>(send_par.state()));

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsIvim__TypesAndValues::UtIvimResults::ALT_utIvimInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utIvimInitializeResult()));
  } else if (send_par.ischosen(LibItsIvim__TypesAndValues::UtIvimResults::ALT_utIvimTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimTriggerResult));
    const LibItsIvim__TypesAndValues::UtIvimTriggerResult &r =
      dynamic_cast<const LibItsIvim__TypesAndValues::UtIvimTriggerResult &>(send_par.utIvimTriggerResult());
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(r.result()));
    OCTETSTRING os = int2oct(r.iviIdentificationNumber(), 2);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  } else if (send_par.ischosen(LibItsIvim__TypesAndValues::UtIvimResults::ALT_utIvimUpdateResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimUpdateResult));
    const LibItsIvim__TypesAndValues::UtIvimUpdateResult &r =
      dynamic_cast<const LibItsIvim__TypesAndValues::UtIvimUpdateResult &>(send_par.utIvimUpdateResult());
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(r.result()));
    OCTETSTRING os = int2oct(r.iviIdentificationNumber(), 2);
    encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  } else if (send_par.ischosen(LibItsIvim__TypesAndValues::UtIvimResults::ALT_utIvimTerminationResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimTerminationResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utIvimTerminationResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtIvimResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsIvim__TypesAndValues::UtIvimEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_ivim_codec::c_utIvimEventIndication));
  BITSTRING   bs = LibItsIvim__EncdecDeclarations::fx__enc__IVIM(send_par.iviMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults::ALT_utMapemSpatemInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utMapemSpatemInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utMapemSpatemInitializeResult()));
  } else if (send_par.ischosen(LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults::ALT_utMapemSpatemTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utMapemSpatemTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utMapemSpatemTriggerResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtMapemSpatemResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utMapemEventIndication));
  BITSTRING   bs = LibItsMapemSpatem__EncdecDeclarations::fx__enc__MAPEM(send_par.mapeMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(uppertester_mapem_spatem_codec::c_utSpatemEventIndication));
  BITSTRING   bs = LibItsMapemSpatem__EncdecDeclarations::fx__enc__SPATEM(send_par.spateMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsSremSsem__TypesAndValues::UtSremResults::ALT_utSremInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utSremInitializeResult()));
  } else if (send_par.ischosen(LibItsSremSsem__TypesAndValues::UtSremResults::ALT_utSremTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utSremTriggerResult()));
  } else if (send_par.ischosen(LibItsSremSsem__TypesAndValues::UtSremResults::ALT_utSremUpdateResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremUpdateResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utSremUpdateResult()));
  } else if (send_par.ischosen(LibItsSremSsem__TypesAndValues::UtSremResults::ALT_utSremCancelationResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremCancelationResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utSremCancelationResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtSremResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSremEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSremEventIndication));
  BITSTRING   bs = LibItsSremSsem__EncdecDeclarations::fx__enc__SREM(send_par.sreMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsSremSsem__TypesAndValues::UtSsemEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterSremSsemCodec::c_utSsemEventIndication));
  BITSTRING   bs = LibItsSremSsem__EncdecDeclarations::fx__enc__SSEM(send_par.sseMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemResults &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  if (send_par.ischosen(LibItsRtcmem__TypesAndValues::UtRtcmemResults::ALT_utRtcmemInitializeResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemInitializeResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utRtcmemInitializeResult()));
  } else if (send_par.ischosen(LibItsRtcmem__TypesAndValues::UtRtcmemResults::ALT_utRtcmemTriggerResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemTriggerResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utRtcmemTriggerResult()));
  } else if (send_par.ischosen(LibItsRtcmem__TypesAndValues::UtRtcmemResults::ALT_utRtcmemUpdateResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemUpdateResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utRtcmemUpdateResult()));
  } else if (send_par.ischosen(LibItsRtcmem__TypesAndValues::UtRtcmemResults::ALT_utRtcmemTerminationResult)) {
    encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemTerminationResult));
    encoding_buffer.put_c((unsigned char)static_cast<const boolean>(send_par.utRtcmemTerminationResult()));
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::sendMsg: Unsupported UtRtcmemResults variant");
    return;
  }

  OCTETSTRING os(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(os, params);
}

void ConfigRsuSimulatorLayer::sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &send_par, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::sendMsg: ", send_par);

  TTCN_Buffer encoding_buffer;
  encoding_buffer.put_c(static_cast<const unsigned char>(UpperTesterRtcmemCodec::c_utRtcmemEventIndication));
  BITSTRING   bs = LibItsRtcmem__EncdecDeclarations::fx__enc__RTCMEM(send_par.rtcmemMsg());
  OCTETSTRING os = bit2oct(bs);
  OCTETSTRING l  = int2oct(os.lengthof(), 2);
  encoding_buffer.put_s(l.lengthof(), static_cast<const unsigned char *>(l));
  encoding_buffer.put_s(os.lengthof(), static_cast<const unsigned char *>(os));
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  send_data(data, _params);
}

void ConfigRsuSimulatorLayer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::send_data: ", data);

  send_to_all_layers(data, params);
}

void ConfigRsuSimulatorLayer::receive_data(OCTETSTRING &data, params &params) { // FIXME To be moved into ConfigRsuSimulatorCodec
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::receive_data: ", data);

  unsigned char id = *(static_cast<const unsigned char *>(data));
  if (id == 0x00) { // Receive an UtxxInitialise
    process_utinitialize_data(data, params);
  } else if (id == uppertester_cam_codec::c_utCamChangeCurvature) {
    process_ut_cam_changecurvature_data(data, params);
  } else if (id == uppertester_cam_codec::c_utCamChangeSpeed) {
    process_ut_cam_changespeed_data(data, params);
  } else if (id == uppertester_cam_codec::c_utCamChangeHeading) {
    process_ut_cam_changeheading_data(data, params);
  } else if (id == uppertester_cam_codec::c_utCamSetVehicleRole) {
    process_ut_cam_setvehiclerole_data(data, params);
  } else if (id == uppertester_denm_codec::c_utDenmTrigger) {
    process_ut_denm_trigger(data, params);
  } else if (id == uppertester_denm_codec::c_utDenmUpdate) {
    process_ut_denm_update(data, params);
  } else if (id == uppertester_denm_codec::c_utDenmTermination) {
    process_ut_denm_termination(data, params);
  } else if (id == uppertester_mapem_spatem_codec::c_utMapemSpatemTrigger) {
    process_ut_mapem_spatem_trigger(data, params);
  } else if (id == uppertester_ivim_codec::c_utIvimTrigger) {
    process_ut_ivim_trigger(data, params);
  } else if (id == uppertester_ivim_codec::c_utIvimUpdate) {
    process_ut_ivim_update(data, params);
  } else if (id == uppertester_ivim_codec::c_utIvimTermination) {
    process_ut_ivim_termination(data, params);
  } else if (id == UpperTesterSremSsemCodec::c_utSremTrigger) {
    process_ut_srem_trigger(data, params);
  } else if (id == UpperTesterSremSsemCodec::c_utSremUpdate) {
    process_ut_srem_update(data, params);
  } else if (id == UpperTesterSremSsemCodec::c_utSremCancelation) {
    process_ut_srem_termination(data, params);
  } else if (id == UpperTesterRtcmemCodec::c_utRtcmemTrigger) {
    process_ut_rtcmem_trigger(data, params);
  } else if (id == UpperTesterRtcmemCodec::c_utRtcmemUpdate) {
    process_ut_rtcmem_update(data, params);
  } else if (id == UpperTesterRtcmemCodec::c_utRtcmemTermination) {
    process_ut_rtcmem_termination(data, params);
  } else if ((id >= uppertester_geonetworking_codec::c_utGnTrigger_GeoUnicast) &&
             (id <= uppertester_geonetworking_codec::c_utGnTrigger_Tsb)) { // Receive an UtGnTrigger
    process_ut_geonetworking_trigger(data, params);
  } else if ((id == uppertester_pki_codec::c_utPkiTriggerEnrolmentRequest) ||
             (id == uppertester_pki_codec::c_utPkiTriggerAuthorizationRequest)) { // Receive an UtPkiTrigger
    process_ut_pki_trigger(data, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::receive_data: Unsupported tag %02x", id);
  }
}

int ConfigRsuSimulatorLayer::process_utinitialize_data(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_utinitialize_data");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_utinitialize_data: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("gn") == 0) {
    OCTETSTRING                                         hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsGeoNetworking__TypesAndValues::UtGnInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("cam") == 0) {
    OCTETSTRING                                hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsCam__TypesAndValues::UtCamInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("denm") == 0) {
    OCTETSTRING                                  hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsDenm__TypesAndValues::UtDenmInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("pki") == 0) {
    OCTETSTRING                                hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsPki__TypesAndValues::UtPkiInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("ivim") == 0) {
    OCTETSTRING                                  hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsIvim__TypesAndValues::UtIvimInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("mapemspatem") == 0) {
    OCTETSTRING                                                hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("sremssem") == 0) {
    OCTETSTRING                                      hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsSremSsem__TypesAndValues::UtSremInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else if (_params[std::string("ut")].compare("rtcmem") == 0) {
    OCTETSTRING                                      hashedId(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsRtcmem__TypesAndValues::UtRtcmemInitialize p(hashedId);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_utinitialize_data: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_cam_changespeed_data(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_cam_changespeed_data");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changespeed_data: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("cam") == 0) {
    OCTETSTRING                             speed(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsCam__TypesAndValues::UtCamTrigger p;
    p.changeSpeed() = oct2int(speed);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changespeed_data: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_cam_changecurvature_data(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_cam_changecurvature_data");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changecurvature_data: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("cam") == 0) {
    OCTETSTRING                             curvature(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsCam__TypesAndValues::UtCamTrigger p;
    p.changeCurvature() = oct2int(curvature);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changecurvature_data: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_cam_changeheading_data(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_cam_changeheading_data");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changeheading_data: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("cam") == 0) {
    OCTETSTRING                             heading(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsCam__TypesAndValues::UtCamTrigger p;
    p.changeHeading() = oct2int(heading);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_changeheading_data: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_cam_setvehiclerole_data(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_cam_setvehiclerole_data");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_setvehiclerole_data: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("cam") == 0) {
    OCTETSTRING                             vehicle_role(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    LibItsCam__TypesAndValues::UtCamTrigger p;
    p.setVehicleRole() = oct2int(vehicle_role);
    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_cam_setvehiclerole_data: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_denm_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_denm_trigger", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("denm") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_denm_trigger: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_denm_trigger: flag=%02x", flag);
    LibItsDenm__TypesAndValues::UtDenmTrigger trigger; // See ETSI TR 103 099 Cause C.4.1 GenerateDenmEvent
    // Decode detectionTime
    trigger.detectionTime() = oct2int(OCTETSTRING(6, p));
    p += 6;
    // ValidityDuration
    if ((flag & 0x80) == 0x80) {
      trigger.validityDuration() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      trigger.validityDuration().set_to_omit();
    }
    p += 3;
    if ((flag & 0x40) == 0x40) {
      trigger.repetitionDuration() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      trigger.repetitionDuration().set_to_omit();
    }
    p += 3;
    // Situation
    trigger.situation().informationQuality()       = oct2int(OCTETSTRING(1, p++));
    trigger.situation().eventType().causeCode()    = oct2int(OCTETSTRING(1, p++));
    trigger.situation().eventType().subCauseCode() = oct2int(OCTETSTRING(1, p++));
    trigger.situation().linkedCause().set_to_omit();
    trigger.situation().eventHistory().set_to_omit();
    // RelevanceDistance
    trigger.relevanceDistance() = oct2int(OCTETSTRING(1, p++));
    // RelevanceTrafficDirection
    trigger.relevanceTrafficDirection() = oct2int(OCTETSTRING(1, p++));
    if ((flag & 0x04) == 0x04) {
      trigger.transmissionInterval() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(2, p)));
    } else {
      trigger.transmissionInterval().set_to_omit();
    }
    p += 2;
    if ((flag & 0x02) == 0x02) {
      trigger.repetitionInterval() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(2, p)));
    } else {
      trigger.repetitionInterval().set_to_omit();
    }
    p += 2;
    // alacarteLength
    INTEGER alacarte_length = oct2int(OCTETSTRING(1, p));
    if (static_cast<int>(alacarte_length) != 0) {
      p += 1;
      // FIXME AlacarteContainer alacarte optional
    } else {
      trigger.alacarte().set_to_omit();
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_denm_trigger: ", trigger);

    // Pass it to the ports if any
    to_all_upper_ports(trigger, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_denm_update(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_denm_update", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_update: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("denm") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_denm_update: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_denm_update: flag=%02x", flag);
    LibItsDenm__TypesAndValues::UtDenmUpdate update; // See ETSI TR 103 099 Cause C.4.1 GenerateDenmEvent
    // Statuion ID
    update.actionId().originatingStationID() = oct2int(OCTETSTRING(4, p));
    p += 4;
    // SequenceNumber
    update.actionId().sequenceNumber() = oct2int(OCTETSTRING(2, p));
    p += 2;
    // Decode detectionTime
    update.detectionTime() = oct2int(OCTETSTRING(6, p));
    p += 6;
    if ((flag & 0x80) == 0x80) {
      update.validityDuration() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      update.validityDuration().set_to_omit();
    }
    p += 3;
    // Situation
    if ((flag & 0x40) == 0x40) {
      DENM__PDU__Descriptions::SituationContainer &s = static_cast<DENM__PDU__Descriptions::SituationContainer &>(*update.get_opt_value());
      s.informationQuality()                         = oct2int(OCTETSTRING(1, p++));
      s.eventType().causeCode()                      = oct2int(OCTETSTRING(1, p++));
      s.eventType().subCauseCode()                   = oct2int(OCTETSTRING(1, p++));
      s.linkedCause().set_to_omit();
      s.eventHistory().set_to_omit();
      update.situation() = OPTIONAL<DENM__PDU__Descriptions::SituationContainer>(s);
    } else {
      update.situation().set_to_omit();
    }
    // Location
    update.location().set_to_omit();
    // RelevanceDistance
    if ((flag & 0x20) == 0x20) {
      update.relevanceDistance() = oct2int(OCTETSTRING(1, p++));
    } else {
      update.relevanceDistance().set_to_omit();
    }
    // RelevanceTrafficDirection
    if ((flag & 0x10) == 0x10) {
      update.relevanceTrafficDirection() = oct2int(OCTETSTRING(1, p++));
    } else {
      update.relevanceTrafficDirection().set_to_omit();
    }
    if ((flag & 0x08) == 0x08) {
      update.transmissionInterval() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(2, p)));
    } else {
      update.transmissionInterval().set_to_omit();
    }
    p += 2;
    if ((flag & 0x04) == 0x04) {
      update.repetitionInterval() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(2, p)));
    } else {
      update.repetitionInterval().set_to_omit();
    }
    p += 2;
    // alacarteLength
    INTEGER alacarte_length = oct2int(OCTETSTRING(1, p));
    if (static_cast<int>(alacarte_length) != 0) {
      p += 1;
      // FIXME AlacarteContainer alacarte optional
    } else {
      update.alacarte().set_to_omit();
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_denm_update: ", update);

    // Pass it to the ports if any
    to_all_upper_ports(update, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_update: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_denm_termination(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_denm_termination");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_termination: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("denm") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_denm_termination: msg_id=%02x", msg_id);
    LibItsDenm__TypesAndValues::UtDenmTermination termination;
    termination.actionId().originatingStationID() = oct2int(OCTETSTRING(4, p));
    p += 4;
    termination.actionId().sequenceNumber() = oct2int(OCTETSTRING(2, p));

    // Pass it to the ports if any
    to_all_upper_ports(termination, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_denm_termination: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("gn") == 0) {
    unsigned char msg_id = *static_cast<const unsigned char *>(data);
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: msg_id=%02x", msg_id);
    OCTETSTRING payload(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: payload:", payload);
    TTCN_EncDec::clear_error();
    TTCN_Buffer decoding_buffer(payload);

    LibItsGeoNetworking__TypesAndValues::UtGnTrigger p;
    if (msg_id == uppertester_geonetworking_codec::c_utGnTrigger_GeoUnicast) {
      LibItsGeoNetworking__TypesAndValues::GenerateGeoUnicastMessage g;
      g.decode(*g.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      if (g.payload().lengthof() > 2) {
        g.payload() = OCTETSTRING(g.payload().lengthof() - 2, static_cast<const unsigned char *>(g.payload()));
      } else {
        g.payload() = OCTETSTRING(0, nullptr);
      }
      p.geoUnicast() = g;
    } else if (msg_id == uppertester_geonetworking_codec::c_utGnTrigger_GeoBroadcast) {
      LibItsGeoNetworking__TypesAndValues::GenerateGeoBroadcastMessage g;
      g.decode(*g.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      if (g.payload().lengthof() > 2) {
        g.payload() = OCTETSTRING(g.payload().lengthof() - 2, static_cast<const unsigned char *>(g.payload()));
      } else {
        g.payload() = OCTETSTRING(0, nullptr);
      }
      p.geoBroadcast() = g;
    } else if (msg_id == uppertester_geonetworking_codec::c_utGnTrigger_GeoAnycast) {
      LibItsGeoNetworking__TypesAndValues::GenerateGeoAnycastMessage g;
      g.decode(*g.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      if (g.payload().lengthof() > 2) {
        g.payload() = OCTETSTRING(g.payload().lengthof() - 2, static_cast<const unsigned char *>(g.payload()));
      } else {
        g.payload() = OCTETSTRING(0, nullptr);
      }
      p.geoAnycast() = g;
    } else if (msg_id == uppertester_geonetworking_codec::c_utGnTrigger_Shb) {
      LibItsGeoNetworking__TypesAndValues::GenerateSHBMessage g;
      g.decode(*g.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      if (g.payload().lengthof() > 2) {
        g.payload() = OCTETSTRING(g.payload().lengthof() - 2, static_cast<const unsigned char *>(g.payload()));
      } else {
        g.payload() = OCTETSTRING(0, nullptr);
      }
      p.shb() = g;
    } else if (msg_id == uppertester_geonetworking_codec::c_utGnTrigger_Tsb) {
      LibItsGeoNetworking__TypesAndValues::GenerateTSBMessage g;
      g.decode(*g.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      if (g.payload().lengthof() > 2) {
        g.payload() = OCTETSTRING(g.payload().lengthof() - 2, static_cast<const unsigned char *>(g.payload()));
      } else {
        g.payload() = OCTETSTRING(0, nullptr);
      }
      p.tsb() = g;
    }

    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_pki_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_pki_trigger");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_pki_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("pki") == 0) {
    unsigned char msg_id = *static_cast<const unsigned char *>(data);
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: msg_id=%02x", msg_id);
    OCTETSTRING payload(data.lengthof() - 1, 1 + static_cast<const unsigned char *>(data));
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: payload:", payload);
    TTCN_EncDec::clear_error();
    TTCN_Buffer decoding_buffer(payload);

    LibItsPki__TypesAndValues::UtPkiTrigger p;
    if (msg_id == uppertester_pki_codec::c_utPkiTriggerEnrolmentRequest) {
      // LibItsPki__TypesAndValues::TriggerEnrolmentRequest e;
      // e.decode(*e.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      // p.triggerEnrolmentRequest() = e;
      p.triggerEnrolmentRequest().cannonicalId()        = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerEnrolmentRequest().encAlgorithm()        = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerEnrolmentRequest().privateKey()          = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerEnrolmentRequest().compressedPublickey() = OCTETSTRING(0, nullptr); // No parameter used
    } else if (msg_id == uppertester_pki_codec::c_utPkiTriggerAuthorizationRequest) {
      // LibItsPki__TypesAndValues::TriggerAuthorizationRequest e;
      // e.decode(*e.get_descriptor(), decoding_buffer, TTCN_EncDec::CT_RAW);
      // p.triggerAuthorizationRequest() = e;
      p.triggerAuthorizationRequest().cannonicalId()        = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerAuthorizationRequest().encAlgorithm()        = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerAuthorizationRequest().privateKey()          = OCTETSTRING(0, nullptr); // No parameter used
      p.triggerAuthorizationRequest().compressedPublickey() = OCTETSTRING(0, nullptr); // No parameter used
    }

    // Pass it to the ports if any
    to_all_upper_ports(p, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_pki_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("mapemspatem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: flag=%02x", flag);
    LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger trigger; // See ETSI TR 103 099 Cause C.4.1 GenerateMapemSpatemEvent
    // Decode Event
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: event=%02x", *p);
    trigger.event().int2enum(static_cast<int>(*p++));
    // Decode IntersectionId
    if ((flag & 0x80) == 0x80) {
      loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: intersectionID=%04x", (short)*p);
      trigger.intersectionID() = OPTIONAL<INTEGER>(static_cast<int>((*p << 8) | *(p + 1)));
    } else {
      trigger.intersectionID().set_to_omit();
    }
    p += 2;
    // Decode RegulatorySpeedLimit
    if ((flag & 0x40) == 0x40) {
      DSRC__noCircular::RegulatorySpeedLimit r;
      r.type__().int2enum(static_cast<int>(*p++));
      r.speed() = static_cast<int>((*p << 8) | *(p + 1));
      loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: RegulatorySpeedLimit: ", r);
      trigger.regulatorySpeedLimit() = OPTIONAL<DSRC__noCircular::RegulatorySpeedLimit>(r);
    } else {
      trigger.regulatorySpeedLimit().set_to_omit();
    }
    p += 2;
    // FIXME Add flags
    //    trigger.latitude().set_to_omit();
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: ", trigger);

    // Pass it to the ports if any
    to_all_upper_ports(trigger, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_mapem_spatem_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_ivim_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_ivim_trigger", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("ivim") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_ivim_trigger: msg_id=%02x", msg_id);
    unsigned short flag = static_cast<unsigned short>((*p++) << 8 | (*p++));
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_ivim_trigger: flag=%02x", flag);
    LibItsIvim__TypesAndValues::UtIvimTrigger trigger; // See ETSI TR 103 099 Cause C.4.1 GenerateIvimEvent
    // Decode ValidityFrom
    if ((flag & 0x8000) == 0x8000) {
      trigger.validFrom() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      trigger.validFrom().set_to_omit();
    }
    p += 6;
    // Decode ValidityTo
    if ((flag & 0x4000) == 0x4000) {
      trigger.validTo() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      trigger.validTo().set_to_omit();
    }
    p += 6;
    // Decode RepetitionInterval
    if ((flag & 0x2000) == 0x2000) {
      trigger.repetitionInterval() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      trigger.repetitionInterval().set_to_omit();
    }
    p += 6;
    // Decode zoneIds
    if ((flag & 0x1000) == 0x1000) {
      PreGenRecordOf::PREGEN__RECORD__OF__INTEGER rov;
      rov.set_size(*p++);
      for (int i = 0; i < rov.size_of(); i++) {
        rov[i] = oct2int(OCTETSTRING(1, p++));
      }
      trigger.zoneIds() = OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER>(rov);
    } else {
      trigger.zoneIds().set_to_omit();
      p += 1;
    }
    // Decode laneNumber
    if ((flag & 0x0800) == 0x0800) {
      trigger.laneNumber() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(1, p)));
    } else {
      trigger.laneNumber().set_to_omit();
    }
    p += 1;
    // Decode relavanceZoneIds
    if ((flag & 0x0400) == 0x0400) {
      PreGenRecordOf::PREGEN__RECORD__OF__INTEGER rov;
      rov.set_size(*p++);
      for (int i = 0; i < rov.size_of(); i++) {
        rov[i] = oct2int(OCTETSTRING(1, p++));
      }
      trigger.relavanceZoneIds() = OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER>(rov);
    } else {
      trigger.relavanceZoneIds().set_to_omit();
      p += 1;
    }
    // Decode detectionZoneIds
    if ((flag & 0x0200) == 0x0200) {
      PreGenRecordOf::PREGEN__RECORD__OF__INTEGER rov;
      rov.set_size(*p++);
      for (int i = 0; i < rov.size_of(); i++) {
        rov[i] = oct2int(OCTETSTRING(1, p++));
      }
      trigger.detectionZoneIds() = OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER>(rov);
    } else {
      trigger.detectionZoneIds().set_to_omit();
      p += 1;
    }
    // Decode driverAwarenesZoneIds
    if ((flag & 0x0100) == 0x0100) {
      PreGenRecordOf::PREGEN__RECORD__OF__INTEGER rov;
      rov.set_size(*p++);
      for (int i = 0; i < rov.size_of(); i++) {
        rov[i] = oct2int(OCTETSTRING(1, p++));
      }
      trigger.driverAwarenesZoneIds() = OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER>(rov);
    } else {
      trigger.driverAwarenesZoneIds().set_to_omit();
      p += 1;
    }
    if ((flag & 0x0080) == 0x0080) {
      CITSapplMgmtIDs::VarLengthNumber v;
      v.content()       = oct2int(OCTETSTRING(1, p));
      trigger.itsRrid() = OPTIONAL<CITSapplMgmtIDs::VarLengthNumber>(v);
    } else {
      trigger.itsRrid().set_to_omit();
    }
    p += 1;
    if ((flag & 0x0040) == 0x0040) {
      trigger.directionGic() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(1, p)));
    } else {
      trigger.directionGic().set_to_omit();
    }
    p += 1;
    if ((flag & 0x0020) == 0x0020) {
      trigger.layoutId() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(1, p)));
    } else {
      trigger.layoutId().set_to_omit();
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_ivim_trigger: ", trigger);

    // Pass it to the ports if any
    to_all_upper_ports(trigger, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_ivim_update(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_ivim_update", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_update: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("ivim") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_ivim_update: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_ivim_update: flag=%02x", flag);
    LibItsIvim__TypesAndValues::UtIvimUpdate update; // See ETSI TR 103 099 Cause C.4.1 GenerateIvimEvent
    // IviIdentificationNumber
    update.iviIdentificationNumber() = oct2int(OCTETSTRING(2, p));
    p += 2;
    // validFrom
    if ((flag & 0x80) == 0x80) {
      update.validFrom() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      update.validFrom().set_to_omit();
    }
    p += 6;
    // validTo
    if ((flag & 0x40) == 0x40) {
      update.validTo() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      update.validTo().set_to_omit();
    }
    p += 6;
    // timeStamp
    if ((flag & 0x20) == 0x20) {
      update.timeStamp() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(6, p)));
    } else {
      update.timeStamp().set_to_omit();
    }
    p += 6;
    // connectedIviStructures
    if ((flag & 0x01) == 0x01) {
      PreGenRecordOf::PREGEN__RECORD__OF__INTEGER rov;
      rov.set_size(*p++);
      for (int i = 0; i < rov.size_of(); i++) {
        rov[i] = oct2int(OCTETSTRING(4, p));
        p += 4;
      }
      update.connectedIviStructures() = OPTIONAL<PreGenRecordOf::PREGEN__RECORD__OF__INTEGER>(rov);
    } else {
      update.connectedIviStructures().set_to_omit();
      p += 1;
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_ivim_update: ", update);

    // Pass it to the ports if any
    to_all_upper_ports(update, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_update: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_ivim_termination(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_ivim_termination");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_termination: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("ivim") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_ivim_termination: msg_id=%02x", msg_id);
    LibItsIvim__TypesAndValues::UtIvimTermination termination;
    termination.iviIdentificationNumber() = oct2int(OCTETSTRING(2, p));

    // Pass it to the ports if any
    to_all_upper_ports(termination, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_ivim_termination: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_srem_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_srem_trigger");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("sremssem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_geonetworking_trigger: msg_id=%02x", msg_id);
    LibItsSremSsem__TypesAndValues::UtSremTrigger trigger; // See ETSI TR 103 099
    // BasicVehicleRole
    trigger.basicVehicleRole() = oct2int(OCTETSTRING(1, p++));
    // Request Importance Level
    trigger.requestImportanceLevel() = oct2int(OCTETSTRING(1, p++));
    // Intersection ID
    trigger.intersectionId() = oct2int(OCTETSTRING(1, p++));

    // Pass it to the ports if any
    to_all_upper_ports(trigger, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_srem_update(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_srem_update");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_update: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("sremssem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_geonetworking_update: msg_id=%02x", msg_id);
    LibItsSremSsem__TypesAndValues::UtSremUpdate update; // See ETSI TR 103 099
    // RequestID
    update.requestID() = oct2int(OCTETSTRING(1, p++));
    // BasicVehicleRole
    update.basicVehicleRole() = oct2int(OCTETSTRING(1, p++));
    // Request Importance Level
    update.requestImportanceLevel() = oct2int(OCTETSTRING(1, p++));

    // Pass it to the ports if any
    to_all_upper_ports(update, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_update: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_srem_termination(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_srem_termination");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_termination: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("sremssem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_geonetworking_termination: msg_id=%02x", msg_id);
    LibItsSremSsem__TypesAndValues::UtSremCancelation termination; // See ETSI TR 103 099
    // RequestID
    termination.requestID() = oct2int(OCTETSTRING(1, p++));

    // Pass it to the ports if any
    to_all_upper_ports(termination, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_srem_termination: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("rtcmem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger: flag=%02x", flag);
    LibItsRtcmem__TypesAndValues::UtRtcmemTrigger trigger; // See ETSI TR 103 099 Cause C.10.1 GenerateRtcmemEvent
    // Decode msgCnt
    trigger.msgCnt() = *p++;
    // Decode rev
    trigger.rev() = DSRC::RTCM__Revision(static_cast<int>(*p++));
    // Decode msgs
    DSRC::RTCMmessageList msgs;
    unsigned char         num_items = *p++; // Number of items
    msgs.set_size(num_items);
    for (unsigned char i = 0; i < num_items; i++) {
      unsigned char len = *p++; // Length of the message
      msgs[i]           = OCTETSTRING(len, p);
      p += len;
    } // End of 'for' statement
    trigger.msgs() = msgs;
    // timeStamp
    if ((flag & 0x80) == 0x80) {
      // TODO trigger.timeStamp() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      trigger.timeStamp().set_to_omit();
    }
    // TODO p += 3;
    // anchorPoint
    if ((flag & 0x40) == 0x40) {
      // TODO trigger.anchorPoint() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      trigger.anchorPoint().set_to_omit();
    }
    // TODO p += 3;
    // rtcmHeader
    if ((flag & 0x20) == 0x20) {
      // TODO trigger.rtcmHeader() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      trigger.rtcmHeader().set_to_omit();
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger: ", trigger);

    // Pass it to the ports if any
    to_all_upper_ports(trigger, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_trigger: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_rtcmem_update(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> ConfigRsuSimulatorLayer::process_ut_rtcmem_update", data);

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_update: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("rtcmem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_rtcmem_update: msg_id=%02x", msg_id);
    unsigned char flag = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_rtcmem_update: flag=%02x", flag);
    LibItsRtcmem__TypesAndValues::UtRtcmemUpdate update; // See ETSI TR 103 099 Cause C.10.2 GenerateRtcmemUpdate
    // Decode msgs
    DSRC::RTCMmessageList msgs;
    p += 1;                         // Skip COER flag
    unsigned char num_items = *p++; // Number of items
    msgs.set_size(num_items);
    for (unsigned char i = 0; i < num_items; i++) {
      unsigned char len = *p++; // Length of the message
      msgs[i]           = OCTETSTRING(len, p);
      p += len;
    } // End of 'for' statement
    update.msgs() = msgs;
    // ValidityDuration
    if ((flag & 0x80) == 0x80) {
      // TODO update.timeStamp() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      update.timeStamp().set_to_omit();
    }
    // TODO p += 3;
    if ((flag & 0x40) == 0x40) {
      // TODO update.anchorPoint() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      update.anchorPoint().set_to_omit();
    }
    // TODO p += 3;
    if ((flag & 0x20) == 0x20) {
      // TODO update.rtcmHeader() = OPTIONAL<INTEGER>(oct2int(OCTETSTRING(3, p)));
    } else {
      update.rtcmHeader().set_to_omit();
    }
    loggers::get_instance().log_msg("ConfigRsuSimulatorLayer::process_ut_rtcmem_update: ", update);

    // Pass it to the ports if any
    to_all_upper_ports(update, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_update: Unsupported protocol");
    return -1;
  }

  return 0;
}

int ConfigRsuSimulatorLayer::process_ut_rtcmem_termination(const OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> ConfigRsuSimulatorLayer::process_ut_rtcmem_termination");

  params::const_iterator it = _params.find("ut");
  if (it == _params.cend()) {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_termination: CF layer's ut parameter is missing");
    return -1;
  }

  if (_params[std::string("ut")].compare("rtcmem") == 0) {
    unsigned char *p      = (unsigned char *)static_cast<const unsigned char *>(data);
    unsigned char  msg_id = *p++;
    loggers::get_instance().log("ConfigRsuSimulatorLayer::process_ut_rtcmem_termination: msg_id=%02x", msg_id);
    LibItsRtcmem__TypesAndValues::UtRtcmemTermination termination;
    termination.msgCnt() = static_cast<int>(*p);

    // Pass it to the ports if any
    to_all_upper_ports(termination, params);
  } else {
    loggers::get_instance().warning("ConfigRsuSimulatorLayer::process_ut_rtcmem_termination: Unsupported protocol");
    return -1;
  }

  return 0;
}

ConfigRsuSimulatorLayerFactory ConfigRsuSimulatorLayerFactory::_f;
