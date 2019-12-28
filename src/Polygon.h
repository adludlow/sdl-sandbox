#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <SDL.h>

#include "Point.h"

struct Polygon {
  std::vector<Point> points;
};

#endif
