#include "location_table.hh"

#include "geonetworking_types.hh"

#include "loggers.hh"

using namespace LibItsGeoNetworking__TypesAndValues;

int location_table::add_entry(const LibItsGeoNetworking__TypesAndValues::LongPosVector &p_long_pos_vector) {
  loggers::get_instance().log_msg(">>> location_table::add_entry:", p_long_pos_vector);

  std::map<const std::string, std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector>>::const_iterator it =
    _entries_by_mids.find(std::string(oct2str(p_long_pos_vector.gnAddr().mid())));
  if (it == _entries_by_mids.cend()) { // Not found
    // Add it
    loggers::get_instance().log_to_hexa("location_table::add_entry: Create new entry: ", p_long_pos_vector.gnAddr().mid());
    std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector> lpv(new LibItsGeoNetworking__TypesAndValues::LongPosVector(p_long_pos_vector));
    _entries_by_mids.insert(std::pair<const std::string, std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector>>(
      std::string(oct2str(p_long_pos_vector.gnAddr().mid())), lpv));
      return 1;
  } else {
    // Update the entry
    loggers::get_instance().log_to_hexa("location_table::add_entry: Update entry: ", p_long_pos_vector.gnAddr().mid());
    _entries_by_mids[std::string(oct2str(p_long_pos_vector.gnAddr().mid()))].reset(new LibItsGeoNetworking__TypesAndValues::LongPosVector(p_long_pos_vector));
  }

  return 0;
} // End of add_entry method

const bool location_table::has_entry(const OCTETSTRING &p_mid) const {
  loggers::get_instance().log_to_hexa(">>> location_table::has_entry: ", p_mid);
  loggers::get_instance().log(">>> location_table::has_entry: l=%d", _entries_by_mids.size());
  //  loggers::get_instance().log_to_hexa(">>> location_table::has_entry: ", _entries_by_mids[0]);

  std::map<const std::string, std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector>>::const_iterator it =
    _entries_by_mids.find(std::string(oct2str(p_mid)));
  return (it != _entries_by_mids.cend());
}

const LibItsGeoNetworking__TypesAndValues::LongPosVector *location_table::get_entry(const OCTETSTRING &p_mid) const {
  loggers::get_instance().log_to_hexa("location_table::get_entry: ", p_mid);

  // Sanity check
  if (_entries_by_mids.empty() || (p_mid.lengthof() != 6)) {
    loggers::get_instance().warning("location_table::get_entry: Wrong parameters");
    return nullptr;
  }

  std::map<const std::string, std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector>>::const_iterator it =
    _entries_by_mids.find(std::string(oct2str(p_mid)));
  if (it == _entries_by_mids.cend()) { // Not found
    loggers::get_instance().warning("location_table::get_entry: Item not found");
    return nullptr;
  }

  return it->second.get();
} // End of get_entry method
