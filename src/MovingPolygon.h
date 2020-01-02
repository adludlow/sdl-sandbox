#ifndef MOVINGPOLYGON_H
#define MOVINGPOLYGON_H

#include "Polygon.h"

class MovingPolygon : Polygon {
  public:
    double heading = 0;
    int velocity = 0;
    double rotation = 0;

    MovingPolygon() {}
    MovingPolygon( Polygon poly ) : Polygon { poly.points } {}
};

#endif
