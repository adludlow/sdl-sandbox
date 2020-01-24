#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Renderable.hpp"
#include "Movable.hpp"
#include "Rotatable.hpp"
#include "Polygon.hpp"

class Asteroid: public Renderable, public Movable, public Polygon {
  public:
    Asteroid();
    Asteroid( Polygon polygon );
    ~Asteroid();

    void render( SDL_Renderer* renderer );
    void move();
    void rotate(bool keepHeading);

    double heading;
    int velocity;
    double rotation;
};

#endif
