#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>

#include "geonetworking_types.hh"

#include "geonetworking_layer_factory.hh"

#include "loggers.hh"

#include "security_services.hh"

#include "base_time.hh"

#include "registration.hh"

#include "converter.hh"

using namespace LibItsGeoNetworking__TypesAndValues;

geonetworking_layer::geonetworking_layer(const std::string &p_type, const std::string &p_param)
  : t_layer<LibItsGeoNetworking__TestSystem::GeoNetworkingPort>(p_type), _params(), _codec(), _beacon(nullptr), _gbc_packet(nullptr), _shb_packet(nullptr),
    _tsb_packet(nullptr), _uni_packet(nullptr), _ls_reply(nullptr), _location_table(),
    _pass_beacon_table(), _device_mode{false}, _secured_mode{false}, _encrypted_mode{false}, _enable_security_checks{false},
    _sendData(), _timerid{0}, _sev{0}, _its{0}, _freq_nanosecs(0), _mask{0}, _sa{0}, _sequence_number{0}, _latitude{0}, _longitude{0}, _force_certificate{false} {
  loggers::get_instance().log(">>> geonetworking_layer::geonetworking_layer: %s, %s", to_string().c_str(), p_param.c_str());

  init(p_type, p_param);
} // End of constructor

geonetworking_layer::~geonetworking_layer() {
  loggers::get_instance().log(">>> geonetworking_layer::~geonetworking_layer");

  if (_timerid != 0) {
    timer_delete(_timerid);
  }
  if (_beacon != nullptr) {
    delete _beacon;
  }
  if (_gbc_packet != nullptr) {
    delete _gbc_packet;
  }
  if (_shb_packet != nullptr) {
    delete _shb_packet;
  }
  if (_tsb_packet != nullptr) {
    delete _tsb_packet;
  }
  if (_uni_packet != nullptr) {
    delete _uni_packet;
  }
  if (_ls_reply != nullptr) {
    delete _ls_reply;
  }
} // End of destructor

void geonetworking_layer::init(const std::string &p_type, const std::string &p_param) {
  loggers::get_instance().log(">>> geonetworking_layer::init: %s, %s", to_string().c_str(), p_param.c_str());

  // Setup parameters
  params::convert(_params, p_param);
  // Sanity checks
  params::const_iterator it = _params.find(params::latitude);
  if (it != _params.cend()) {
    _latitude = converter::get_instance().string_to_int(it->second);
  }
  it = _params.find(params::longitude);
  if (it != _params.cend()) {
    _longitude = converter::get_instance().string_to_int(it->second);
  }
  OCTETSTRING ll_address;
  it = _params.find(params::ll_address);
  if (it != _params.cend()) {
    ll_address = str2oct(CHARSTRING(it->second.c_str()));
  }
  INTEGER distanceA = 1000; // 1km
  it                = _params.find(params::distanceA);
  if (it != _params.cend()) {
    distanceA = converter::get_instance().string_to_int(it->second);
  }
  INTEGER distanceB = 1000; // 1Km
  it                = _params.find(params::distanceB);
  if (it != _params.cend()) {
    distanceB = converter::get_instance().string_to_int(it->second);
  }
  INTEGER angle = 0;
  it            = _params.find(params::angle);
  if (it != _params.cend()) {
    angle = converter::get_instance().string_to_int(it->second);
  }
  INTEGER station_type = 5; // passangerCar
  it                   = _params.find(params::station_type);
  if (it != _params.cend()) {
    station_type = converter::get_instance().string_to_int(it->second);
  }
  INTEGER country = 0;
  it              = _params.find(params::country);
  if (it != _params.cend()) {
    country = converter::get_instance().string_to_int(it->second);
  }
  INTEGER type_of_address = 1; // Manual
  it                      = _params.find(params::type_of_address);
  if (it != _params.cend()) {
    type_of_address = converter::get_instance().string_to_int(it->second);
  }

  it = _params.find(params::device_mode);
  if (it != _params.cend()) {
    _device_mode = (1 == converter::get_instance().string_to_int(it->second));
  }
  it = _params.find(params::secured_mode);
  if (it != _params.cend()) {
    _secured_mode = (1 == converter::get_instance().string_to_int(it->second));
  } else {
    _params.insert(std::pair<std::string, std::string>(params::secured_mode, "0"));
  }
  it = _params.find(params::encrypted_mode);
  if (it != _params.cend()) {
    _encrypted_mode = (1 == converter::get_instance().string_to_int(it->second));
  } else {
    _params.insert(std::pair<std::string, std::string>(params::encrypted_mode, "0"));
  }
  it = _params.find(params::enable_security_checks);
  if (it != _params.cend()) {
    _enable_security_checks = (1 == converter::get_instance().string_to_int(it->second));
  }

  // Add broadcast address if needed
  it = _params.find(params::its_aid);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::its_aid, "141"));
  }
  it = _params.find(params::mac_bc);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::its_aid, "FFFFFFFFFFFF"));
  }

  // Set up default security parameters value
  if (_secured_mode || _encrypted_mode) {
    loggers::get_instance().log("geonetworking_layer::init: Setup secured mode");
    setup_secured_mode();
  }

  // Automatic beaconing mode
  fill_beacon(ll_address, station_type, country, type_of_address);
  params::const_iterator i = _params.find(params::beaconing);
  if ((i != _params.cend()) && (i->second.compare("1") == 0)) { // Immediate beaconing was requested
    // Prepare beaconing operation
    start_beaconing();
  }

  // Fill packet templates
  fill_gbc_packet(ll_address, _latitude, _longitude, distanceA, distanceB,
                  angle); // TODO Check if GeoBroadcastArea lat/lon are identical to lat/lon of the Test System
  fill_shb_packet(ll_address);
  fill_tsb_packet(ll_address);
  fill_uni_packet(ll_address);
  fill_ls_reply(ll_address);

  // Register this object for AdapterControlPort
  loggers::get_instance().log("geonetworking_layer::init: Register %s/%p", p_type.c_str(), this);
  registration<geonetworking_layer>::get_instance().add_item(p_type, this);

  // Add 4 leap seconds to convert to TAI (as Feb 2019)
  base_time::get_instance().set_leap_delay_us(4 * 1000000); // TODO Set it as parameter
} // End of init_params

void geonetworking_layer::sendMsg(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingReq &p, params &p_params) {
  loggers::get_instance().log(">>> geonetworking_layer::sendMsg");

  params param(_params);
  // Encode GeoNetworking PDU
  OCTETSTRING data;
  _codec.encode(p.msgOut(), data);
  params::const_iterator it = param.find(params::its_aid);
  if (it != param.cend()) {
    int aid = std::stoi(param[params::its_aid]);
    loggers::get_instance().log("geonetworking_layer::sendMsg: aid=%d/%d", aid, (int)p.its__aid());
    if (aid != (int)p.its__aid()) {
      param[params::its_aid] = std::to_string((int)p.its__aid());
      loggers::get_instance().log("geonetworking_layer::sendMsg: New aid=%s", param[params::its_aid].c_str());
    }
  } else{
    param.insert(std::pair<std::string, std::string>(params::its_aid, std::to_string((int)p.its__aid())));
  }
  if (_force_certificate == true) { // Force certificate instead of hashed_id in next message
    _force_certificate = false;
    param.insert(std::pair<std::string, std::string>(params::force_certificate, std::string("1")));
  }
  loggers::get_instance().log("geonetworking_layer::sendMsg: _params");
  _params.log();
  loggers::get_instance().log("geonetworking_layer::sendMsg: params");
  param.log();
  send_data(data, param);
}

void geonetworking_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::send_data: ", data);
  params.log();

  if (_device_mode) { // Need to build a GN packet
    params[params::certificate] = _params[params::certificate];
    params[params::hash]        = _params[params::hash]; // TODO Should be removed
    if (build_geonetworking_pdu(data, params) != 0) {
      return;
    }
  }
  if (_secured_mode) { // Add Security support
    if (build_secured_pdu(data, params) != 0) {
      return;
    }
  }

  // TODO To be removed
  while (_sendData.try_lock() == FALSE) {
    // not ready yet
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  } // End of 'while' statement
  send_to_all_layers(data, params);
  _sendData.unlock();
  loggers::get_instance().log("<<< geonetworking_layer::send_data");
}

const TTCN_RAWdescriptor_t  _intx_raw_   = {RAW_INTX,          SG_NO, ORDER_MSB, ORDER_MSB, ORDER_LSB, ORDER_MSB, EXT_BIT_NO, ORDER_LSB, ORDER_LSB,
                                         TOP_BIT_INHERITED, 0,     0,         0,         8,         0,         NULL,       -1,        CharCoding::UNKNOWN};
const TTCN_Typedescriptor_t _intx_descr_ = {"IntX", NULL, &_intx_raw_, NULL, NULL, NULL, NULL, NULL, TTCN_Typedescriptor_t::DONTCARE};

void geonetworking_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::receive_data: ", data);

  // Check security mode
  Ieee1609Dot2::Ieee1609Dot2Data                   ieee_1609dot2_data;
  LibItsGeoNetworking__TypesAndValues::BasicHeader basic_header;
  decode_basic_header(data, basic_header);
  if ((int)basic_header.nextHeader() == 2) { // Verify and extract the GeoNetworking Secured Packet as specified in ETSI EN 302 636-4-1 V1.3.1 (2017-08)
                                             // Clause 9.6.1 Composition of the Basic Header
    unsigned int basic_header_len = 4;       // FIXME How to retrive the BasicHeader length basic_header.get_descriptor()->raw->fieldlength / 8;
    loggers::get_instance().log("geonetworking_layer::receive_data: basic_header_len = %d", basic_header_len);
    // Verify and extract the GeoNetworking Secured Packet as specified in ETSI EN 302 636-4-1 V1.3.1 (2017-08) Clause 9.4 GeoNetworking Secured Packet
    OCTETSTRING unsecured_gn_payload;
    OCTETSTRING secured_data = OCTETSTRING(data.lengthof() - basic_header_len, static_cast<const unsigned char *>(data) + basic_header_len);
    ///////////////////
    // FIXME Check what to do with this!
    if (*static_cast<const unsigned char *>(secured_data) != 0x03) {
      loggers::get_instance().warning("geonetworking_layer::receive_data: Security error, wrong protocol number, discard it anyway");
      if (_enable_security_checks) {
        return;
      }
      return;
    } else {
      if (security_services::get_instance().verify_and_extract_gn_payload(secured_data, _enable_security_checks, ieee_1609dot2_data, unsecured_gn_payload,
                                                                          params) != 0) {
        loggers::get_instance().warning("geonetworking_layer::receive_data: Security error");
        if (_enable_security_checks) {
          return;
        }
      }
    }
    // Update data
    loggers::get_instance().log_msg("geonetworking_layer::receive_data: Unsecured payload: ", unsecured_gn_payload);
    data = OCTETSTRING(basic_header_len, static_cast<const unsigned char *>(data)) + unsecured_gn_payload;
  }

  // Decode the payload
  loggers::get_instance().log_msg("geonetworking_layer::receive_data: Geonetworking payload to decode: ", data);
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingInd ind;
  _codec.decode(data, ind.msgIn(), &params);
  if (ind.msgIn().is_bound()) {
    // Update optional securedMsg field if required
    if (ieee_1609dot2_data.is_bound()) {
      ind.msgIn().gnPacket().securedMsg() = OPTIONAL<Ieee1609Dot2::Ieee1609Dot2Data>(ieee_1609dot2_data);
    } // else, nothing to do
    // Update context
    const LibItsGeoNetworking__TypesAndValues::LongPosVector *     sopv = nullptr;
    const LibItsGeoNetworking__TypesAndValues::GnNonSecuredPacket &p    = ind.msgIn().gnPacket().packet();
    const LibItsGeoNetworking__TypesAndValues::HeaderTST &         htst = p.commonHeader().headerTST();
    if (p.extendedHeader().ispresent()) { // Update location table
      const LibItsGeoNetworking__TypesAndValues::ExtendedHeader &ex = p.extendedHeader();
      if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_beaconHdr)) { // Receive a beacon
        sopv = &ex.beaconHeader().srcPosVector();
      } else if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_tsbHdr)) { // Receive a topologicallyScopeBroadcast
        if (ex.ischosen(LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_tsbHeader)) {
          sopv = &ex.tsbHeader().srcPosVector();
        } else {
          sopv = &ex.shbHeader().srcPosVector();
        }
      } else if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_geoBroadcastHdr)) {
        sopv = &ex.geoBroadcastHeader().srcPosVector();
      } else if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_lsHdr)) {         // Receive a location service
        if (ex.ischosen(LibItsGeoNetworking__TypesAndValues::ExtendedHeader::ALT_lsRequestHeader)) { // Receive a LocationService/LsRequest
          sopv = &ex.lsRequestHeader().srcPosVector();
          // TODO Send LsReply if we are not in context of GN ATS in case of non GN test suite
          if (_device_mode) {
            // Update _ls_reply
            ExtendedHeader *eh = static_cast<ExtendedHeader *>(_ls_reply->gnPacket().packet().extendedHeader().get_opt_value());
            if (eh != nullptr) {
              // Update sequence number
              eh->lsReplyHeader().seqNumber() = _sequence_number++;
              // Update destination
              eh->lsReplyHeader().dstPosVector().gnAddr()    = sopv->gnAddr();
              eh->lsReplyHeader().dstPosVector().latitude()  = sopv->latitude();
              eh->lsReplyHeader().dstPosVector().longitude() = sopv->longitude();
              // Update timestamp
              eh->lsReplyHeader().srcPosVector().timestamp__().set_long_long_val(base_time::get_instance().get_its_current_time_mod_ms());
              eh->lsReplyHeader().dstPosVector().timestamp__() = eh->lsReplyHeader().srcPosVector().timestamp__();

              loggers::get_instance().log_msg("geonetworking_layer::receive_data: ", *_ls_reply);

              // send it
              // Encode GeoNetworking PDU
              OCTETSTRING os;
              _codec.encode(*_ls_reply, os);
              // Apply signature
              if (_secured_mode) {
                if (build_secured_pdu(data, _params) != 0) {
                  return;
                }
              }
              // Send it
              // TODO To be removed
              while (_sendData.try_lock() == FALSE) {
                // not ready yet
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
              } // End of 'while' statement
              send_to_all_layers(os, params);
              _sendData.unlock();
            } else {
              loggers::get_instance().error("geonetworking_layer::send_data: Wrong cast");
              return;
            }
          }
        } else {
          sopv = &ex.lsReplyHeader().srcPosVector();
        }
      } else if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_geoAnycastHdr)) { // Receive a GeoAnycast
        sopv = &ex.geoAnycastHeader().srcPosVector();
      } else if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_geoUnicastHdr)) {
        sopv = &ex.geoUnicastHeader().srcPosVector();
      } // else, nothing to do
      loggers::get_instance().log("geonetworking_layer::receive_data: sopv is bound: %d", sopv->is_bound());
      if (sopv->is_bound()) {
        const LibItsGeoNetworking__TypesAndValues::LongPosVector &lpv = *sopv;
        if (_location_table.add_entry(lpv) == 1) {
          // Force to send new certificate to anticipate certificate distribution process with the new ITS-S
          _force_certificate = true;
        }
      }
    }

    // By default incoming beacons are filtered by the test adapter
    if (htst.ischosen(LibItsGeoNetworking__TypesAndValues::HeaderTST::ALT_beaconHdr)) {
      loggers::get_instance().log_msg("geonetworking_layer::receive_data: Pass beaconing filtering: ", sopv->gnAddr().mid());
      if (_pass_beacon_table.empty()) { // Discard beacon
        loggers::get_instance().log("geonetworking_layer::receive_data: Pass beaconing table empty, discard it");
        return;
      } else {                                                     // Check beacon filter for StartPassBeaconing/Stop
        if (!_pass_beacon_table.has_entry(sopv->gnAddr().mid())) { // Discard beacon
          loggers::get_instance().log_msg("geonetworking_layer::receive_data: Not in pass beaconing table, discard it", *sopv);
          return;
        } // else, continue
      }
    } // else, continue
  } else {
    // Inavlid GeoNetworking payload, discard it
    loggers::get_instance().warning("geonetworking_layer::receive_data: Failed to decode payload, discard it");
    return;
  }

  // Add lower layers parameters
  // 1. Destination MAC address
  params::const_iterator it = params.find(params::mac_dst);
  if (it != params.cend()) {
    loggers::get_instance().log("geonetworking_layer::receive_data: dst=%s", it->second.c_str());
    ind.macDestinationAddress() = str2oct(CHARSTRING(it->second.c_str()));
  } else {
    ind.macDestinationAddress() = str2oct(CHARSTRING(_params["mac_bc"].c_str()));
  }
  // 2. ssp
  it = params.find(params::ssp);
  if (it != params.cend()) {
    loggers::get_instance().log("geonetworking_layer::receive_data: ssp=%s", it->second.c_str());
    ind.ssp() = oct2bit(str2oct(CHARSTRING(it->second.c_str())));
  } else {
    ind.ssp().set_to_omit();
  }
  // 3. its_aid
  it = params.find(params::its_aid);
  if (it != params.cend()) {
    loggers::get_instance().log("geonetworking_layer::receive_data: its_aid=%s", it->second.c_str());
    ind.its__aid() = std::stoi(it->second.c_str());
  } else {
    ind.its__aid().set_to_omit();
  }

  // Pass the GeoNetworking raw payload to the upper layers if any
  it = params.find(params::gn_payload);
  if (it != params.cend()) {
    loggers::get_instance().log("geonetworking_layer::receive_data: gn_payload=%s", it->second.c_str());
    OCTETSTRING os(str2oct(CHARSTRING(it->second.c_str())));
    receive_to_all_layers(os, params);
  } else {
    loggers::get_instance().warning("geonetworking_layer::receive_data: No payload to pass to upper layers");
  }

  // Pass it to the ports
  to_all_upper_ports(ind, params);
}

OCTETSTRING geonetworking_layer::trigger_ac_event(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_to_hexa(">>> geonetworking_layer::trigger_ac_event: ", data);

  return int2oct(0, 2);
} // End of trigger_ac_event method

void geonetworking_layer::start_beaconing() {
  loggers::get_instance().log(">>> geonetworking_layer::start_beaconing");
  // loggers::get_instance().log_msg("geonetworking_layer::start_beaconing: _beacon=", *_beacon);

  // Establish handler for timer signal
  loggers::get_instance().log("geonetworking_layer::start_beaconing: Establishing handler for signal %d\n", _signal_id);
  _sa.sa_flags     = SA_SIGINFO;
  _sa.sa_sigaction = timer_irq_sigalrm_handler;
  sigemptyset(&_sa.sa_mask);
  if (sigaction(_signal_id, &_sa, nullptr) == -1) {
    loggers::get_instance().error("geonetworking_layer::start_beaconing: Sigaction failure: %d", errno);
  }
  // Block timer signal temporarily
  loggers::get_instance().log("geonetworking_layer::start_beaconing: Blocking signal %d\n", _signal_id);
  sigemptyset(&_mask);
  sigaddset(&_mask, _signal_id);
  if (sigprocmask(SIG_SETMASK, &_mask, nullptr) == -1) {
    loggers::get_instance().error("geonetworking_layer::start_beaconing: Sigprocmask failure: %d", errno);
  }
  // Create the timer
  _sev.sigev_notify          = SIGEV_SIGNAL;
  _sev.sigev_signo           = _signal_id; // Use signal alarm
  _sev.sigev_value.sival_ptr = this;       // The geonetworking_layer object address
  if (timer_create(CLOCK_REALTIME, &_sev, &_timerid) == -1) {
    loggers::get_instance().error("geonetworking_layer::start_beaconing: Timer failure: %d", errno);
  }
  loggers::get_instance().log("geonetworking_layer::start_beaconing: timer ID is 0x%x\n", (long)_timerid);
  // Start the timer
  unsigned int           expiry = 1000; // Default expiry time 1000ms
  params::const_iterator i      = _params.find("expiry");
  if (i != _params.cend()) {
    expiry = static_cast<unsigned int>(std::strtoul(i->second.c_str(), nullptr, 10));
  }
  _freq_nanosecs           = expiry * 1000000;
  _its.it_value.tv_sec     = _freq_nanosecs / 1000000000;
  _its.it_value.tv_nsec    = _freq_nanosecs % 1000000000;
  _its.it_interval.tv_sec  = _its.it_value.tv_sec;
  _its.it_interval.tv_nsec = _its.it_value.tv_nsec;
  if (timer_settime(_timerid, 0, &_its, nullptr) == -1) {
    loggers::get_instance().error("geonetworking_layer::start_beaconing: Sigprocmask failure: %d", errno);
  }
  // Unlock the timer signal, so that timer notification can be delivered
  loggers::get_instance().log("geonetworking_layer::start_beaconing: Unblocking signal %d\n", _signal_id);
  if (sigprocmask(SIG_UNBLOCK, &_mask, nullptr) == -1) {
    loggers::get_instance().error("geonetworking_layer::start_beaconing: Sigprocmask failure: %d", errno);
  }
} // End of start_beaconing method

void geonetworking_layer::start_beaconing(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &p_beacon) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::start_beaconing", p_beacon);

  // Initialize the beacon
  if (_beacon != nullptr) {
    delete _beacon;
  }
  _beacon = new LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu(p_beacon);

  start_beaconing(); // TODO Refined adding a boolean return code
} // End of start_beaconing method

void geonetworking_layer::stop_beaconing() {
  loggers::get_instance().log(">>> geonetworking_layer::stop_beaconing");

  // Block timer signal temporarily
  loggers::get_instance().log("geonetworking_layer::stop_beaconing: Blocking signal %d\n", _signal_id);
  sigemptyset(&_mask);
  sigaddset(&_mask, _signal_id);
  if (sigprocmask(SIG_SETMASK, &_mask, nullptr) == -1) {
    loggers::get_instance().error("geonetworking_layer::stop_beaconing: Sigprocmask failure: %d", errno);
  }
  timer_delete(_timerid);
  _timerid = 0;
} // End of stop_beaconing method

void geonetworking_layer::send_beacon() {
  loggers::get_instance().log(">>> geonetworking_layer::send_beacon");

  ExtendedHeader *eh = static_cast<ExtendedHeader *>(_beacon->gnPacket().packet().extendedHeader().get_opt_value());
  if (eh == nullptr) {
    loggers::get_instance().error("geonetworking_layer::send_beacon: Wrong cast");
  }
  // Update timestamp
  eh->beaconHeader().srcPosVector().timestamp__().set_long_long_val((unsigned int)base_time::get_instance().get_its_current_time_mod_ms());
  // loggers::get_instance().log_msg("geonetworking_layer::send_beacon: ", *_beacon);
  // Encode message using TITAN because of payload in omited
  TTCN_Buffer encoding_buffer;
  _beacon->encode(*(_beacon->get_descriptor()), encoding_buffer, TTCN_EncDec::CT_RAW);
  OCTETSTRING data(encoding_buffer.get_len(), encoding_buffer.get_data());
  params      params(_params);
  if (_secured_mode) { // Apply Security
    if (build_secured_pdu(data, _params) != 0) {
      return;
    }
  }
  // Send it
  // TODO To be removed
  while (_sendData.try_lock() == FALSE) {
    // not ready yet
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  } // End of 'while' statement
  send_to_all_layers(data, params);
  _sendData.unlock();

  // loggers::get_instance().log("<<< geonetworking_layer::send_beacon");
} // End of send_beacon method

void geonetworking_layer::start_pass_beaconing(const LibItsGeoNetworking__TypesAndValues::BeaconHeader &p_beacon) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::start_pass_beaconing", p_beacon);

  const LibItsGeoNetworking__TypesAndValues::LongPosVector &lpv = p_beacon.srcPosVector();
  if (!_pass_beacon_table.has_entry(lpv.gnAddr().mid())) {
    _pass_beacon_table.add_entry(lpv);
  } // TODO Refined adding a boolean return code
} // End of start_pass_beaconing method

void geonetworking_layer::stop_pass_beaconing() {
  loggers::get_instance().log(">>> geonetworking_layer::stop_pass_beaconing");

  _pass_beacon_table.reset();
} // End of stop_pass_beaconing method

int geonetworking_layer::enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security) {
  loggers::get_instance().log(">>> geonetworking_layer::enable_secured_mode: '%s' - %x", p_certificate_id.c_str(), p_enforce_security);

  loggers::get_instance().log("geonetworking_layer::enable_secured_mode: _secured_mode = %x", _secured_mode);
  if (!_secured_mode) {
    loggers::get_instance().log("geonetworking_layer::enable_secured_mode: Setup secured mode");
    _secured_mode = true;
    setup_secured_mode();
  }
  _enable_security_checks   = p_enforce_security;
  params::const_iterator it = _params.find(params::certificate);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(params::certificate, p_certificate_id));
  } else {
    _params[params::certificate] = p_certificate_id;
  }

  loggers::get_instance().log("geonetworking_layer::enable_secured_mode: Certificate to be used: '%s'", _params[params::certificate].c_str());

  return 0;
}

int geonetworking_layer::disable_secured_mode() {
  loggers::get_instance().log(">>> geonetworking_layer::disable_secured_mode");
  _secured_mode           = false;
  _enable_security_checks = false;
  return 0;
}

const LongPosVector *geonetworking_layer::get_lpv(const GN__Address &p_gn_address) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::get_lpv", p_gn_address);

  const LongPosVector *lpv = nullptr;
  if (_location_table.has_entry(p_gn_address.mid())) {
    lpv = _location_table.get_entry(p_gn_address.mid());
  }
  return lpv;
} // End of get_lpv

const LibItsGeoNetworking__TypesAndValues::BasicHeader geonetworking_layer::fill_basic_header() const {
  return LibItsGeoNetworking__TypesAndValues::BasicHeader(1, // GeoNetworking version
                                                          BasicNextHeader(BasicNextHeader::e__commonHeader), 0, Lifetime(4, LtBase(LtBase::e__50ms)), 1);
}

void geonetworking_layer::fill_beacon(const OCTETSTRING &p_ll_address, const INTEGER p_station_type, const INTEGER p_country, const INTEGER type_of_address) {
  _beacon = new GeoNetworkingPdu();
  HeaderTST h;
  h.beaconHdr() = BeaconHeaderType(HeaderType(HeaderType::e__beacon), 0);
  ExtendedHeader eh;
  eh.beaconHeader() =
    BeaconHeader(LongPosVector(GN__Address(TypeOfAddress((TypeOfAddress)type_of_address), StationType((StationType)p_station_type), p_country, p_ll_address), 0,
                               _latitude, _longitude, int2bit(0, 1), 0, 0));
  _beacon->basicHeader() = fill_basic_header();
  _beacon->gnPacket().packet() =
    GnNonSecuredPacket(CommonHeader(NextHeader(NextHeader::e__any), 0, h,
                                    TrafficClass(SCF(SCF::e__scfEnabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0), int2bit(0, 8), 0, 1, 0),
                       OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _beacon->gnPacket().packet().payload().set_to_omit();
  _beacon->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_beacon: beacon value: ", *_beacon);
} // End of fill_beacon method

void geonetworking_layer::fill_gbc_packet(const OCTETSTRING &p_ll_address, const INTEGER &p_geoAreaPosLatitude, const INTEGER &p_geoAreaPosLongitude,
                                          const INTEGER &p_distanceA, const INTEGER &p_distanceB, const INTEGER &p_angle) {
  _gbc_packet = new GeoNetworkingPdu();
  HeaderTST h;
  h.geoBroadcastHdr() =
    GeoBroadcastHeaderType(HeaderType(HeaderType::e__geoBroadcast), HeaderSubTypeGeoBroadcast(HeaderSubTypeGeoBroadcast::e__geoBroadcastElip));
  ExtendedHeader eh;
  eh.geoBroadcastHeader()          = GeoAnycastHeader( // GeoBradcastHeader is identical as GeoAnycastHeader
    0, 0,
    LongPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                              StationType(StationType::e__passengerCar), // TODO Use params
                              33, p_ll_address),
                  0, _latitude, _longitude, int2bit(1, 1), // PAI
                  0, 0),
    p_geoAreaPosLatitude, p_geoAreaPosLongitude, p_distanceA, p_distanceB, p_angle, 0);
  _gbc_packet->basicHeader()       = fill_basic_header();
  _gbc_packet->gnPacket().packet() = GnNonSecuredPacket(
    CommonHeader(NextHeader(NextHeader::e__btpA), 0, h, TrafficClass(SCF(SCF::e__scfDisabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0),
                 int2bit(128, 8), // Mobile stationnary flag set
                 0, 5, 0),
    OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _gbc_packet->gnPacket().packet().payload().set_to_omit();
  _gbc_packet->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_gbc_packet: packet value: ", *_gbc_packet);
} // End of fill_gbc_packet method

void geonetworking_layer::fill_shb_packet(const OCTETSTRING &p_ll_address) {
  _shb_packet = new GeoNetworkingPdu();
  HeaderTST h;
  h.tsbHdr() = TsbHeaderType(HeaderType(HeaderType::e__topologicallyScopedBroadcast), HeaderSubTypeTSB(HeaderSubTypeTSB::e__singleHop));
  ExtendedHeader eh;
  eh.shbHeader()                   = SHBHeader(LongPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                       StationType(StationType::e__passengerCar), // TODO Use params
                                                       33, p_ll_address),
                                           0, _latitude, _longitude, int2bit(1, 1), // PAI
                                           0, 0),
                             0);
  _shb_packet->basicHeader()       = fill_basic_header();
  _shb_packet->gnPacket().packet() = GnNonSecuredPacket(
    CommonHeader(NextHeader(NextHeader::e__btpA), 0, h, TrafficClass(SCF(SCF::e__scfDisabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0),
                 int2bit(128, 8), // Mobile stationnary flag set
                 0, 1, 0),
    OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _shb_packet->gnPacket().packet().payload().set_to_omit();
  _shb_packet->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_shb_packet: packet value: ", *_shb_packet);
} // End of fill_shb_packet method

void geonetworking_layer::fill_tsb_packet(const OCTETSTRING &p_ll_address, const int p_hop_number, const int p_max_hop_limit) {
  _tsb_packet = new GeoNetworkingPdu();
  HeaderTST h;
  h.tsbHdr() = TsbHeaderType(HeaderType(HeaderType::e__topologicallyScopedBroadcast), HeaderSubTypeTSB(HeaderSubTypeTSB::e__multiHop));
  ExtendedHeader eh;
  eh.tsbHeader()                   = TSBHeader(0, 0,
                             LongPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                       StationType(StationType::e__passengerCar), // TODO Use params
                                                       33, p_ll_address),
                                           0, _latitude, _longitude, int2bit(1, 1), // PAI
                                           0, 0));
  _tsb_packet->basicHeader()       = fill_basic_header();
  _tsb_packet->gnPacket().packet() = GnNonSecuredPacket(
    CommonHeader(NextHeader(NextHeader::e__btpA), 0, h, TrafficClass(SCF(SCF::e__scfDisabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0),
                 int2bit(128, 8), // Mobile stationnary flag set
                 0, p_max_hop_limit, 0),
    OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _tsb_packet->gnPacket().packet().payload().set_to_omit();
  _tsb_packet->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_tsb_packet: packet value: ", *_tsb_packet);
} // End of fill_tsb_packet method

void geonetworking_layer::fill_uni_packet(const OCTETSTRING &p_ll_address, const int p_hop_number, const int p_max_hop_limit) {
  _uni_packet = new GeoNetworkingPdu();
  HeaderTST h;
  h.geoUnicastHdr() = GeoUnicastHeaderType(HeaderType(HeaderType::e__geoUnicast), 0);
  ExtendedHeader eh;
  eh.geoUnicastHeader()            = GeoUnicastHeader(0, 0,
                                           LongPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                                     StationType(StationType::e__passengerCar), // TODO Use params
                                                                     33, p_ll_address),
                                                         0, _latitude, _longitude, int2bit(1, 1), // PAI
                                                         0, 0),
                                           ShortPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                                      StationType(StationType::e__passengerCar), // TODO Use params
                                                                      33, p_ll_address),
                                                          0, _latitude + 100, _longitude + 100));
  _uni_packet->basicHeader()       = fill_basic_header();
  _uni_packet->gnPacket().packet() = GnNonSecuredPacket(
    CommonHeader(NextHeader(NextHeader::e__btpA), 0, h, TrafficClass(SCF(SCF::e__scfDisabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0),
                 int2bit(128, 8), // Mobile stationnary flag set
                 0, p_max_hop_limit, 0),
    OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _uni_packet->gnPacket().packet().payload().set_to_omit();
  _uni_packet->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_uni_packet: packet value: ", *_uni_packet);
} // End of fill_uni_packet method

void geonetworking_layer::fill_ls_reply(const OCTETSTRING &p_ll_address) {
  _ls_reply = new GeoNetworkingPdu();
  HeaderTST h;
  h.lsHdr() = LsHeaderType(HeaderType(HeaderType::e__locationService), HeaderSubTypeLs(HeaderSubTypeLs::e__lsReply));
  ExtendedHeader eh;
  eh.lsReplyHeader()             = LSReplyHeader(0, 0,
                                     LongPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                               StationType(StationType::e__passengerCar), // TODO Use params
                                                               33, p_ll_address),
                                                   0, _latitude, _longitude, int2bit(1, 1), // PAI
                                                   0, 0),
                                     ShortPosVector(GN__Address(TypeOfAddress(TypeOfAddress::e__manual),   // TODO Use params
                                                                StationType(StationType::e__passengerCar), // TODO Use params
                                                                33, p_ll_address),
                                                    0, _latitude, _longitude));
  _ls_reply->basicHeader()       = fill_basic_header();
  _ls_reply->gnPacket().packet() = GnNonSecuredPacket(CommonHeader(NextHeader(NextHeader::e__any), 0, h,
                                                                   TrafficClass(SCF(SCF::e__scfDisabled), ChannelOffload(ChannelOffload::e__choffDisabled), 0),
                                                                   int2bit(128, 8), // Mobile stationnary flag set
                                                                   0, 5, 0),
                                                      OPTIONAL<ExtendedHeader>(eh), OPTIONAL<GnRawPayload>());
  _ls_reply->gnPacket().packet().payload().set_to_omit();
  _ls_reply->gnPacket().securedMsg().set_to_omit();
  // loggers::get_instance().log_msg("geonetworking_layer::fill_ls_reply: packet value: ", *_ls_reply);
} // End of fill_ls_reply method

void geonetworking_layer::timer_irq_sigalrm_handler(int p_signal, siginfo_t *p_signal_info, void *p_uc) {
  // loggers::get_instance().log(">>> geonetworking_layer::timer_irq_sigalrm_handler: Caught signal %d", p_signal);

  static_cast<geonetworking_layer *>(p_signal_info->si_value.sival_ptr)->send_beacon();
} // End of method timer_irq_sigalrm_handler

int geonetworking_layer::build_geonetworking_pdu(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> geonetworking_layer::build_geonetworking_pdu");
  // params.log();

  std::string            next_header;
  params::const_iterator it = params.find(params::next_header);
  if (it != params.cend()) {
    next_header = it->second.c_str();
  }
  std::string header_type;
  it = params.find(params::header_type);
  if (it != params.cend()) {
    header_type = it->second.c_str();
  }
  std::string header_sub_type;
  it = params.find(params::header_sub_type);
  if (it != params.cend()) {
    header_sub_type = it->second.c_str();
  }
  loggers::get_instance().log("geonetworking_layer::build_geonetworking_pdu: %s, %s, %s", next_header.c_str(), header_type.c_str(), header_sub_type.c_str());

  if (header_type.compare("tsb") == 0) {
    if (header_sub_type.compare("sh") == 0) { // Use SHB
      ExtendedHeader *eh = static_cast<ExtendedHeader *>(_shb_packet->gnPacket().packet().extendedHeader().get_opt_value());
      if (eh == nullptr) {
        loggers::get_instance().error("geonetworking_layer::build_geonetworking_pdu: Wrong cast");
        return -1;
      }
      // Update NextHeader
      it = params.find(params::next_header);
      if (next_header.compare("btpB") == 0) {
        _shb_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpB;
      } else { // Default btp is btpA
        _shb_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpA;
      }
      // Update payload
      _shb_packet->gnPacket().packet().commonHeader().plLength() = data.lengthof();
      _shb_packet->gnPacket().packet().payload()                 = OPTIONAL<OCTETSTRING>(data);
      // Update timestamp
      eh->shbHeader().srcPosVector().timestamp__().set_long_long_val(static_cast<unsigned int>(base_time::get_instance().get_its_current_time_mod_ms()));

      loggers::get_instance().log_msg("geonetworking_layer::build_geonetworking_pdu: shb: ", *_shb_packet);
      // Encode GeoNetworking PDU
      OCTETSTRING os;
      _codec.encode(*_shb_packet, os);
      data = os;
    } else { // Use TSB
      ExtendedHeader *eh = static_cast<ExtendedHeader *>(_tsb_packet->gnPacket().packet().extendedHeader().get_opt_value());
      if (eh == nullptr) {
        loggers::get_instance().error("geonetworking_layer::build_geonetworking_pdu: Wrong cast");
        return -1;
      }
      // Update sequence number
      eh->tsbHeader().seqNumber() = _sequence_number++;
      // Update NextHeader
      it = params.find(params::next_header);
      if (next_header.compare("btpB") == 0) {
        _tsb_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpB;
      } else { // Default btp is btpA
        _tsb_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpA;
      }
      // Update payload
      _tsb_packet->gnPacket().packet().commonHeader().plLength() = data.lengthof();
      _tsb_packet->gnPacket().packet().payload()                 = OPTIONAL<OCTETSTRING>(data);
      // Update timestamp
      eh->tsbHeader().srcPosVector().timestamp__().set_long_long_val(static_cast<unsigned int>(base_time::get_instance().get_its_current_time_mod_ms()));

      loggers::get_instance().log_msg("geonetworking_layer::build_geonetworking_pdu: tsb: ", *_tsb_packet);
      // Encode GeoNetworking PDU
      OCTETSTRING os;
      _codec.encode(*_tsb_packet, os);
      data = os;
    }
  } else if (header_type.compare("uni") == 0) {
    ExtendedHeader *eh = static_cast<ExtendedHeader *>(_uni_packet->gnPacket().packet().extendedHeader().get_opt_value());
    if (eh == nullptr) {
      loggers::get_instance().error("geonetworking_layer::build_geonetworking_pdu: Wrong cast");
      return -1;
    }
    // Update NextHeader
    it = params.find(params::next_header);
    if (next_header.compare("btpB") == 0) {
      _uni_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpB;
    } else { // Default btp is btpA
      _uni_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpA;
    }
    // Update sequence number
    eh->geoUnicastHeader().seqNumber() = _sequence_number++;
    // Update payload
    _uni_packet->gnPacket().packet().commonHeader().plLength() = data.lengthof();
    _uni_packet->gnPacket().packet().payload()                 = OPTIONAL<OCTETSTRING>(data);
    // Update timestamp
    eh->geoUnicastHeader().srcPosVector().timestamp__().set_long_long_val(static_cast<unsigned int>(base_time::get_instance().get_its_current_time_mod_ms()));

    loggers::get_instance().log_msg("geonetworking_layer::build_geonetworking_pdu: uni: ", *_uni_packet);

    // Encode GeoNetworking PDU
    OCTETSTRING os;
    _codec.encode(*_uni_packet, os);
    data = os;
  } else { // TODO To be continued
    // Default: Use GBC
    ExtendedHeader *eh = static_cast<ExtendedHeader *>(_gbc_packet->gnPacket().packet().extendedHeader().get_opt_value());
    if (eh == nullptr) {
      loggers::get_instance().error("geonetworking_layer::build_geonetworking_pdu: Wrong cast");
      return -1;
    }
    // Update NextHeader
    it = params.find(params::next_header);
    if (next_header.compare("btpB") == 0) {
      _gbc_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpB;
    } else { // Default btp is btpA
      _gbc_packet->gnPacket().packet().commonHeader().nextHeader() = NextHeader::e__btpA;
    }
    // Update sequence number
    eh->geoBroadcastHeader().seqNumber() = _sequence_number++;
    // Update payload
    _gbc_packet->gnPacket().packet().commonHeader().plLength() = data.lengthof();
    _gbc_packet->gnPacket().packet().payload()                 = OPTIONAL<OCTETSTRING>(data);
    // Update timestamp
    eh->geoBroadcastHeader().srcPosVector().timestamp__().set_long_long_val(static_cast<unsigned int>(base_time::get_instance().get_its_current_time_mod_ms()));

    loggers::get_instance().log_msg("geonetworking_layer::build_geonetworking_pdu: gbc: ", *_gbc_packet);

    // Encode GeoNetworking PDU
    OCTETSTRING os;
    _codec.encode(*_gbc_packet, os);
    data = os;
  }

  return 0;
}

int geonetworking_layer::build_secured_pdu(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> geonetworking_layer::build_secured_pdu: ", data);
  // params.log();

  LibItsGeoNetworking__TypesAndValues::BasicHeader basic_header;
  decode_basic_header(data, basic_header);
  if (basic_header.nextHeader() == BasicNextHeader::e__securedPacket) { // Already secured (ATS Security test suite/f_sendSecuredGn/Cam/Denm TTCN-3 functions
    // Leave data unchanged
    loggers::get_instance().log_msg("<<< geonetworking_layer::build_secured_pdu: Leave data unchanged: ", data);
    return 0;
  }
  // Update security mode
  unsigned int basic_header_len = 4; // FIXME How to retrieve the BasicHeader length basic_header.get_descriptor()->raw->fieldlength / 8;
  loggers::get_instance().log("geonetworking_layer::build_secured_pdu: basic_header_len = %d", basic_header_len);
  basic_header.nextHeader()        = BasicNextHeader::e__securedPacket;
  OCTETSTRING unsecured_gn_payload = OCTETSTRING(data.lengthof() - basic_header_len, static_cast<const unsigned char *>(data) + basic_header_len);
  OCTETSTRING secured_gn_payload;
  if (security_services::get_instance().secure_gn_payload(unsecured_gn_payload, secured_gn_payload, params) != 0) {
    loggers::get_instance().warning("geonetworking_layer::build_secured_pdu: failed to build secured pdu");
    return -1;
  }

  // Encode the basic header
  // loggers::get_instance().log_msg("geonetworking_layer::build_secured_pdu: New basic_header = ", basic_header);
  RAW_enc_tr_pos rp;
  rp.level = 0;
  rp.pos   = NULL;
  RAW_enc_tree enc_tree(FALSE, NULL, &rp, 1, basic_header.get_descriptor()->raw);
  basic_header.RAW_encode(*basic_header.get_descriptor(), enc_tree);
  TTCN_Buffer encoding_buffer;
  enc_tree.put_to_buf(encoding_buffer);
  // Copy result
  data = OCTETSTRING(encoding_buffer.get_len(), encoding_buffer.get_data()) + secured_gn_payload;
  loggers::get_instance().log_msg("geonetworking_layer::build_secured_pdu: Secured pdu = ", data);

  return 0;
}

int geonetworking_layer::decode_basic_header(const OCTETSTRING &p_data, LibItsGeoNetworking__TypesAndValues::BasicHeader &p_basic_header) {
  // loggers::get_instance().log_msg(">>> geonetworking_layer::decode_basic_header: ", p_data);

  // Update security mode
  OCTETSTRING bh =
    OCTETSTRING(4, static_cast<const unsigned char *>(p_data)); // Extract the basic header as specified in ETSI EN 302 636-4-1 V1.3.1 (2017-08) Clause 9.6
  // loggers::get_instance().log_msg("geonetworking_layer::decode_basic_header: bh: ", bh);
  TTCN_Buffer decoding_buffer(bh);
  p_basic_header.RAW_decode(*p_basic_header.get_descriptor(), decoding_buffer, decoding_buffer.get_len() * 8, raw_order_t::ORDER_MSB);
  // loggers::get_instance().log_msg("geonetworking_layer::decode_basic_header: ", p_basic_header);

  return 0;
}

int geonetworking_layer::setup_secured_mode() {
  loggers::get_instance().log(">>> geonetworking_layer::setup_secured_mode");

  loggers::get_instance().log("geonetworking_layer::setup_secured_mode: GN Layer address = %p", this);

  params::const_iterator it = _params.find(params::certificate);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("certificate"), "CERT_TS_A_AT"));
  }
  it = _params.find(params::sec_db_path);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("sec_db_path"), ""));
  }
  it = _params.find(params::hash);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("hash"), "SHA-256"));
  }
  // Set up security services even if secured_mode is set to 0. Later, we can receive an AcEnableSecurity request, the sertificate caching will be ready to go
  security_services::get_instance().setup(_params);
  security_services::get_instance().set_position(_latitude, _longitude);

  return 0;
}

geonetworking_layer_factory geonetworking_layer_factory::_f;
