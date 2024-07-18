/*!
 * \file      layer_factory.hh
 * \brief     Header file for ITS abstract protocol layer definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "layer.hh"

/*!
 * \class layer_factory
 * \brief  This class provides a factory class to create layer class instances
 * \abstract
 */
class layer_factory {
public: //! \publicsection
  /*!
   * \fn codec();
   * \brief  Default constructor
   */
  layer_factory(){};
  /*!
   * \fn layer* create_layer(const std::string & type, const std::string & param);
   * \brief  Create the layers stack based on the provided layers stack description (cf. remark)
   * \param[in] p_type The provided layers stack description
   * \param[in] p_params Optional parameters
   * \return 0 on success, -1 otherwise
   * \remark The description below introduces layers stack in case of ITS project:
   *     CAM layer
   *       next_header     : btpA|btpB (overwrite BTP.type)
   *       header_type     : tsb|gbc
   *       header_sub_type : sh (single hop)
   *     DENM layer
   *       next_header     : btpA|btpB (overwrite BTP.type)
   *       header_type     : tsb|gbc
   *     BTP layer
   *       type            : btpA|btpB
   *       destination port: dst_port
   *       source port     : src_port
   *       device_mode     : Set to 1 if the layer shall encapsulate upper layer PDU
   *     GN layer
   *       its_aid                : ITS AID as defined by ETSI TS 102 965 V1.2.1. Default: 141
   *       ll_address             : GeoNetworking address of the Test System
   *       latitude               : latitude of the Test System
   *       longitude              : longitude of the Test System
   *       beaconing              : Set to 1 if GnLayer shall start beaconing
   *       Beaconing timer expiry : expiry (ms)
   *       device_mode            : Set to 1 if the layer shall encapsulate upper layer PDU
   *       secured_mode           : Set to 1 if message exchanges shall be signed
   *       encrypted_mode         : Set to 1 if message exchanges shall be encrypted
   *                                NOTE: For signed & encrypted message exchanges, both secured_mode and encrypted_mode shall be set to 1
   *       certificate            : Certificate identifier the Test Adapter shall use
   *       secure_db_path         : Path to the certificates and keys storage location
   *       hash                   : Hash algorithm to be used when secured mode is set
   *                                Authorized values are SHA-256 or SHA-384
   *                                Default: SHA-256
   *       signature              : Signature algorithm to be used when secured mode is set
   *                                Authorized values are NISTP-256, BP-256 and BP-384
   *                                Default: NISTP-256
   *       cypher                 : Cyphering algorithm to be used when secured mode is set
   *     Ethernet layer
   *       mac_src  :Source MAC address
   *       mac_bc   :Broadcast address
   *       eth_type : Ethernet type
   *     Commsignia layer
   *       mac_src     : Device MAC address, used to discard packets
   *                     To indicate no filering, use the value 000000000000
   *       mac_bc      : Broadcast address
   *       eth_type    : Ethernet type, used to discard packets
   *       target_host : Device address
   *       target_port : Device port
   *       source_port : Test System port
   *       interface_id: Interface id, used to discard packets
   *       tx_power    : TX power (dB)
   *     UDP layer (IP/UDP based on Pcap)
   *       dst_ip  : destination IPv4 address (aa.bb.cc.dd)
   *       dst_port: destination port
   *       src_ip  : source IPv4 address (aa.bb.cc.dd)
   *       src_port: source port
   *     Pcap layer
   *       mac_src    : Source MAC address, used to exclude from capture the acket sent by the Test System
   *       filter     : Pcap filter (compliant with tcpdump syntax)
   *                    E.g. filter=and ether src 04e548000001
   *       Online mode:
   *         nic: Local NIC
   *              If set, online mode is used
   *       Offline mode (nic is present but not set):
   *         file        : File to read
   *         frame_offset: Frame offset, used to skip packets with frame number < frame_offset
   *         time_offset : Time offset, used to skip packets with time offset < time_offset
   *         save_mode   : 1 to save sent packet, 0 otherwise
   *     Here are some examples:
   *       GeoNetworking multiple component case:
   *         NodeB.geoNetworkingPort.params :=
"GN(ll_address=04e548000001,latitude=43551050,longitude=10298730,beaconing=0,expiry=1000,its_aid=141)/COMMSIGNIA(mac_src=04e548000001,mac_bc=FFFFFFFFFFFF,eth_type=8947,target_host=10.200.1.101,target_port=7942,source_port=7943,its_aid=141,interface_id=2,tx_power=-32)/UDP(dst_ip=192.168.56.1,dst_port=12346,src_ip=192.168.156.4,src_port=12345)/ETH(mac_src=04e548000001,mac_dst=0A0027000011,eth_type=0800)/PCAP(mac_src=04e548000001,file=/home/vagrant/TriesAndDelete/etsi_its/testdata/TC_AUTO_IOT_DENM_RWW_BV_01_short.pcap,filter=and
(udp port 30000 or udp port 7943))" NodeC.geoNetworkingPort.params :=
"GN(ll_address=70b3d5791b48,latitude=43551050,longitude=10298730,beaconing=0,expiry=1000,its_aid=141)/COMMSIGNIA(mac_src=70b3d5791b48,mac_bc=FFFFFFFFFFFF,eth_type=8947,target_host=10.200.1.101,target_port=7942,source_port=7943,its_aid=141,interface_id=2,tx_power=-32)/UDP(dst_ip=192.168.56.1,dst_port=12346,src_ip=192.168.156.4,src_port=12345)/ETH(mac_src=70b3d5791b48,mac_dst=0A0027000011,eth_type=0800)/PCAP(mac_src=70b3d5791b48,file=/home/vagrant/TriesAndDelete/etsi_its/testdata/TC_AUTO_IOT_DENM_RWW_BV_01_short.pcap,filter=and
(udp port 30000 or udp port 7943))"
   *         NodeB.geoNetworkingPort.params :=
"GN(ll_address=04e548000001,latitude=43551050,longitude=10298730,beaconing=0,expiry=1000,its_aid=141)/ETH(mac_src=04e548000001,mac_dst=0A0027000011,eth_type=0800)/PCAP(mac_src=04e548000001,file=/home/vagrant/TriesAndDelete/etsi_its/testdata/TC_AUTO_IOT_DENM_RWW_BV_01.pcap,filter=and
ether src 04e548000001)" #NodeC.geoNetworkingPort.params :=
"GN(ll_address=70b3d5791b48,latitude=43551050,longitude=10298730,beaconing=0,expiry=1000,its_aid=141)/ETH(mac_src=70b3d5791b48,mac_dst=0A0027000011,eth_type=0800)/PCAP(mac_src=70b3d5791b48,file=/home/vagrant/TriesAndDelete/etsi_its/testdata/TC_AUTO_IOT_DENM_RWW_BV_01.pcap,filter=and
ether src 70b3d5791b48)"
   *       UpperTester port based on UDP
   *         system.utPort.params :=
"UT_GN/UDP(dst_ip=192.168.1.1,dst_port=12346,src_ip=192.168.156.4,src_port=12345)/ETH(mac_src=026f8338c1e5,mac_dst=0A0027000011,eth_type=0800)/PCAP(mac_src=0800275c4959,nic=enp0s8,filter=and
udp port 12346)"
   * \pure
   */
  virtual layer *create_layer(const std::string &p_type, const std::string &p_params) = 0;
}; // End of class layer_factory
