#include "Point.h"

Point::Point( int x, int y ) {
  setX(x);
  setY(y);
}

Point::Point() : Point( 0, 0 ) {}

Point::~Point() {}

void Point::setX( int x ) { this->x = x; }

void Point::setY( int y ) { this->y = y; }

int Point::getX() { return x; }

int Point::getY() { return y; }