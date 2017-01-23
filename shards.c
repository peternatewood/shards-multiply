#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
  bool isRunning = true;
  SDL_Event event;

  if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  }
  else {
    SDL_Window* gWindow = SDL_CreateWindow( "Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

    if ( gWindow == NULL ) printf("Window creation failed!");
    else {
      while ( isRunning ) {
        while ( SDL_PollEvent( &event ) != 0 ) {
          if ( event.type == SDL_QUIT ) {
            isRunning = false;
          }
        }
      }
      SDL_DestroyWindow( gWindow );
      gWindow = NULL;
    }
  }

  isRunning = NULL;
  SDL_Quit();

  return 0;
}
