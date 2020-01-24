#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "Polygon.hpp"

namespace geom = boost::geometry;
namespace trans = geom::strategy::transform;

typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
typedef geom::model::polygon<point_t> polygon_t;

double random( double min, double max );
point_t pointToBoostPoint( const Point& point );
Point boostPointToPoint( const point_t& boostPoint );
polygon_t polygonToBoostPolygon( const Polygon& poly );
Polygon boostPolygonToPolygon( const polygon_t& boostPoly );

#endif
