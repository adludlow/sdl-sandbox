#ifndef MOVINGPOLYGON_H
#define MOVINGPOLYGON_H

#include "Polygon.h"

struct MovingPolygon : Polygon {
  double heading = 0;
  int velocity = 0;
};

#endif
