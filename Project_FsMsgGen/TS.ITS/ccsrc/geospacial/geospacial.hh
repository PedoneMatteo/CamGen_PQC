#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <cmath>

#include <json/json.h>

#define EPSILON 0.0000001
#define MODULUS(p) (sqrt(p.x * p.x + p.y * p.y + p.z * p.z))
#define PI 3.1415926535897932384626435
#define HALFPI 1.5707963267948966192313216916398
#define TWOPI 6.283185307179586476925287
//#define RTOD 57.2957795
#define R 6378140.0 // Radius of the earth in meters

class coordinates {
public:
  double latitude;
  double longitude;
  double altitude;

public:
  coordinates();
  coordinates(const double p_latitude, const double p_longitude, const double p_altitude);
  virtual ~coordinates(){};
  inline const coordinates to_radian() const { return coordinates(latitude * PI / 180, longitude * PI / 180, altitude); };
  const bool               is_valid_coordinates() const;
  ;
  const double distance_less_than_1km(const coordinates &p) const; // this position has origin
  const double distance(const coordinates &p) const;               // this position has origin
};

class point {
public:
  double x, y, z;

public:
  point();
  point(const double p_x, const double p_y, const double p_z);
};

typedef std::vector<std::tuple<coordinates, point>> polygon;

class geospacial {
  std::map<std::string, std::vector<polygon>> _countries;
  const point                                 wgs84_to_point(const double p_latitude, const double p_longitude) const;
  const std::string                           process_properties(const Json::Value p_properties);
  const std::vector<polygon>                  process_geometry(const Json::Value p_geometry);
  const std::vector<polygon>                  extract_polygons(const Json::Value p_polygon);
  const double                                CalcAngleSum_2d(const point &p1, const point &p2) const;
  const int                                   is_inside_polygon(const polygon &p_polygon, const point &q) const;

public:
  geospacial(){};
  virtual ~geospacial(){};
  int       load_countries_map(const std::string &p_file);
  const int is_in_country(const std::string &p_country, const coordinates &p_position);
};
