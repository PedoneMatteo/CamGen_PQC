/*!
 * \file      location_table.hh
 * \brief     Header file for GeoNetworking location table definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <map>
#include <memory>

class OCTETSTRING;

namespace LibItsGeoNetworking__TypesAndValues {
  class LongPosVector;
}

/*!
 * \class location_table
 * \brief  This class provides description of GeoNetworking table
 * \todo Add time expired removal procedure
 * \todo Add reference to the GeoNetworking ETSI standard
 */
class location_table {
  std::map<const std::string, std::shared_ptr<LibItsGeoNetworking__TypesAndValues::LongPosVector>>   _entries_by_mids;

public: //! \publicsection
  /*!
   * \brief Default constructor
   */
  explicit location_table() : _entries_by_mids(){};
  /*!
   * \brief Default destructor
   */
  virtual ~location_table() { _entries_by_mids.clear(); }; // std::shared_ptr provides memory free process

  /*!
   * \fn const bool has_entry(const OCTETSTRING& p_mid) const;
   * \brief Indicates if the GeoNetworking MID is present in the table
   * \param[in] p_mid The GeoNetworking MID
   * \return true on success, false otherwise
   */
  const bool has_entry(const OCTETSTRING &p_mid) const;
  /*!
   * \fn void add_entry(const LibItsGeoNetworking__TypesAndValues::LongPosVector& p_long_pos_vector);
   * \brief Add a new item in the location table
   * \param[in] p_long_pos_vector The new long position vector to add
   * \return 1 if this is a new entry, 0 otherwise
   */
  int add_entry(const LibItsGeoNetworking__TypesAndValues::LongPosVector &p_long_pos_vector);
  /*!
   * \fn const LibItsGeoNetworking__TypesAndValues::LongPosVector* get_entry(const OCTETSTRING & p_mid) const;
   * \brief Retrieve an item from the location table based on the GeoNetworking MID
   * \param[in] p_mid The GeoNetworking MID
   */
  const LibItsGeoNetworking__TypesAndValues::LongPosVector *get_entry(const OCTETSTRING &p_mid) const;

  /*!
   * \inline
   * \fn const bool empty() const;
   * \brief Indicates if the table is empty
   * \return true on success, false otherwise
   */
  inline const bool empty() const { return _entries_by_mids.empty(); };
  /*!
   * \inline
   * \fn void reset();
   * \brief Purge the location table
   */
  inline void reset() {
    _entries_by_mids.clear();
  };
}; // End of class location_table
