#include "Polygon.h"

Polygon::Polygon() : Polygon( std::vector<Point>() ) {}

Polygon::Polygon( std::vector<Point> points ) { this->points = points; }

Polygon::~Polygon() {}

std::vector<Point> Polygon::getPoints() { return points; }

void Polygon::setPoints( std::vector<Point> points ) { this->points = points; }