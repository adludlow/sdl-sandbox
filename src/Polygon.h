#include <vector>
#include <SDL.h>

#include "Point.h"

class Polygon {
  public:
    Polygon();
    Polygon( std::vector<Point> points );
    ~Polygon();

    std::vector<Point> getPoints();
    void setPoints( std::vector<Point> points );

    void render( SDL_Renderer* renderer );
    void rotate2D( double angle );

  private:
    std::vector<Point> points;
};