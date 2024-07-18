#include "LibItsIpv6OverGeoNetworking_Functions.hh"
#include <math.h>
#include <time.h>

namespace LibItsIpv6OverGeoNetworking__Functions {
  /*       * @desc    Calculate ICMPv6 checksum on pseudo header according RFC 4443 - Clause 2.3
           * @param   p_sourceAddress         Source address,
           * @param   p_destinationAddress    Destination address
           * @param   p_payloadLength         Upper-Layer Packet Length
           * @param   p_payload               Upper-Layer payload
           * @param   p_nextHdr               Next header value (e.g. 0x3a for ICMPv6)
           * @return  The checksum value
           * <pre>
           * Pseudo header is defined by RFC 2460 - Clause 8.1
           *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           *  |                                                               |
           *  +                                                               +
           *  |                                                               |
           *  +                         Source Address                        +
           *  |                                                               |
           *  +                                                               +
           *  |                                                               |
           *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           *  |                                                               |
           *  +                                                               +
           *  |                                                               |
           *  +                      Destination Address                      +
           *  |                                                               |
           *  +                                                               +
           *  |                                                               |
           *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           *  |                   Upper-Layer Packet Length                   |
           *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           *  |                      zero                     |  Next Header  |
           *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           * </pre>
          external function fx_computeIPv6CheckSum(
              in template (value) Ipv6Address p_sourceAddress,
              in template (value) Ipv6Address p_destinationAddress,
              in template (value) integer p_payloadLength,
              in template (value) octetstring p_payload,
              in template (value) UInt8 p_nextHdr
          ) return Oct2;
  */

  OCTETSTRING fx__computeIPv6CheckSum(const OCTETSTRING_template &p__sourceAddress, const OCTETSTRING_template &p__destinationAddress,
                                      const INTEGER_template &p__payloadLength, const OCTETSTRING_template &p__payload, const INTEGER_template &p__nextHdr) {
    OCTETSTRING ret_val = int2oct(0, 1);
    return ret_val;
  }

} // namespace LibItsIpv6OverGeoNetworking__Functions
