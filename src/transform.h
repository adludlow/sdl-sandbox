#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "MovingPolygon.h"

namespace geom = boost::geometry;
namespace trans = geom::strategy::transform;

typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
typedef geom::model::polygon<point_t> polygon_t;

MovingPolygon rotate2D( MovingPolygon polygon, double angle );
MovingPolygon translate2D( MovingPolygon polygon, int magnitude );
MovingPolygon translate2D( MovingPolygon polygon, int magnitude, Polygon border );
polygon_t polygonToBoostPolygon( Polygon& poly );
