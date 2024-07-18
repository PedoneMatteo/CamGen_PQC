#include "LibItsCommon_Functions.hh"
#include <math.h>

#include "base_time.hh"
#include "loggers.hh"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define earthRadius 6378137.0L
#define rbis = ((double)(earthRadius * M_PI / 180))

namespace LibItsCommon__Functions {

  /**
   * @desc    This external function gets the current time
   * @return  Timestamp - current time since 01/01/2004 in milliseconds
   * @see     fx_getCurrentTime() return TimestampIts
   */
  INTEGER fx__getCurrentTime() {
    INTEGER i;
    i.set_long_long_val(base_time::get_instance().get_its_current_time_ms());
    loggers::get_instance().log_msg("<<< fx__getCurrentTime: ", i);
    return i;
  }
  /**
   * @desc    This external function gets the current time since 01/01/1970 in UTC format
   * @return  The current time since 01/01/1970 in UTC format in milliseconds
   * @see     fx_getCurrentTimeUtc() return UInt64
   */
  INTEGER fx__getCurrentTimeUtc() {
    INTEGER i;
    i.set_long_long_val(base_time::get_instance().get_current_time_ms());
    loggers::get_instance().log_msg("<<< fx__getCurrentTimeUtc: ", i);
    return i;
  }
  /**
   * @desc    Gets the current time since 01/01/2004
   * @return  TimeMark - tenths of a second in the current or next hour in units of 1/10th second from UTC time
   * @see     function f_getCurrentTimeMark() return TimeMark
   */
  INTEGER fx__getCurrentTimeMark() {
    //	TODO: this is just a sceleton. fill in the function
    return 0;
  }

  /**
   * @desc    Gets the Minute of current UTC year
   * @return  MinuteOfTheYear - tenths of a second in the current or next hour in units of 1/10th second from UTC time
   * @see     function f_getMinuteOfTheYear() return MinuteOfTheYear
   */
  INTEGER fx__getMinuteOfTheYear() {
    //	TODO: this is just a sceleton. fill in the function
    return 0;
  }

  /**
   * @desc    Gets the milliseconds point in the current UTC minute
   * @return  DSecond - The milliseconds point in the current UTC minute (0..60000)
   * @see     function f_getDSecond() return DSecond
   */
  INTEGER fx__getDSecond() {
    //	TODO: this is just a sceleton. fill in the function
    return 0;
  }

  /*       * @desc    External function to compute distance between two points
   * @param   p_latitudeA   Latitude of first point
   * @param   p_longitudeA  Longitude of first point
   * @param   p_latitudeB   Latitude of second point
   * @param   p_longitudeB  Longitude of second point
   * @return  Computed distance in meters
   fx_computeDistance(in Int32 p_latitudeA, in Int32 p_longitudeA, in Int32 p_latitudeB, in Int32 p_longitudeB) return float;
  */
  FLOAT fx__computeDistance(const INTEGER &p__latitudeA, const INTEGER &p__longitudeA, const INTEGER &p__latitudeB, const INTEGER &p__longitudeB) {
    double d_latA = ((double)p__latitudeA) / 10000000.0;
    double d_latB = ((double)p__latitudeB) / 10000000.0;

    double d_lonA = ((double)p__longitudeA) / 10000000.0;
    double d_lonB = ((double)p__longitudeB) / 10000000.0;

    double d_lat = (d_latB - d_latA) * (M_PI / 180.0);
    double d_lon = (d_lonB - d_lonA) * (M_PI / 180.0);

    double a = sin(d_lat / 2) * sin(d_lat / 2) + cos(d_latA * M_PI / 180.0) * cos(d_latB * M_PI / 180.0) * sin(d_lon / 2) * sin(d_lon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return FLOAT(earthRadius * c);
  }

  /*       * @desc    External function to compute a position using a reference position, a distance and an orientation
   * @param   p_iutLongPosVector  Reference position
   * @param   p_distance          Distance to the reference position (in meter)
   * @param   p_orientation       Direction of the computed position (0 to 359; 0 means North)
   * @param   p_latitude          Computed position's latitude
   * @param   p_longitude         Computed position's longitude
   fx_computePositionUsingDistance(in Int32 p_refLatitude,in Int32 p_refLongitude,in float p_distance,in integer p_orientation,out Int32 p_latitude,out Int32
   p_longitude);
  */
  void fx__computePositionUsingDistance(const INTEGER &p__refLatitude, const INTEGER &p__refLongitude, const FLOAT &p__distance, const INTEGER &p__orientation,
                                        INTEGER &p__latitude, INTEGER &p__longitude) {
    double angularD   = static_cast<const float>(p__distance) / earthRadius;
    double radHeading = (double)static_cast<const int>(p__orientation) * M_PI / 180;

    // Convert to rad
    double lat1  = ((double)static_cast<const int>(p__refLatitude) / 10000000) * M_PI / 180;
    double long1 = ((double)static_cast<const int>(p__refLongitude) / 10000000) * M_PI / 180;

    double lat2  = asin(sin(lat1) * cos(angularD) + cos(lat1) * sin(angularD) * cos(radHeading));
    double long2 = long1 + atan2(sin(radHeading) * sin(angularD) * cos(lat1), cos(angularD) - sin(lat1) * sin(lat2));

    // normalise to -180...+180
    long2 = fmod((long2 + 3 * M_PI), (2 * M_PI) - M_PI);

    // convert to 1/10 of microdegrees
    long rlat2  = round(lat2 * 10000000 / M_PI * 180);
    long rlong2 = round(long2 * 10000000 / M_PI * 180);

    p__latitude = rlat2; // asin(sin(ref_lat)*cos(distance) + cos(ref_lat)*sin(distance)*cos(angle)) * 180.0 / M_PI;
    p__longitude =
      rlong2; //((double)p__refLongitude) + atan2(sin(angle)*sin(distance)*cos(ref_lat), cos(distance) - sin(ref_lat)*sin((double)p__latitude))*(180.0/M_PI);
  }

  /*       * @desc    External function to compute radius of a given circular area
   * @param   p_squareMeters  Square meters of an circular area
   * @return  Computed radius in meters
   fx_computeRadiusFromCircularArea(in float p_squareMeters) return float;
  */
  FLOAT fx__computeRadiusFromCircularArea(const FLOAT &p__squareMeters) { return FLOAT(sqrt(p__squareMeters / M_PI)); }

} // namespace LibItsCommon__Functions
