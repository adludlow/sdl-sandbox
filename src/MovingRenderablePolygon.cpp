#include <algorithm>

#include "MovingRenderablePolygon.h"

MovingRenderablePolygon::MovingRenderablePolygon() {}

MovingRenderablePolygon::MovingRenderablePolygon( Polygon polygon ) : Polygon( polygon ) {}

MovingRenderablePolygon::~MovingRenderablePolygon() {}

void MovingRenderablePolygon::render( SDL_Renderer* renderer ) {
  std::vector<SDL_Point> sdlPoints;
  sdlPoints.resize(points.size());
  std::transform(
    points.begin(),
    points.end(),
    sdlPoints.begin(),
    []( Point p ) -> SDL_Point {
      return { round(p.x), round(p.y) };
    }
  );

  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
}

void MovingRenderablePolygon::move() {

}
