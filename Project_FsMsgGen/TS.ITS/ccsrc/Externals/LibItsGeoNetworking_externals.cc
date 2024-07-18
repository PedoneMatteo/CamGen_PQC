#include "LibItsGeoNetworking_Functions.hh"

#include "base_time.hh"
#include "loggers.hh"

namespace LibItsGeoNetworking__Functions {
  /*       * @desc    External function to compute timestamp based on current time
           * @return  Unix-Epoch-Time mod 2^32
          fx_computeGnTimestamp() return UInt32;
  */
  INTEGER fx__computeGnTimestamp() {
    INTEGER i;
    i.set_long_long_val(
      base_time::get_instance().get_its_current_time_ms() %
      4294967296); // Expresses the time in milliseconds at which the latitude and longitude of the ITS-S were acquired by the GeoAdhoc router. The time is
                   // encoded as: TST =TST(TAI)mod232 where TST(TAI) is the number of elapsed TAI milliseconds since 2004-01-01 00:00:00.000 UTC
    loggers::get_instance().log_msg("<<< fx__computeGnTimestamp: ", i);
    return i;
  }

} // namespace LibItsGeoNetworking__Functions
