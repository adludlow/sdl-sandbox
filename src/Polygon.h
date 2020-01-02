#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <SDL.h>

#include "Point.h"

class Polygon {
  public:
    Polygon() {}
    Polygon( std::vector<Point> points ) : points( points ) {}

    std::vector<Point> points;
    Point centroid;

};

#endif
