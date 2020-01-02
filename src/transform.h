#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "MovingRenderablePolygon.h"

namespace geom = boost::geometry;
namespace trans = geom::strategy::transform;

typedef geom::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
typedef geom::model::polygon<point_t> polygon_t;

MovingRenderablePolygon rotate2D( MovingRenderablePolygon polygon, double angle, bool keepHeading = false );
MovingRenderablePolygon translate2D( MovingRenderablePolygon polygon, int magnitude );
MovingRenderablePolygon translate2D( MovingRenderablePolygon polygon, int magnitude, Polygon border );
polygon_t polygonToBoostPolygon( Polygon& poly );

bool within( Polygon inner, Polygon outer );
