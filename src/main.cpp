#include <SDL.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include "MovingPolygon.h"
#include "transform.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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

    Polygon screenBorder ( {
      { 0, 0 },
      { SCREEN_WIDTH, 0 },
      { SCREEN_WIDTH, SCREEN_HEIGHT },
      { 0, SCREEN_HEIGHT },
      { 0, 0 }
    } );

    MovingPolygon triangle;
    triangle.points = points;

    double angle = 0.0;
    double angleDelta = 0.005;

    bool move = false;
    bool rotate = false;

    const Uint8 *keystate = NULL;

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
      SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
      SDL_RenderClear( gRenderer );
      SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

      render( gRenderer, triangle );
      SDL_RenderPresent( gRenderer );

      if( rotate ) {
        triangle = rotate2D( triangle, angle );
      }
      if( move ) {
        triangle = translate2D( triangle, 1, screenBorder );
      }
    }
  }
  close();
  return 0;
}