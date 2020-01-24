#include <vector>
#include <iostream>

#include "Asteroid.hpp"
#include "transform.hpp"

Asteroid::Asteroid() {}

Asteroid::Asteroid( Polygon polygon ) : Polygon(polygon) {}

Asteroid::~Asteroid() {}

void Asteroid::render( SDL_Renderer* renderer ) {
  std::vector<SDL_Point> sdlPoints;
  for( auto i : points() ) {
    sdlPoints.push_back({ round(i.x), round(i.y)});
  }

  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
}

void Asteroid::move() {
  translate2D( *this, heading, velocity );
}

void Asteroid::rotate( bool keepHeading ) {

}
