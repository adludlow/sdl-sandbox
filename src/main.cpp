#include <SDL.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <queue>

#include "MovingPolygon.h"
#include "transform.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const int INIT_ASTEROIDS = 100;
const int MAX_ASTEROIDS = 10;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const Polygon screenBorder ( {
  { 0, 0 },
  { SCREEN_WIDTH, 0 },
  { SCREEN_WIDTH, SCREEN_HEIGHT },
  { 0, SCREEN_HEIGHT },
  { 0, 0 }
} );

const Polygon gameSpace ( {
  { -200, -200 },
  { SCREEN_WIDTH + 200, -200 },
  { SCREEN_WIDTH + 200, SCREEN_HEIGHT + 200 },
  { -200, SCREEN_HEIGHT + 200 },
  { -200, -200 }
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

void render( SDL_Renderer* renderer, Polygon poly ) {
  std::vector<SDL_Point> sdlPoints;
  sdlPoints.resize(poly.points.size());
  std::transform(
    poly.points.begin(),
    poly.points.end(),
    sdlPoints.begin(),
    []( Point p ) -> SDL_Point {
      return { round(p.x), round(p.y) };
    }
  );

  SDL_RenderDrawLines( renderer, sdlPoints.data(), sdlPoints.size() );
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
    double angle = random( 0, 1 ) * M_PI*2;
    angles.push_back(angle);
  }
  std::sort( angles.begin(), angles.end() );

  std::vector<Point> points;
  for ( int i = 0; i < numVerts; i++ ) {
    double x = center.x + cos( angles.at(i) ) * ( radius + random( -16, 16 ));
    double y = center.y + sin( angles.at(i) ) * ( radius + random( -16, 16 ));
    points.push_back( { x, y } );
  }
  points.push_back( points.at(0) );

  return Polygon( points );
}

void renderPipeline( SDL_Renderer* renderer, std::vector<Polygon> objectsToRender ) {
  for( auto it = objectsToRender.begin(); it != objectsToRender.end(); it++ ) {
    render( renderer, *it );
  }
}

void renderFrame( SDL_Renderer* renderer, std::vector<Polygon> objectsToRender ) {
  SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderClear( gRenderer );
  SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

  renderPipeline( gRenderer, objectsToRender );

  SDL_RenderPresent( gRenderer );
}

MovingPolygon generateAsteroid() {
  MovingPolygon asteroid = generatePolygon( { SCREEN_WIDTH, random(0, SCREEN_HEIGHT ) }, 100, 10 );
  asteroid.heading = random( 0, M_PI );
  return asteroid;
}

std::vector<MovingPolygon> generateAsteroids( int numAsteroids ) {
  std::vector<MovingPolygon> asteroids;
  for( int i = 0; i < numAsteroids; i++ ) {
    asteroids.push_back(generateAsteroid());
  }
  return asteroids;
}

std::vector<MovingPolygon> updateAsteroids( std::vector<MovingPolygon> asteroids ) {
  std::vector<MovingPolygon> updatedAsteroids;
  for( auto it = asteroids.begin(); it != asteroids.end(); it++ ) {
    if( within( *it, gameSpace )) {
      MovingPolygon updatedAsteroid = translate2D( rotate2D( *it, 0.002, true ), 1 );
      updatedAsteroids.push_back( updatedAsteroid );
    } 
    else {
      updatedAsteroids.push_back( generateAsteroid() );
    }
  }
  return updatedAsteroids;
}

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

    MovingPolygon triangle;
    triangle.points = points;

    std::vector<MovingPolygon> asteroids = generateAsteroids( INIT_ASTEROIDS );

    double angle = 0.0;
    double angleDelta = 0.005;

    bool move = false;
    bool rotate = false;

    const Uint8 *keystate = NULL;

    std::vector<Polygon> objectsToRender;
    
    while( !quit ) {
      //move = false;
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

      if( rotate ) {
        triangle = rotate2D( triangle, angle );
      }
      if( move ) {
        triangle = translate2D( triangle, 1, screenBorder );
      }

      asteroids = updateAsteroids( asteroids );

      objectsToRender.push_back( triangle );
      objectsToRender.insert( objectsToRender.end(), asteroids.begin(), asteroids.end() );

      renderFrame( gRenderer, objectsToRender );

      objectsToRender.clear();
    }
  }
  close();
  return 0;
}