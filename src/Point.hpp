#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
  public:
    double x;
    double y;

    Point () {}
    Point ( double x, double y ) : x(x), y(y) {}

    Point operator - ( Point const &point ) {
      return { point.x - x, point.y - y };
    }

    Point operator + ( Point const &point ) {
      return { point.x + x, point.y + y };
    }

    void print () const {
      std::cout << "X:" << x << "," << "Y:" << y << std::endl;
    }
};

#endif
