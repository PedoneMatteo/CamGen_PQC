#include "LibItsIpv6OverGeoNetworking_EncdecDeclarations.hh"

namespace LibItsIpv6OverGeoNetworking__EncdecDeclarations {

  /****************************************************
   * @desc    External function to encode a Ipv6Packet type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/
  BITSTRING fx__enc__Ipv6Packet(const LibItsIpv6OverGeoNetworking__TypesAndValues::Ipv6Packet &p) { return int2bit(0, 8); }

  /****************************************************
   * @desc    External function to decode a Ipv6Packet type
   * @param   value to encode
   * @return  encoded value
   ****************************************************/

  INTEGER fx__dec__Ipv6Packet(BITSTRING &b, LibItsIpv6OverGeoNetworking__TypesAndValues::Ipv6Packet &p) { return -1; }

} // namespace LibItsIpv6OverGeoNetworking__EncdecDeclarations
