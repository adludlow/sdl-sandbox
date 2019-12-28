#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <cmath>

#include "transform.h"

namespace geom = boost::geometry;
namespace trans = geom::strategy::transform;

typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
typedef geom::model::polygon<point_t> polygon_t;

MovingPolygon translate2D( MovingPolygon polygon, int magnitude ) {
  polygon_t poly;
  for( unsigned int i = 0; i < polygon.points.size(); i++ ) {
    poly.outer().push_back( point_t(polygon.points[i].x, polygon.points[i].y));
  }
  geom::correct( poly );

  double deltaX = -magnitude * sin(polygon.heading);
  double deltaY = -magnitude * cos(polygon.heading);

  polygon_t poly_t;
  trans::translate_transformer<double, 2, 2> translate( 
    deltaX,
    deltaY
  );
  geom::transform( poly, poly_t, translate );

  std::vector<Point> newPoints;
  for( unsigned int i = 0; i < poly_t.outer().size(); i++ ) {
    newPoints.push_back(
      { poly_t.outer()[i].get<0>(), poly_t.outer()[i].get<1>() }
    );
  }
  MovingPolygon newPoly;
  newPoly.points = newPoints;
  newPoly.heading = polygon.heading;

  return newPoly;
}

MovingPolygon rotate2D( MovingPolygon polygon, double angle ) {
  polygon_t poly;
  for( unsigned int i = 0; i < polygon.points.size(); i++ ) {
    poly.outer().push_back( point_t(polygon.points[i].x, polygon.points[i].y));
  }
  geom::correct( poly );

  // Calculate centroid
  point_t centroid;
  geom::centroid( poly, centroid );

  // Translate to origin
  polygon_t poly_o;
  trans::translate_transformer<double, 2, 2> translate( 
    -centroid.get<0>(),
    -centroid.get<1>()
  );
  geom::transform( poly, poly_o, translate );

  trans::rotate_transformer<geom::radian, double, 2, 2> rotate( angle );
  polygon_t poly_c;
  geom::transform( poly_o, poly_c, rotate );

  trans::translate_transformer<double, 2, 2> translateBack( 
    centroid.get<0>(),
    centroid.get<1>()
  );
  polygon_t result;
  geom::transform( poly_c, result, translateBack );

  std::vector<Point> newPoints;
  for( unsigned int i = 0; i < result.outer().size(); i++ ) {
    newPoints.push_back(
      { result.outer()[i].get<0>(), result.outer()[i].get<1>() }
    );
  }
  MovingPolygon newPoly;
  newPoly.points = newPoints;
  if( polygon.heading + angle > 2 * M_PI ) {
    newPoly.heading = polygon.heading + angle - 2 * M_PI;
  } else if( polygon.heading + angle < 0 ) {
    newPoly.heading = polygon.heading + angle + 2 * M_PI;
  } else {
    newPoly.heading = polygon.heading + angle;
  }

  return newPoly;
}