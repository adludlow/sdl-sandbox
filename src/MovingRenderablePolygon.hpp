#ifndef MOVINGRENDERABLEPOLYGON_H
#define MOVINGRENDERABLEPOLYGON_H

#include "Renderable.hpp"
#include "MovingPolygon.hpp"

class MovingRenderablePolygon: public Renderable, public MovingPolygon {
  public:
    MovingRenderablePolygon();
    MovingRenderablePolygon( MovingPolygon polygon );
    ~MovingRenderablePolygon();

    void render( SDL_Renderer* renderer );
    void move();
};

#endif