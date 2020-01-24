#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <boost/math/constants/constants.hpp>

enum SIDE { UP, RIGHT, DOWN, LEFT };

const int SCREEN_ORIGIN_X = 0;
const int SCREEN_ORIGIN_Y = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const int GAMESPACE_ORIGIN_X = 100;
const int GAMESPACE_ORIGIN_Y = 100;
const int GAMESPACE_WIDTH = SCREEN_WIDTH - 100;
const int GAMESPACE_HEIGHT = SCREEN_HEIGHT - 100;

const int CREATION_ZONE_ORIGIN_X = 200;
const int CREATION_ZONE_ORIGIN_Y = 200;
const int CREATION_ZONE_WIDTH = SCREEN_WIDTH - 200;
const int CREATION_ZONE_HEIGHT = SCREEN_HEIGHT - 200;

const int INIT_ASTEROIDS = 6;
const int MAX_ASTEROIDS = 6;
const int ASTEROID_RADIUS = 50;
const int ASTEROID_VERTS = 13;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const double pi = boost::math::constants::pi<double>();
const double rad_one_deg = 2 * pi / 360;

#endif
