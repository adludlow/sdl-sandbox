#ifndef POINT_H
#define POINT_H

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
};

#endif
