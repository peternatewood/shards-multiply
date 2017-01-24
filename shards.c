#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int PALETTE[8][4] = {
  { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE },
  { 0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE },
  { 0x2E, 0x63, 0xB3, SDL_ALPHA_OPAQUE },
  { 44, 98, 178, SDL_ALPHA_OPAQUE },
  { 44, 98, 178, SDL_ALPHA_OPAQUE },
  { 44, 98, 178, SDL_ALPHA_OPAQUE },
  { 44, 98, 178, SDL_ALPHA_OPAQUE },
  { 44, 98, 178, SDL_ALPHA_OPAQUE }
};

struct Timer {
  int startTicks;
  int pausedTicks;
  bool isStarted;
  bool isPaused;
};

struct Actor {
  int x, y, r;
  short xAcc, yAcc;
};

int main() {
  bool isRunning = true;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  }
  else {
    SDL_Window* gWindow = SDL_CreateWindow("Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (gWindow == NULL) printf("Window creation failed! SDL Error: %s\n", SDL_GetError());
    else {
      SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) printf("Surface creation failed! SDL Error: %s\n", SDL_GetError());
      else {
        struct Timer frameTimer = { SDL_GetTicks(), 0, false, false };
        struct Actor player = { 100, 100, 20, 0, 0 };
        SDL_Rect playerRect = { x: player.x, y: player.y, w: player.r, h: player.r };

        while (isRunning) {
          SDL_RenderClear(gRenderer);

          while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
              isRunning = false;
            }
            else if (!event.key.repeat && event.type == SDL_KEYDOWN) {
              switch (event.key.keysym.sym) {
                case SDLK_UP:     player.yAcc -= 1; break;
                case SDLK_DOWN:   player.yAcc += 1; break;
                case SDLK_LEFT:   player.xAcc -= 1; break;
                case SDLK_RIGHT:  player.xAcc += 1; break;
              }
              // printf("Keydown event %d, yAcc: %d\n, player.y %d\n", event.key.keysym.sym, player.yAcc);
            }
            else if (event.type == SDL_KEYUP) {
              switch (event.key.keysym.sym) {
                case SDLK_UP:     player.yAcc += 1; break;
                case SDLK_DOWN:   player.yAcc -= 1; break;
                case SDLK_LEFT:   player.xAcc += 1; break;
                case SDLK_RIGHT:  player.xAcc -= 1; break;
              }
              // printf("Keyup event %d, yAcc: %d\n, player.y %d\n", event.key.keysym.sym, player.yAcc);
            }
          }
          playerRect.y += player.yAcc;
          playerRect.x += player.xAcc;

          // Background color
          SDL_SetRenderDrawColor(gRenderer, 44, 98, 178, SDL_ALPHA_OPAQUE);
          SDL_RenderFillRect(gRenderer, NULL);

          // Player rect
          SDL_SetRenderDrawColor(gRenderer, PALETTE[1][0], PALETTE[1][1], PALETTE[1][2], PALETTE[1][1]);
          SDL_RenderFillRect(gRenderer, &playerRect);

          SDL_RenderPresent(gRenderer);
        }
        SDL_DestroyRenderer(gRenderer);
        gRenderer = NULL;
      }
      SDL_DestroyWindow(gWindow);
      gWindow = NULL;
    }
  }

  isRunning = NULL;
  SDL_Quit();

  return 0;
}
