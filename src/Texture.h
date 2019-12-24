#include <string>
#include <SDL.h>

class Texture {
  public:
    Texture();
    ~Texture();

    bool loadFromFile( std::string path, SDL_Renderer* renderer );

    void free();

    void setColour( Uint8 red, Uint8 green, Uint8 blue );

    //void setBlendMode( SDL_BlendMode blending );

    //void setAlpha( Uint8 alpha );

    void render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL );

    int getWidth();
    int getHeight();

  private:
    SDL_Texture* texture;

    int width;
    int height;
};