#include <SDL.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <memory>

#include "Timer.hpp"
#include "Asteroid.hpp"
#include "MovingRenderablePolygon.hpp"
#include "transform.hpp"
#include "constants.hpp"
#include "util.hpp"

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

void renderPipeline( SDL_Renderer* renderer, std::vector<Renderable*> objectsToRender ) {
  for( auto it = objectsToRender.begin(); it != objectsToRender.end(); it++ ) {
    (*it)->render( renderer );
  }
}

void renderFrame( SDL_Renderer* renderer, std::vector<Renderable*> objectsToRender ) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderClear( renderer );
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

  renderPipeline( renderer, objectsToRender );

  SDL_RenderPresent( renderer );
}

std::unique_ptr<Asteroid> generateAsteroid() {
  double x, y, heading = 0;
  int side = round( random( 0, 3 ) );
  switch ( side ) {
    case UP:
      x = random( CREATION_ZONE_ORIGIN_X, CREATION_ZONE_WIDTH );
      y = CREATION_ZONE_ORIGIN_Y;
      if( x < CREATION_ZONE_WIDTH / 2 )
        heading = pi + atan( ( CREATION_ZONE_WIDTH - 2*x ) / CREATION_ZONE_HEIGHT );
      else if ( x > CREATION_ZONE_WIDTH / 2 )
        heading = pi - atan( ( CREATION_ZONE_WIDTH - (( CREATION_ZONE_WIDTH - x ) * 2)) / CREATION_ZONE_HEIGHT );
      else
        heading = 2 * pi;
      break;

    case RIGHT:
      y = random( CREATION_ZONE_ORIGIN_Y, CREATION_ZONE_HEIGHT );
      x = CREATION_ZONE_WIDTH;
      if( y < CREATION_ZONE_HEIGHT / 2 )
        heading = pi - atan( CREATION_ZONE_WIDTH / ( CREATION_ZONE_HEIGHT - 2*y ) );
      else if ( y > CREATION_ZONE_HEIGHT / 2 )
        heading = atan( CREATION_ZONE_WIDTH / ( CREATION_ZONE_HEIGHT - (( CREATION_ZONE_HEIGHT - y ) * 2)) );
      else
        heading = 1.5 * pi;
      break;

    case DOWN:
      x = random( CREATION_ZONE_ORIGIN_X, CREATION_ZONE_WIDTH );
      y = CREATION_ZONE_HEIGHT;
      if ( x < CREATION_ZONE_WIDTH / 2 )
        heading = ( 1.5 * pi ) + atan( CREATION_ZONE_HEIGHT / ( CREATION_ZONE_WIDTH - 2*x) );
      else if ( x > CREATION_ZONE_WIDTH / 2 )
        heading = atan( ( CREATION_ZONE_WIDTH - (( CREATION_ZONE_WIDTH - x ) * 2)) / CREATION_ZONE_HEIGHT );
      else
        heading = 0;
      break;

    case LEFT:
      y = random( CREATION_ZONE_ORIGIN_Y, CREATION_ZONE_HEIGHT );
      x = CREATION_ZONE_ORIGIN_X;
      if( y < CREATION_ZONE_HEIGHT / 2 )
        heading = ( 1.5 * pi ) - atan( ( CREATION_ZONE_HEIGHT - 2*y ) / CREATION_ZONE_WIDTH );
      else if ( y > CREATION_ZONE_HEIGHT / 2 )
        heading = ( 1.5 * pi ) + atan( ( CREATION_ZONE_HEIGHT - (( CREATION_ZONE_HEIGHT - y ) * 2)) / CREATION_ZONE_WIDTH );
      else
        heading = 2.5 * pi;
      break;
  } 

  auto asteroid = std::make_unique<Asteroid>(Polygon( { x, y }, ASTEROID_RADIUS, ASTEROID_VERTS ));
  asteroid->heading = heading;
  asteroid->velocity = 1;
  return asteroid;
}

std::vector<std::unique_ptr<Asteroid>> generateAsteroids( int numAsteroids ) {
  std::vector<std::unique_ptr<Asteroid>> asteroids;
  for( int i = 0; i < numAsteroids; i++ ) {
    asteroids.push_back(generateAsteroid());
  }
  return asteroids;
}

bool detectCollision( const Polygon& p1, const Polygon& p2 ) {
  // Create a list of vectors perpendicular to polygon edges.
  std::vector<Point> normals;
  for ( int i = 0; i < p1.points().size()-1; i++ ) {
    Point edge = p1.points()[i+1] - p1.points()[i];
    Point normal = { edge.y, -edge.x };
    normals.push_back(normal);
  }
  for ( int i = 0; i < p2.points().size()-1; i++ ) {
    Point edge = p2.points()[i+1] - p2.points()[i];
    Point normal = { edge.y, -edge.x };
    normals.push_back(normal);
  }

  // Project vertices from polygons onto the perpendicular vectors (normals);
  for( auto n = normals.begin(); n != normals.end(); n++ ) {
    // P1
    double dp = dotProduct(*n, p1.points()[0]);
    double p1Min = dp;
    double p1Max = dp;
    for( int i = 0; i < p1.points().size(); i++ ) {
      dp = dotProduct(*n, p1.points()[i]);
      if( dp < p1Min ) {
        p1Min = dp;
      }
      if( dp > p1Max ) {
        p1Max = dp;
      }
    }

    // P2
    dp = dotProduct(*n, p2.points()[0]);
    double p2Min = dp;
    double p2Max = dp;
    for( int i = 0; i < p2.points().size(); i++ ) {
      dp = dotProduct(*n, p2.points()[i]);
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
    //std::cout << p1Min << " " << p1Max << ", " << p2Min << " " << p2Max << std::endl;
    if( interval > 1 ) {
      return false;
    }
  }
  return true;
}

void updateAsteroids( std::vector<std::unique_ptr<Asteroid>>& asteroids ) {
  for( auto it = asteroids.begin(); it != asteroids.end(); it++ ) {
    if( within( **it, gameSpace )) {
      (*it)->move();
      (*it)->rotate(true);

    } 
    else {
      // delete asteroid
      asteroids.erase(it);
      asteroids.push_back( generateAsteroid() );
    }
  }
  for( int i = 0; i < asteroids.size(); i++ ) {
    for( int j = i + 1; j < asteroids.size(); j++ ) {
      if( detectCollision( *asteroids[i], *asteroids[j] )) {
        asteroids[j]->heading = asteroids[j]->heading + pi;
        translate2D(*asteroids[j], asteroids[j]->heading, 5);
        asteroids[i]->heading = asteroids[i]->heading + pi;
        translate2D(*asteroids[i], asteroids[i]->heading, 5);
      }
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

    auto triangle = MovingRenderablePolygon();
    triangle.setPoints(points);

    std::vector<std::unique_ptr<Asteroid>> asteroids = generateAsteroids( INIT_ASTEROIDS );

    double angle = 0.0;
    double angleDelta = 0.08;

    bool move = false;
    bool rotate = false;

    const Uint8 *keystate = NULL;

    std::vector<Renderable*> objectsToRender;
    
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
        rotate2D( triangle, angle );
      }
      if( move ) {
        translate2D( triangle, 10, screenBorder );
      }

      updateAsteroids( asteroids );

      objectsToRender.push_back( &triangle );
      for( auto i = asteroids.begin(); i != asteroids.end(); i++ ) {
        objectsToRender.push_back((*i).get());
      }

      //detectCollisions( objectsToRender );
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