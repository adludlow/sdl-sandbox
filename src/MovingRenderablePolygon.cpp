#include <iostream>
#include <algorithm>

#include "MovingRenderablePolygon.hpp"

MovingRenderablePolygon::MovingRenderablePolygon() {}

MovingRenderablePolygon::MovingRenderablePolygon( MovingPolygon polygon ) : MovingPolygon( polygon ) {}

MovingRenderablePolygon::~MovingRenderablePolygon() {}

void MovingRenderablePolygon::render( SDL_Renderer* renderer ) {
  std::vector<SDL_Point> sdlPoints;
  for( auto i : points() ) {
    sdlPoints.push_back({ round(i.x), round(i.y)});
  }

  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
}

void MovingRenderablePolygon::move() {

}
