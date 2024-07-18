#include "ItsRSUsSimulator_Functions.hh"

namespace ItsRSUsSimulator__Functions {
  void fx__computePositionFromRotation(const INTEGER &p__refLatitude, const INTEGER &p__refLongitude, const INTEGER &p__cenLatitude,
                                       const INTEGER &p__cenLongitude, const INTEGER &p__rotation, INTEGER &p__latitude, INTEGER &p__longitude) {
    // TODO
    p__latitude  = p__cenLatitude;
    p__longitude = p__cenLongitude;
  }

} // namespace ItsRSUsSimulator__Functions
