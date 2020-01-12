#include <SDL.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <queue>

#include "Timer.h"
#include "MovingRenderablePolygon.h"
#include "transform.h"

enum SIDE { UP, RIGHT, DOWN, LEFT };

const int SCREEN_ORIGIN_X = 0;
const int SCREEN_ORIGIN_Y = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const int GAMESPACE_ORIGIN_X = -500;
const int GAMESPACE_ORIGIN_Y = -500;
const int GAMESPACE_WIDTH = SCREEN_WIDTH + 500;
const int GAMESPACE_HEIGHT = SCREEN_HEIGHT + 500;

const int INIT_ASTEROIDS = 2;
const int MAX_ASTEROIDS = 2;
const int ASTEROID_RADIUS = 150;
const int ASTEROID_VERTS = 13;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const double pi = boost::math::constants::pi<double>();
const double rad_one_deg = 2 * pi / 360;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const Polygon screenBorder ( {
  { SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y },
  { SCREEN_WIDTH, SCREEN_ORIGIN_Y },
  { SCREEN_WIDTH, SCREEN_HEIGHT },
  { SCREEN_ORIGIN_X, SCREEN_HEIGHT },
  { SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y }
} );

const Polygon gameSpace ( {
  { GAMESPACE_ORIGIN_X, GAMESPACE_ORIGIN_Y },
  { GAMESPACE_WIDTH, GAMESPACE_ORIGIN_Y },
  { GAMESPACE_WIDTH, GAMESPACE_HEIGHT },
  { GAMESPACE_ORIGIN_X, GAMESPACE_HEIGHT },
  { GAMESPACE_ORIGIN_X, GAMESPACE_ORIGIN_Y }
} );

bool init() {
  bool success = true;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialise. SDL_Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    gWindow = SDL_CreateWindow(
      "SDL Sandbox",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN
    );
    if( gWindow == NULL ) {
      printf( "Window could not be created. SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL ) {
        printf( "Renderer could not be created. SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
    }
  }

  return success;
}

void close() {
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;
  SDL_Quit();
}

double random( double min, double max ) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis( min, max );
  return dis(gen);
}

Polygon generatePolygon( Point center, double radius, int numVerts ) {
  std::vector<double> angles;
  for( int i = 0; i < numVerts; i++ ) {
    double angle = random( 0, 1 ) * pi*2;
    angles.push_back(angle);
  }
  std::sort( angles.begin(), angles.end() );

  std::vector<Point> points;
  for ( int i = 0; i < numVerts; i++ ) {
    double x = center.x + cos( angles.at(i) ) * ( radius ); //+ random( -16, 16 ));
    double y = center.y + sin( angles.at(i) ) * ( radius ); //+ random( -16, 16 ));
    points.push_back( { x, y } );
  }
  points.push_back( points.at(0) );

  return Polygon( points );
}

void renderPipeline( SDL_Renderer* renderer, std::vector<MovingRenderablePolygon> objectsToRender ) {
  for( auto it = objectsToRender.begin(); it != objectsToRender.end(); it++ ) {
    it->render( renderer );
  }
}

void renderFrame( SDL_Renderer* renderer, std::vector<MovingRenderablePolygon> objectsToRender ) {
  SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderClear( gRenderer );
  SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

  renderPipeline( gRenderer, objectsToRender );

  SDL_RenderPresent( gRenderer );
}

MovingRenderablePolygon generateAsteroid() {
  double x, y, heading = 0;
  int side = round( random( 0, 3 ) );
  switch ( side ) {
    case UP:
      x = random( GAMESPACE_ORIGIN_X, GAMESPACE_WIDTH );
      y = GAMESPACE_ORIGIN_Y;
      if( x < GAMESPACE_WIDTH / 2 )
        heading = pi + atan( ( GAMESPACE_WIDTH - 2*x ) / GAMESPACE_HEIGHT );
      else if ( x > GAMESPACE_WIDTH / 2 )
        heading = pi - atan( ( GAMESPACE_WIDTH - (( GAMESPACE_WIDTH - x ) * 2)) / GAMESPACE_HEIGHT );
      else
        heading = 2 * pi;
      break;

    case RIGHT:
      y = random( GAMESPACE_ORIGIN_Y, GAMESPACE_HEIGHT );
      x = GAMESPACE_WIDTH;
      if( y < GAMESPACE_HEIGHT / 2 )
        heading = pi - atan( GAMESPACE_WIDTH / ( GAMESPACE_HEIGHT - 2*y ) );
      else if ( y > GAMESPACE_HEIGHT / 2 )
        heading = atan( GAMESPACE_WIDTH / ( GAMESPACE_HEIGHT - (( GAMESPACE_HEIGHT - y ) * 2)) );
      else
        heading = 1.5 * pi;
      break;

    case DOWN:
      x = random( GAMESPACE_ORIGIN_X, GAMESPACE_WIDTH );
      y = GAMESPACE_HEIGHT;
      if ( x < GAMESPACE_WIDTH / 2 )
        heading = ( 1.5 * pi ) + atan( GAMESPACE_HEIGHT / ( GAMESPACE_WIDTH - 2*x) );
      else if ( x > GAMESPACE_WIDTH / 2 )
        heading = atan( ( GAMESPACE_WIDTH - (( GAMESPACE_WIDTH - x ) * 2)) / GAMESPACE_HEIGHT );
      else
        heading = 0;
      break;

    case LEFT:
      y = random( GAMESPACE_ORIGIN_Y, GAMESPACE_HEIGHT );
      x = GAMESPACE_ORIGIN_X;
      if( y < GAMESPACE_HEIGHT / 2 )
        heading = ( 1.5 * pi ) - atan( ( GAMESPACE_HEIGHT - 2*y ) / GAMESPACE_WIDTH );
      else if ( y > GAMESPACE_HEIGHT / 2 )
        heading = ( 1.5 * pi ) + atan( ( GAMESPACE_HEIGHT - (( GAMESPACE_HEIGHT - y ) * 2)) / GAMESPACE_WIDTH );
      else
        heading = 2.5 * pi;
      break;
  } 

  MovingRenderablePolygon asteroid = MovingRenderablePolygon(generatePolygon( { x, y }, ASTEROID_RADIUS, ASTEROID_VERTS ));
  asteroid.heading = heading;
  return asteroid;
}

std::vector<MovingRenderablePolygon> generateAsteroids( int numAsteroids ) {
  std::vector<MovingRenderablePolygon> asteroids;
  for( int i = 0; i < numAsteroids; i++ ) {
    asteroids.push_back(generateAsteroid());
  }
  return asteroids;
}

std::vector<MovingRenderablePolygon> updateAsteroids( std::vector<MovingRenderablePolygon> asteroids ) {
  std::vector<MovingRenderablePolygon> updatedAsteroids;
  for( auto it = asteroids.begin(); it != asteroids.end(); it++ ) {
    if( within( *it, gameSpace )) {
      MovingRenderablePolygon updatedAsteroid(translate2D( rotate2D( *it, 0.002, true ), 2 ));
      updatedAsteroids.push_back( updatedAsteroid );
    } 
    else {
      updatedAsteroids.push_back( generateAsteroid() );
      //std::cout << "New Asteroid." << std::endl;
    }
  }
  return updatedAsteroids;
}

bool detectCollision( Polygon p1, Polygon p2 ) {
  // Create a list of vectors perpendicular to polygon edges.
  std::vector<Point> normals;
  for ( int i = 0; i < p1.points.size()-1; i++ ) {
    Point edge = p1.points[i+1] - p1.points[i];
    Point normal = { edge.y, -edge.x };
    normals.push_back(normal);
  }
  for ( int i = 0; i < p2.points.size()-1; i++ ) {
    Point edge = p2.points[i+1] - p2.points[i];
    Point normal = { edge.y, -edge.x };
    normals.push_back(normal);
  }

  // Project vertices from polygons onto the perpendicular vectors (normals);
  for( auto n = normals.begin(); n != normals.end(); n++ ) {
    // P1
    double dp = dotProduct(*n, p1.points[0]);
    double p1Min = dp;
    double p1Max = dp;
    for( auto i = p1.points.begin(); i != p1.points.end(); i++ ) {
      dp = dotProduct(*i, *n);
      if( dp < p1Min ) {
        p1Min = dp;
      }
      if( dp > p1Max ) {
        p1Max = dp;
      }
    }

    // P2
    dp = dotProduct(*n, p2.points[0]);
    double p2Min = dp;
    double p2Max = dp;
    for( auto i = p2.points.begin(); i != p2.points.end(); i++ ) {
      dp = dotProduct(*i, *n);
      if( dp < p2Min ) {
        p2Min = dp;
      }
      if( dp > p2Max ) {
        p2Max = dp;
      }
    }
    double interval;
    if( p1Min < p2Min ) {
      interval = p2Min - p1Max;
    } else {
      interval = p1Min - p2Max;
    }
    if( interval > 1 ) {
      return false;
    }
  }
  return true;
}

void detectCollisions( std::vector<MovingRenderablePolygon> objects ) {
  for( int i = 0; i < objects.size(); i++ ) {
    for( int j = i + 1; j < objects.size(); j++ ) {
      std::cout << " Colliding: " << detectCollision( objects[i], objects[j] ) << std::endl;
    }
  }
}

#ifdef main
# undef main
#endif /* main */

int main( int argc, char* args[] ) {
  if( !init() ) {
    printf( "Failed to initialise.\n" );
  } else {
    bool quit = false;
    SDL_Event e;

    std::vector<Point> points = { 
      { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 },
      { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20 },
      { SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2 },
      { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 }
    };

    MovingRenderablePolygon triangle;
    triangle.points = points;

    std::vector<MovingRenderablePolygon> asteroids = generateAsteroids( INIT_ASTEROIDS );

    double angle = 0.0;
    double angleDelta = 0.08;

    bool move = false;
    bool rotate = false;

    const Uint8 *keystate = NULL;

    std::vector<MovingRenderablePolygon> objectsToRender;
    
    Timer fpsTimer;
    Timer capTimer;
    int countedFrames = 0;
    fpsTimer.start();

    while( !quit ) {
      capTimer.start();
      while( SDL_PollEvent( &e ) != 0 ) {
        keystate = SDL_GetKeyboardState(NULL);
        switch( e.type ) {
          case SDL_QUIT:
            quit = true;
            break;
          case SDL_KEYDOWN:
            if( keystate[SDL_SCANCODE_LEFT] ) {
              angle = angleDelta;
              rotate = true;
            }
            if( keystate[SDL_SCANCODE_RIGHT] ) {
              angle = -angleDelta;
              rotate = true;
            }
            if( keystate[SDL_SCANCODE_UP] ) {
              move = true;
            }
            break;
          case SDL_KEYUP:
            if( !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] ) {
              rotate = false;
            }
            if( !keystate[SDL_SCANCODE_UP] ) {
              move = false;
            }
            break;
        }
      }
      float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
      if( avgFps > 2000000 ) {
        avgFps = 0;
      }

      if( rotate ) {
        triangle = rotate2D( triangle, angle );
      }
      if( move ) {
        triangle = translate2D( triangle, 10, screenBorder );
      }

      asteroids = updateAsteroids( asteroids );

      objectsToRender.push_back( triangle );
      objectsToRender.insert( objectsToRender.end(), asteroids.begin(), asteroids.end() );

      detectCollisions( objectsToRender );

      renderFrame( gRenderer, objectsToRender );
      countedFrames++;
      int frameTicks = capTimer.getTicks();
      if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
        SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
      }

      objectsToRender.clear();
    }
  }
  close();
  return 0;
}