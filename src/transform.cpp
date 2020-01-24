#include <cmath>
#include <boost/geometry/arithmetic/dot_product.hpp>

#include "transform.hpp"
#include "constants.hpp"

bool within( const Polygon& inner, const Polygon& outer ) {
  point_t centroid;
  geom::centroid( polygonToBoostPolygon(inner), centroid );
  //polygon_t innerBoostPoly = polygonToBoostPolygon( inner );
  polygon_t outerBoostPoly = polygonToBoostPolygon( outer );
  if( geom::within( centroid, outerBoostPoly )) {
    return true;
  } 
  return false;
}

void translate2D( Polygon& polygon, double heading, int magnitude ) {
  polygon_t poly = polygonToBoostPolygon( polygon );

  double deltaX = -magnitude * sin(heading);
  double deltaY = -magnitude * cos(heading);

  polygon_t poly_t;
  trans::translate_transformer<double, 2, 2> translate( 
    deltaX,
    deltaY
  );
  geom::transform( poly, poly_t, translate );

  Polygon newPoly(boostPolygonToPolygon( poly_t ));
  polygon.setPoints(newPoly.points());

  point_t centroid;
  geom::centroid( poly, centroid );

  polygon.setCentroid(boostPointToPoint( centroid ));
}

MovingPolygon translate2D( MovingPolygon polygon, int magnitude ) {
  polygon_t poly = polygonToBoostPolygon( polygon );

  double deltaX = -magnitude * sin(polygon.heading);
  double deltaY = -magnitude * cos(polygon.heading);

  polygon_t poly_t;
  trans::translate_transformer<double, 2, 2> translate( 
    deltaX,
    deltaY
  );
  geom::transform( poly, poly_t, translate );

  MovingPolygon newPoly(boostPolygonToPolygon( poly_t ));
  newPoly.heading = polygon.heading;

  point_t centroid;
  geom::centroid( poly, centroid );

  newPoly.setCentroid(boostPointToPoint( centroid ));

  return newPoly;
}

void translate2D( MovingPolygon& polygon, int magnitude, Polygon border ) {
  MovingPolygon translated = translate2D( polygon, magnitude );
  polygon_t translatedBoostPoly = polygonToBoostPolygon( translated );
  polygon_t boostBorder = polygonToBoostPolygon( border );
  if( geom::within( translatedBoostPoly, boostBorder )) {
    polygon.setPoints(translated.points());
  }
}

void rotate2D( MovingPolygon& polygon, double angle, bool keepHeading ) {
  polygon_t poly = polygonToBoostPolygon( polygon );

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

  MovingPolygon newPoly(boostPolygonToPolygon( result ));
  polygon.setPoints(newPoly.points());

  double heading = 0;
  if( !keepHeading ) {
    if( polygon.heading + angle > 2 * pi ) {
      heading = polygon.heading + angle - 2 * pi;
    } else if( polygon.heading + angle < 0 ) {
      heading = polygon.heading + angle + 2 * pi;
    } else {
      heading = polygon.heading + angle;
    }
  } else {
    heading = polygon.heading;
  }
  polygon.heading = heading;
}

double dotProduct( Point v1, Point v2 ) {
  return geom::dot_product( pointToBoostPoint(v1), pointToBoostPoint(v2) );
}