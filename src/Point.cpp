#include "Point.h"

Point::Point( double x, double y ) {
  setX(x);
  setY(y);
}

Point::Point() : Point( 0, 0 ) {}

Point::~Point() {}

void Point::setX( double x ) { this->x = x; }

void Point::setY( double y ) { this->y = y; }

double Point::getX() { return x; }

double Point::getY() { return y; }