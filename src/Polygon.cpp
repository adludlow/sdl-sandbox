#include <iostream>

#include "Polygon.hpp"
#include "constants.hpp"
#include "util.hpp"

Polygon::Polygon( Point center, double radius, int numVerts ) {
  std::vector<double> angles;
  for( int i = 0; i < numVerts; i++ ) {
    double angle = random( 0, 1 ) * pi*2;
    angles.push_back(angle);
  }
  std::sort( angles.begin(), angles.end() );

  points_ = std::vector<Point>();
  for ( int i = 0; i < numVerts; i++ ) {
    double x = center.x + cos( angles.at(i) ) * ( radius ); //+ random( -16, 16 ));
    double y = center.y + sin( angles.at(i) ) * ( radius ); //+ random( -16, 16 ));
    points_.push_back( { x, y } );
  }
  points_.push_back( points_.at(0) );
}

std::vector<Point> Polygon::points() const {
  return points_;
}

void Polygon::setPoints(const std::vector<Point>& points) {
  points_.clear();
  points_.insert(points_.begin(), points.begin(), points.end());
}

Point Polygon::centroid() const {
  return centroid_;
}

void Polygon::setCentroid(const Point& centroid) {
  centroid_ = centroid;
}

void Polygon::print() {
  for( int i = 0; i < points_.size(); i++ ) {
    std::cout << i << " " << points_[i].x << " " << points_[i].y << std::endl;
  }
}