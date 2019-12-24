#include <vector>

#include "Point.h"

class Polygon {
  public:
    Polygon();
    Polygon( std::vector<Point> points );
    ~Polygon();

    std::vector<Point> getPoints();
    void setPoints( std::vector<Point> points );

  private:
    std::vector<Point> points;
};