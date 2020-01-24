#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "util.hpp"
#include "MovingPolygon.hpp"

void rotate2D( MovingPolygon& polygon, double angle, bool keepHeading = false );
void translate2D( Polygon& polygon, double heading, int magnitude );
MovingPolygon translate2D( MovingPolygon polygon, int magnitude );
void translate2D( MovingPolygon& polygon, int magnitude, Polygon border );
double dotProduct( Point v1, Point v2 );

bool within( const Polygon& inner, const Polygon& outer );

#endif
