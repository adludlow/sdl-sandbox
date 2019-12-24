#include "Texture.h"

#include <SDL_image.h>

Texture::Texture() {
  texture = NULL;
  width = 0;
  height = 0;
}

Texture::~Texture() {
  free();
}

bool Texture::loadFromFile( std::string path, SDL_Renderer* renderer ) {
  free();
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL ) {
    printf( "Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  } else {
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0x00, 0x09, 0xFD ));

    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == NULL ) {
      printf( "Unable to create texture from %s. SDL_Error: %s\n", path.c_str(), SDL_GetError() );
    } else {
      width = loadedSurface->w;
      height = loadedSurface->h;
    }

    SDL_FreeSurface( loadedSurface );
  }

  texture = newTexture;

  return texture != NULL;
}

void Texture::free() {
  if( texture != NULL ) {
    SDL_DestroyTexture( texture );
    texture = NULL;
    width = 0;
    height = 0;
  }
}

void Texture::render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip ) {
  SDL_Rect renderQuad = { x, y, width, height };

  if( clip != NULL ) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy( renderer, texture, clip, &renderQuad );
}

void Texture::setColour( Uint8 red, Uint8 green, Uint8 blue ) {
  SDL_SetTextureColorMod( texture, red, green, blue );
}

int Texture::getWidth() {
  return width;
}

int Texture::getHeight() {
  return height;
}