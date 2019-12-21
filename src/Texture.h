#include <string>
#include <SDL.h>

class Texture {
  public:
    Texture();
    ~Texture();

    bool loadFromFile( std::string path, SDL_Renderer* renderer );

    void free();

    void render( int x, int y, SDL_Renderer* renderer );

    int getWidth();
    int getHeight();

  private:
    SDL_Texture* texture;

    int width;
    int height;
};