#include "util.hpp"

double random( double min, double max ) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis( min, max );
  return dis(gen);
}

point_t pointToBoostPoint( const Point& point ) {
  return point_t( point.x, point.y );
}

Point boostPointToPoint( const point_t& boostPoint ) {
  return { boostPoint.get<0>(), boostPoint.get<1>() };
}

polygon_t polygonToBoostPolygon( const Polygon& poly ) {
  polygon_t boostPoly;
  for( unsigned int i = 0; i < poly.points().size(); i++ ) {
    boostPoly.outer().push_back( point_t(poly.points()[i].x, poly.points()[i].y));
  }
  geom::correct( boostPoly );
  return boostPoly;
}

Polygon boostPolygonToPolygon( const polygon_t& boostPoly ) {
  std::vector<Point> newPoints;
  for( unsigned int i = 0; i < boostPoly.outer().size(); i++ ) {
    newPoints.push_back(
      { boostPoly.outer()[i].get<0>(), boostPoly.outer()[i].get<1>() }
    );
  }
  Polygon newPoly;
  newPoly.setPoints(newPoints);

  return newPoly;
}