#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int WORLD_WIDTH = 1280;
const int WORLD_HEIGHT = 960;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

/*class Character {
  public:
    static const int WIDTH = 20;
    static const int HEIGHT = 20;

    static const int VEL = 10;

    Character();

    void handleEvent( SDL_Event& e );

    void move();

    void render( int camX, int camY );

    int getPosX();
    int getPosY();

  private:
    int mPosX, mPosY;

    int mVelX, mVelY;
};

void Character::move() {
  mPosX += mVelX;
  if(( mPosX < 0 ) || ( mPosX + WIDTH > WORLD_WIDTH )) {
    // Move back
    mPosX -= mVelX;
  }

  // Move up or down
  mPosY += mVelY;
  if(( mPosY < 0 ) || ( mPosY + HEIGHT > WORLD_HEIGHT )) {
    // Move back
    mPosY -= mVelY;
  }
}

void Character::render( int camX, int camY ) {
  // Show the character relative to the camera
  gDotTexture.render( mPosX - camX, mPosY - camY );
}*/

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

bool init();

bool loadMedia();

void close();

SDL_Surface* loadSurface( std::string path );
SDL_Texture* loadTexture( std::string path );

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

SDL_Texture* loadTexture( std::string path ) {
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL ) {
    printf( "Unable to load image%s. SDL_Image Error: %s\n", path.c_str(), IMG_GetError() );
  } else {
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL ) {
      printf( "Unable to create texture frm %s. SDL Error: %s\n", path.c_str(), IMG_GetError() );
    } else {
      SDL_FreeSurface( loadedSurface );
    }
  }

  return newTexture;
}

SDL_Surface* loadSurface( std::string path ) {
  SDL_Surface* optimisedSurface = NULL;

  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL ) {
    printf( "Unable to load image %s. SDL Error: %s\n", path.c_str(), SDL_GetError() );
  } else {
    // Convert surface to screen format
    optimisedSurface = SDL_ConvertSurface(
        loadedSurface,
        gScreenSurface->format,
        0
    );
    if( optimisedSurface == NULL ) {
      printf(
          "Unable to optimise image %s. SDL Error: %s\n",
          path.c_str(),
          SDL_GetError()
      );
      SDL_FreeSurface( loadedSurface );
    }
  }

  return optimisedSurface;
}

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
      } else {
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if( (IMG_Init( imgFlags ) & imgFlags) != imgFlags ) {
          printf( "SDL_image could not initialise! SDL_image Error: %s\n", IMG_GetError() );
        } else {
          gScreenSurface = SDL_GetWindowSurface( gWindow );
        } 
      }
    }
  }

  return success;
}

bool loadMedia() {
  bool success = true;

  return success;
}

void close() {
  
  SDL_DestroyTexture( gTexture );
  gTexture = NULL;

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  IMG_Quit();
  SDL_Quit();
}

int main( int argc, char* args[] ) {
  if( !init() ) {
    printf( "Failed to initalise.\n" );
  } else {
    if( !loadMedia() ) {
      printf( "Failed to load media\n" );
    } else {
      bool quit = false;
      SDL_Event e;
      gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
      while ( !quit ) {
        while(SDL_PollEvent( &e ) != 0 ) {
          if( e.type == SDL_QUIT ) {
            quit = true;
          } 
       }

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        // Render shapes
        SDL_Rect fillRect = { SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect( gRenderer, &fillRect );

        SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3 };
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
        SDL_RenderDrawRect( gRenderer, &outlineRect );

        SDL_RenderPresent( gRenderer );

     }
    }
  }
  close();

  return 0;
}
