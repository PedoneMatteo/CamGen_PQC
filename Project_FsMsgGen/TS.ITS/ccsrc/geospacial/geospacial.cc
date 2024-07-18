#include "geospacial.hh"

coordinates::coordinates() {
  latitude  = .0;
  longitude = .0;
  altitude  = .0;
}

coordinates::coordinates(const double p_latitude, const double p_longitude, const double p_altitude) {
  latitude  = p_latitude;
  longitude = p_longitude;
  altitude  = p_altitude;
}

const bool coordinates::is_valid_coordinates() const {
  if (latitude > -90.0L && latitude < 90.L && longitude > -180.0L && longitude < 180L) {
    return true;
  } else if (latitude > -HALFPI && latitude < HALFPI && longitude > -PI && longitude < PI) {
    return true;
  }
  return false;
}

const double coordinates::distance_less_than_1km(const coordinates &p) const { // this position has origin
  double     a = HALFPI - latitude;
  double     b = HALFPI - p.latitude;
  double     c = sqrt(a * a + b * b - 2 * a * b * cos(p.longitude - longitude));
  return (double)(R * c);
}

const double coordinates::distance(const coordinates &p) const { // this position has origin
  double     dlon = p.longitude - longitude;
  double     dlat = p.latitude - latitude;
  double     s1   = sin(dlat / 2);
  double     s2   = sin(dlon / 2);
  double     a    = (s1 * s1) + cos(latitude) * cos(p.latitude) * (s2 * s2);
  double     c    = 2 * asin(std::min((double)1.0L, sqrt(a)));
  return (double)(R * c);
}

point::point() {
  x = .0;
  y = .0;
  z = .0;
}

point::point(const double p_x, const double p_y, const double p_z) {
  x = p_x;
  y = p_y;
  z = p_z;
}

int geospacial::load_countries_map(const std::string &p_file) {
  std::ifstream ifs;
  ifs.open(p_file);
  if (ifs.fail()) {
    ifs.close();
    return -1;
  }

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  Json::Value    root;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cerr << errs << std::endl;
    ifs.close();
    return -1;
  }
  // Get the value of the member of root named 'encoding', return a 'null' value if there is no such member.
  const Json::Value plugins = root["features"];
  for (Json::Value::const_iterator it = plugins.begin(); it != plugins.end(); it++) {
    const Json::Value    v = *it;
    Json::Value::Members m = v.getMemberNames();
    std::string          key;
    std::vector<polygon> coord;
    for (std::vector<std::string>::const_iterator i = m.cbegin(); i != m.cend(); ++i) {
      if (std::string(*i).compare("properties") == 0) {
        key = process_properties(v[*i]);
      } else if (std::string(*i).compare("geometry") == 0) {
        coord = process_geometry(v[*i]);
      }
    }
    std::pair<std::map<std::string, std::vector<polygon>>::iterator, bool> ret = _countries.insert(std::pair<std::string, std::vector<polygon>>(key, coord));
    if (ret.second == false) {
      // Element already exist, add update it
      // std::cerr << "element already existed: " << key << " - " << coord.size() << std::endl;
      _countries[key].insert(_countries[key].end(), coord.begin(), coord.end());
    }
  } // End of 'for' statement
  ifs.close();

  return 0;
}

const int geospacial::is_in_country(const std::string &p_country, const coordinates &p_position) {
  if (_countries.size() == 0) {
    return -1;
  }
  std::map<std::string, std::vector<polygon>>::const_iterator it = _countries.find(p_country);
  if (it == _countries.cend()) {
    return -1;
  }

  point p = wgs84_to_point(p_position.latitude, p_position.longitude);
  for (auto ik = it->second.cbegin(); ik != it->second.cend(); ++ik) {
    if (is_inside_polygon(*ik, p) == 0) {
      return 0;
    }
  } // End of 'for' statement

  return -1;
}

const std::string geospacial::process_properties(const Json::Value p_properties) { return p_properties["iso_n3"].asString(); }

const std::vector<polygon> geospacial::process_geometry(const Json::Value p_geometry) {
  std::vector<polygon> ar;
  for (Json::Value::const_iterator it = p_geometry["coordinates"].begin(); it != p_geometry["coordinates"].end(); it++) {
    std::vector<polygon> polygons = extract_polygons(*it);
    ar.insert(ar.end(), polygons.begin(), polygons.end());
  }

  return ar;
}

const std::vector<polygon> geospacial::extract_polygons(const Json::Value p_polygon) {
  std::vector<polygon>        l;
  Json::Value::const_iterator it = p_polygon.begin();
  Json::Value::const_iterator ik = static_cast<const Json::Value>(*it).begin();
  if (static_cast<const Json::Value>(*ik).isArray()) {
    l = extract_polygons(*it);
  } else if (static_cast<const Json::Value>(*ik).isDouble()) {
    polygon p;
    for (; it != p_polygon.end(); it++) {
      ik = static_cast<const Json::Value>(*it).begin();
      coordinates c(static_cast<const Json::Value>(*++ik).asDouble(), // latitude
                    static_cast<const Json::Value>(*ik).asDouble(),   // longitude
                    .0);
      p.push_back(std::make_tuple(c, wgs84_to_point(c.latitude, c.longitude)));
    } // End of 'for' statement
    l.push_back(p);
  } else {
    std::cerr << "extract_polygons: wrong type" << std::endl;
  }

  return l;
}

const point geospacial::wgs84_to_point(const double p_latitude, const double p_longitude) const {
  double latitude_rad  = p_latitude * PI / 180.0L;
  double longitude_rad = p_longitude * PI / 180.0L;
  return point((double)(R * cos(latitude_rad) * cos(longitude_rad)), (double)(R * cos(latitude_rad) * sin(longitude_rad)), (double)(R * sin(latitude_rad)));
}

const int geospacial::is_inside_polygon(const polygon &p_polygon, const point &q) const {
  int    i;
  double angle = .0L;
  point  p1;
  point  p2;
  int    n = p_polygon.size();

  for (i = 0; i < n; i++) {
    // std::clog << "is_inside_polygon: Processing vertex " << i << " -> " << (i + 1) % n << std::endl;
    point p     = std::get<1>(p_polygon[i]);
    point pnext = std::get<1>(p_polygon[(i + 1) % n]);
    p1.x        = p.x - q.x;
    p1.y        = p.y - q.y;
    p2.x        = pnext.x - q.x;
    p2.y        = pnext.y - q.y;
    angle += CalcAngleSum_2d(p1, p2);
    // std::clog << "is_inside_polygon: New angle " << angle << std::endl;
  }
  // std::clog << "is_inside_polygon: Final angle " << fabs(angle) << std::endl;

  if (fabs(angle) < PI)
    return -1;
  else
    return 0;
}

const double geospacial::CalcAngleSum_2d(const point &p1, const point &p2) const {
  double theta1 = atan2(p1.y, p1.x);
  double theta2 = atan2(p2.y, p2.x);
  double dtheta = theta2 - theta1;
  while (dtheta > PI)
    dtheta -= TWOPI;
  while (dtheta < -PI)
    dtheta += TWOPI;

  return (dtheta);
}
