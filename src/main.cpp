#include <SDL.h>
#include <chrono>
#include <thread>

#include "Polygon.h"

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

int main( int argc, char* args[] ) {
  if( !init() ) {
    printf( "Failed to initialise.\n" );
  } else {
    bool quit = false;
    SDL_Event e;

    std::vector<Point> _points = { 
      Point( SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 ),
      Point( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20 ),
      Point( SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2 ),
      Point( SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 )
    };

    Polygon triangle = Polygon( _points );

    double angle = 0.0;
    double angleDelta = 0.1;
    double angleMax = 0.5;
    double angleMin = -0.5;

    while( !quit ) {
      while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ) {
          quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
          switch( e.key.keysym.sym ) {
            case SDLK_RIGHT:
              if( !(angle < angleMin) ) {
                angle -= angleDelta;
              }
              break;
            case SDLK_LEFT:
              if( !(angle > angleMax) ) {
                angle += angleDelta;
              }
              break;
          }
        }
      }
      SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
      SDL_RenderClear( gRenderer );
      SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE );

      triangle.render( gRenderer );
      SDL_RenderPresent( gRenderer );

      triangle.rotate2D( angle );
    }
  }
  close();
  return 0;
}