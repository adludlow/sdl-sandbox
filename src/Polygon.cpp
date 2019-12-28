#include <iostream>
#include <SDL.h>
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "Polygon.h"

Polygon::Polygon() : Polygon( std::vector<Point>() ) {}

Polygon::Polygon( std::vector<Point> points ) {
  this->points = points;
}

Polygon::~Polygon() {}

std::vector<Point> Polygon::getPoints() { return points; }

void Polygon::setPoints( std::vector<Point> points ) { this->points = points; }

void Polygon::render( SDL_Renderer* renderer ) {
  std::vector<SDL_Point> sdlPoints;
  sdlPoints.resize(points.size());
  std::transform(
    points.begin(),
    points.end(),
    sdlPoints.begin(),
    []( Point p ) -> SDL_Point {
      return { round(p.x), round(p.y) };
    }
  );

  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
}

void Polygon::rotate2D( double angle ) {
  namespace geom = boost::geometry;
  namespace trans = geom::strategy::transform;

  typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
  typedef geom::model::polygon<point_t> polygon_t;

  polygon_t poly;
  for( unsigned int i = 0; i < points.size(); i++ ) {
    poly.outer().push_back( point_t(points[i].x, points[i].y));
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

  trans::rotate_transformer<geom::degree, double, 2, 2> rotate( angle );
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
  points = newPoints;
}