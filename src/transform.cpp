#include <cmath>

#include "transform.h"

point_t pointToBoostPoint( Point& point ) {
  return point_t( point.x, point.y );
}

Point boostPointToPoint( point_t& boostPoint ) {
  return { boostPoint.get<0>(), boostPoint.get<1>() };
}

polygon_t polygonToBoostPolygon( Polygon& poly ) {
  polygon_t boostPoly;
  for( unsigned int i = 0; i < poly.points.size(); i++ ) {
    boostPoly.outer().push_back( point_t(poly.points[i].x, poly.points[i].y));
  }
  geom::correct( boostPoly );
  return boostPoly;
}

Polygon boostPolygonToPolygon( polygon_t& boostPoly ) {
  std::vector<Point> newPoints;
  for( unsigned int i = 0; i < boostPoly.outer().size(); i++ ) {
    newPoints.push_back(
      { boostPoly.outer()[i].get<0>(), boostPoly.outer()[i].get<1>() }
    );
  }
  Polygon newPoly;
  newPoly.points = newPoints;

  return newPoly;
}

bool within( Polygon inner, Polygon outer ) {
  polygon_t innerBoostPoly = polygonToBoostPolygon( inner );
  polygon_t outerBoostPoly = polygonToBoostPolygon( outer );
  if( geom::within( innerBoostPoly, outerBoostPoly )) {
    return true;
  } 
  return false;
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

  newPoly.centroid = boostPointToPoint( centroid );

  return newPoly;
}

MovingPolygon translate2D( MovingPolygon polygon, int magnitude, Polygon border ) {
  MovingPolygon translated = translate2D( polygon, magnitude );
  polygon_t translatedBoostPoly = polygonToBoostPolygon( translated );
  polygon_t boostBorder = polygonToBoostPolygon( border );
  if( geom::within( translatedBoostPoly, boostBorder )) {
    return translated;
  } else {
    return polygon;
  }
}

MovingPolygon rotate2D( MovingPolygon polygon, double angle, bool keepHeading ) {
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

  if( !keepHeading ) {
    if( polygon.heading + angle > 2 * M_PI ) {
      newPoly.heading = polygon.heading + angle - 2 * M_PI;
    } else if( polygon.heading + angle < 0 ) {
      newPoly.heading = polygon.heading + angle + 2 * M_PI;
    } else {
      newPoly.heading = polygon.heading + angle;
    }
  } else {
    newPoly.heading = polygon.heading;
  }

  return newPoly;
}
