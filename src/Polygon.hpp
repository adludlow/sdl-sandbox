#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <SDL.h>

#include "Point.hpp"

class Polygon {
  public:
    Polygon() : points_(std::vector<Point>()) {}
    Polygon( std::vector<Point> points ) : points_( points ) {}
    Polygon( Point center, double radius, int numVerts );

    std::vector<Point> points() const;
    void setPoints(const std::vector<Point>& points);
    Point centroid() const;
    void setCentroid(const Point& centroid);

    void print();

  private:
    std::vector<Point> points_;
    Point centroid_;
};

#endif
