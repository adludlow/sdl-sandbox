#ifndef MOVINGRENDERABLEPOLYGON_H
#define MOVINGRENDERABLEPOLYGON_H

#include "Renderable.h"
#include "Polygon.h"

class MovingRenderablePolygon: Renderable, public Polygon {
  public:
    MovingRenderablePolygon();
    MovingRenderablePolygon( Polygon polygon );
    ~MovingRenderablePolygon();

    void render( SDL_Renderer* renderer );
    void move();

    double heading = 0;
    int velocity = 0;
    double rotation = 0;
};

#endif