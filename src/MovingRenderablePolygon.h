#ifndef MOVINGRENDERABLEPOLYGON_H
#define MOVINGRENDERABLEPOLYGON_H

#include "Renderable.h"
#include "MovingPolygon.h"

class MovingRenderablePolygon: Renderable, public MovingPolygon {
  public:
    MovingRenderablePolygon();
    MovingRenderablePolygon( MovingPolygon polygon );
    ~MovingRenderablePolygon();

    void render( SDL_Renderer* renderer );
    void move();
};

#endif