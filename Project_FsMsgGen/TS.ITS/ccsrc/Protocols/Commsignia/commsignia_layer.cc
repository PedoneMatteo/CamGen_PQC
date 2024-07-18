#include <algorithm>
#include <arpa/inet.h>
#include <cctype>

#include "commsignia_layer_factory.hh"

#include "loggers.hh"

#include "converter.hh"

unsigned char commsignia_layer::_g5_fixed_header[10] = {0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x0D};

commsignia_layer::commsignia_layer(const std::string &p_type, const std::string &param) : layer(p_type), _params(), _mac_src(), _eth_type() {
  loggers::get_instance().log(">>> commsignia_layer::commsignia_layer: %s, %s", to_string().c_str(), param.c_str());

  // Setup parameters
  params::convert(_params, param);
  params::const_iterator it = _params.find(params::mac_src);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("mac_src"), "000000000000"));
  }
  _mac_src = converter::get_instance().hexa_to_bytes(_params[params::mac_src]);
  it       = _params.find(params::mac_bc);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("mac_bc"), "FFFFFFFFFFFF"));
  }
  it = _params.find(params::eth_type);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("eth_type"), "8947"));
  }
  _eth_type = converter::get_instance().hexa_to_bytes(_params[params::eth_type]);
  it        = _params.find(params::interface_id);
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("interface_id"), "1")); // The interfce id for filtering & to send on
  }
  it = _params.find(std::string("power_tx"));
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("power_tx"), "20")); // 20db
  }
  it = _params.find(std::string("data_rate"));
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("data_rate"), "12")); // 12 * 500Kbps = 6Mbps
  }
  it = _params.find(std::string("use_vpn"));
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("use_vpn"), "0"));
  }
  it = _params.find(std::string("xport_mode"));
  if (it == _params.cend()) {
    _params.insert(std::pair<std::string, std::string>(std::string("xport_mode"), "G5"));
  }
  //_params.log();
}

void commsignia_layer::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> commsignia_layer::send_data: ", data);

  OCTETSTRING buffer = OCTETSTRING(0, nullptr);
  if (_params[std::string("xport_mode")].compare("G5") == 0) {
    buffer = OCTETSTRING(10, commsignia_layer::_g5_fixed_header);
    if (_params[std::string("use_vpn")].compare("1") == 0) {
      buffer += int2oct(1, 1); // Injection to software
    } else {
      buffer += int2oct(0, 1); // Injection to radio
    }
    buffer += int2oct(0, 1); // Fix
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: Injection=", buffer);
    buffer += int2oct(std::stoi(_params[params::interface_id]), 4);
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: Interface=", buffer);
    buffer += int2oct(std::stoi(_params[std::string("data_rate")]), 2);
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: data_rate=", buffer);
    buffer += int2oct(7, 1);                                                        // MAC user priority
    buffer += int2oct(0x80000000 & std::stoi(_params[std::string("power_tx")]), 4); // Negative number
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: PowerTx=", buffer);
    // Destination MAC address
    params::const_iterator it = params.find(params::mac_dst); // Find in provided parameters, params
    if (it != params.cend()) {
      buffer += str2oct(CHARSTRING(it->second.c_str()));
    } else {
      it = _params.find(params::mac_dst);
      if (it != _params.cend()) {
        buffer += str2oct(CHARSTRING(it->second.c_str()));
      } else {
        buffer += str2oct(CHARSTRING(_params[params::mac_bc].c_str()));
      }
    }
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: mac_dst=", buffer);
    // Source MAC address
    it = params.find(params::mac_src); // Find in provided parameters, params
    if (it != params.cend()) {
      buffer += str2oct(CHARSTRING(it->second.c_str()));
    } else {
      it = _params.find(params::mac_src);
      if (it != _params.cend()) {
        buffer += str2oct(CHARSTRING(it->second.c_str()));
      } else {
        buffer += str2oct(CHARSTRING(_params[params::mac_src].c_str()));
      }
    }
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: mac_src=", buffer);
    buffer += int2oct(0, 2); // Fixed
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: Fixed=", buffer);
    buffer += int2oct(data.lengthof(), 2); // Data length
    loggers::get_instance().log_msg("commsignia_layer::send_data: buffer: Data length=", buffer);
  }               // else, LTE-V2X, injection buffer = GN+BTP+CAM/DENM only
  buffer += data; // Payload

  loggers::get_instance().log_msg("commsignia_layer::send_data: Final buffer=", buffer);
  send_to_all_layers(buffer, params);
}

void commsignia_layer::receive_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log_msg(">>> commsignia_layer::receive_data: ", data);

  const unsigned char *p       = static_cast<const unsigned char *>(data);
  const unsigned char  c2p_ver = (unsigned char)*p;

  // Check the frame version
  if ((c2p_ver != 0x11) && (c2p_ver != 0x17)) { // Rx G5 or LTE-V2X
    // Discard it, only use G5 or LTE-V2X RX version
    return;
  }

  if (c2p_ver == 0x11) { // G5
    commsignia_layer::c2p_s_v1_rx_t r;
    r.s_header.u8_ver_type = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: version=%02x", r.s_header.u8_ver_type);
    memcpy((void *)&r.s_header.u32_tst_sec, (unsigned int *)p, sizeof(unsigned int));
    p += sizeof(unsigned int);
    loggers::get_instance().log("commsignia_layer::receive_data: time (in s)=%08x", /*ntohl(*/ r.s_header.u32_tst_sec) /*)*/;
    memcpy((void *)&r.s_header.u32_tst_msec, (unsigned int *)p, sizeof(unsigned int));
    p += sizeof(unsigned int);
    loggers::get_instance().log("commsignia_layer::receive_data: time (in ms)=%08x", /*ntohl(*/ r.s_header.u32_tst_msec) /*)*/;
    r.u8_primary_channel = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: primary_channel=%02x", r.u8_primary_channel);
    r.u8_secondary_channel = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: secondary_channel=%02x", r.u8_secondary_channel);
    r.u8_used_interface = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: used_interface=%02x", r.u8_used_interface);
    r.u8_data_rate = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: data_rate=%02x", r.u8_data_rate);
    r.u8_antenna = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: antenna=%02x", r.u8_antenna);
    memcpy((void *)&r.s32_latitude, (int *)p, sizeof(int));
    p += sizeof(int);
    loggers::get_instance().log("commsignia_layer::receive_data: latitude=%08x", r.s32_latitude);
    memcpy((void *)&r.s32_longitude, (int *)p, sizeof(int));
    p += sizeof(int);
    loggers::get_instance().log("commsignia_layer::receive_data: longitude=%08x", r.s32_longitude);
    memcpy((void *)&r.u16_speed, (unsigned short *)p, sizeof(unsigned short));
    p += sizeof(unsigned short);
    loggers::get_instance().log("commsignia_layer::receive_data: speed=%02x", r.u16_speed); // ntohs
    memcpy((void *)&r.u16_heading, (unsigned short *)p, sizeof(unsigned short));
    p += sizeof(unsigned short);
    loggers::get_instance().log("commsignia_layer::receive_data: heading=%02x", r.u16_heading);
    r.s8_rssi_ant_1 = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: s8_rssi_ant_1=%02x", r.s8_rssi_ant_1);
    r.s8_rssi_ant_2 = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: s8_rssi_ant_2=%02x", r.s8_rssi_ant_2);
    r.s8_noise_ant_1 = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: s8_noise_ant_1=%02x", r.s8_noise_ant_1);
    r.s8_noise_ant_2 = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: s8_noise_ant_2=%02x", r.s8_noise_ant_2);
    memcpy((void *)&r.u16_cbr_ant_1, (unsigned short *)p, sizeof(unsigned short));
    p += sizeof(unsigned short);
    loggers::get_instance().log("commsignia_layer::receive_data: cbr_ant_1=%04x", r.u16_cbr_ant_1);
    memcpy((void *)&r.u16_cbr_ant_2, (unsigned short *)p, sizeof(unsigned short));
    p += sizeof(unsigned short);
    loggers::get_instance().log("commsignia_layer::receive_data: cbr_ant_2=%04x", r.u16_cbr_ant_2);
    // Filtering on antenna index
    loggers::get_instance().log("commsignia_layer::receive_data: compare %02x with %02x", r.u8_antenna,
                                static_cast<unsigned char>(std::stoi(_params[params::interface_id])));
    if (r.u8_antenna != std::stoi(_params[params::interface_id])) {
      // Discard packet
      loggers::get_instance().warning("commsignia_layer::receive_data: Discard packet due to wrong antenna id");
      // TODO return;
    }      // else, continue
  } else { // LTE-CV2X
    commsignia_layer::c2p_s_v1_rx_cv2x_t r;
    r.s_header.u8_ver_type = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: version=%02x", r.s_header.u8_ver_type);
    memcpy((void *)&r.s_header.u32_tst_sec, (unsigned int *)p, sizeof(unsigned int));
    p += sizeof(unsigned int);
    loggers::get_instance().log("commsignia_layer::receive_data: time (in s)=%08x", /*ntohl(*/ r.s_header.u32_tst_sec) /*)*/;
    memcpy((void *)&r.s_header.u32_tst_msec, (unsigned int *)p, sizeof(unsigned int));
    p += sizeof(unsigned int);
    loggers::get_instance().log("commsignia_layer::receive_data: time (in ms)=%08x", /*ntohl(*/ r.s_header.u32_tst_msec) /*)*/;
    r.u8_socket_index = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: u8_socket_index=%02x", r.u8_socket_index);
    memcpy((void *)&r.u16_ethertype, (unsigned short *)p, sizeof(unsigned short));
    p += sizeof(unsigned short);
    loggers::get_instance().log("commsignia_layer::receive_data: u16_ethertype=%04x", r.u16_ethertype);
    r.s8_rssi = (signed char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: s8_rssi=%02x", r.s8_rssi);
    r.u8_datarate_500kbps = (unsigned char)*p++;
    loggers::get_instance().log("commsignia_layer::receive_data: u8_datarate_500kbps=%02x", r.u8_datarate_500kbps);
  }

  commsignia_layer::c2p_802_11p_hdr h;
  memcpy((void *)&h.frame_ctrl, (unsigned short *)p, sizeof(unsigned short));
  p += sizeof(unsigned short);
  loggers::get_instance().log("commsignia_layer::receive_data: frame_ctrl=%04x", h.frame_ctrl);
  memcpy((void *)&h.duration, (unsigned short *)p, sizeof(unsigned short));
  p += sizeof(unsigned short);
  loggers::get_instance().log("commsignia_layer::receive_data: duration=%04x", h.duration);
  memcpy((void *)&h.dst_addr, (unsigned short *)p, 6 * sizeof(unsigned char));
  p += 6 * sizeof(unsigned char);
  memcpy((void *)&h.src_addr, (unsigned short *)p, 6 * sizeof(unsigned char));
  p += 6 * sizeof(unsigned char);
  memcpy((void *)&h.bss_id, (unsigned short *)p, 6 * sizeof(unsigned char));
  p += 6 * sizeof(unsigned char);
  memcpy((void *)&h.fragment_seq_num, (unsigned short *)p, sizeof(unsigned short));
  p += sizeof(unsigned short);
  loggers::get_instance().log("commsignia_layer::receive_data: fragment_seq_num=%04x", h.fragment_seq_num);

  OCTETSTRING bid = OCTETSTRING(6, (const unsigned char *)&h.bss_id); // TODO To be removed
  loggers::get_instance().log_msg("commsignia_layer::receive_data: bss_id: ", bid);
  OCTETSTRING dst = OCTETSTRING(6, (const unsigned char *)&h.dst_addr);
  loggers::get_instance().log_msg("commsignia_layer::receive_data: dst: ", dst);
  OCTETSTRING src = OCTETSTRING(6, (const unsigned char *)&h.src_addr);
  loggers::get_instance().log_msg("commsignia_layer::receive_data: src: ", src);

  // Filtering on source MAC address of the packet
  if (!std::equal(_mac_src.cbegin(), _mac_src.cend(), static_cast<const unsigned char *>(src))) {
    // Discard packet
    loggers::get_instance().warning("commsignia_layer::receive_data: Discard packet due to wrong comparison");
    // TODO return;
  } // else, continue
  int length;
  if (c2p_ver == 0x11) {
    const commsignia_layer::c2p_llc_hdr *l;
    if ((ntohs(h.frame_ctrl) & 0xf000) == 0x8000) {
      l      = (const commsignia_layer::c2p_llc_hdr *)(static_cast<const unsigned char *>(data) + sizeof(commsignia_layer::c2p_s_v1_rx_t) +
                                                  sizeof(commsignia_layer::c2p_802_11p_hdr) + sizeof(commsignia_layer::c2p_qos_ctrl));
      length = sizeof(commsignia_layer::c2p_s_v1_rx_t) + sizeof(commsignia_layer::c2p_802_11p_hdr) + sizeof(commsignia_layer::c2p_qos_ctrl) +
               sizeof(commsignia_layer::c2p_llc_hdr);
    } else {
      l      = (const commsignia_layer::c2p_llc_hdr *)(p + sizeof(commsignia_layer::c2p_s_v1_rx_t) + sizeof(commsignia_layer::c2p_802_11p_hdr));
      length = sizeof(commsignia_layer::c2p_s_v1_rx_t) + sizeof(commsignia_layer::c2p_802_11p_hdr) + sizeof(commsignia_layer::c2p_llc_hdr);
    }
  } else { // LTE-V2X
    length = sizeof(commsignia_layer::c2p_s_v1_rx_cv2x_t) + sizeof(commsignia_layer::c2p_802_11p_hdr) + 10 /*Fixed size*/;
  }

  // Check ether type
  // if ((_eth_type[1] == (unsigned char)((l->type & 0xff00) >> 8)) && (_eth_type[0] == (unsigned char)(l->type & 0xff))) { // Warning: Network ordered bytes
  // Extract payload
  data = OCTETSTRING(data.lengthof() - length, length + static_cast<const unsigned char *>(data));
  // loggers::get_instance().log_msg("commsignia_layer::receive_data: payload for upper layer:", data);

  // Update params
  CHARSTRING s            = oct2str(dst);
  params[params::mac_dst] = std::string(static_cast<const char *>(s));
  s                       = oct2str(src);
  params[params::mac_src] = std::string(static_cast<const char *>(s));

  receive_to_all_layers(data, params);
  /*} else {
  // Discard packet
  loggers::get_instance().warning("commsignia_layer::receive_data: Discard packet due to wrong ethernet type");
  //TODO return;
  }*/
}

commsignia_layer_factory commsignia_layer_factory::_f;
