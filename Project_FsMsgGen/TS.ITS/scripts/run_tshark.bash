#!/bin/bash

# Debug mode
#set -vx
#set -e

PCAP_SAVE_PATH=~/tmp
rm $PCAP_SAVE_PATH/capture_*.pcap*
#~/frameworks/wireshark-build/run/tshark -ieth1 -V -f"ether proto 0x8947 or udp src port 12345 or udp dst port 12345" -Tfields -eframe.time -eeth.dst -eeth.src -eeth.type -edata -g -w $PCAP_SAVE_PATH/capture_`date +'%Y%m%d'`.pcap -F pcap
#~/frameworks/wireshark-build/run/tshark -ieth1 -V -f"udp port 9091 or udp port 7946" -Tfields -eframe.time -eeth.dst -eeth.src -eeth.type -edata -g -w $PCAP_SAVE_PATH/capture_`date +'%Y%m%d'`.pcap -F pcap
tcpdump -i wlp0s20f3 -vvv -xx -w $PCAP_SAVE_PATH/capture_`date +'%Y%m%d'`.pcap
