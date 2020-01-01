#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <SDL.h>

#include "Point.h"

struct Polygon {
  std::vector<Point> points;
  Point centroid;

  Polygon() {}
  Polygon( std::vector<Point> points ) : points( points ) {}
};

#endif
