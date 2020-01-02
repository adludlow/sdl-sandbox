#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <SDL.h>

class Renderable {
  public:
    virtual void render( SDL_Renderer* renderer ) = 0;
};

#endif
