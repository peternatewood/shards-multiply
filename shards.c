#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float TICKS_PER_FRAME = 1000 / 60;

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

const int sprite[64] = {
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1
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
        int frames = 0;
        int frameTicks = 0;
        struct Timer frameTimer = { SDL_GetTicks(), 0, false, false };
        struct Actor player = { 100, 100, 20, 0, 0 };

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
            }
            else if (event.type == SDL_KEYUP) {
              switch (event.key.keysym.sym) {
                case SDLK_UP:     player.yAcc += 1; break;
                case SDLK_DOWN:   player.yAcc -= 1; break;
                case SDLK_LEFT:   player.xAcc += 1; break;
                case SDLK_RIGHT:  player.xAcc -= 1; break;
              }
            }
          }
          player.y += player.yAcc;
          player.x += player.xAcc;

          // Background color
          SDL_SetRenderDrawColor(gRenderer, 44, 98, 178, SDL_ALPHA_OPAQUE);
          SDL_RenderFillRect(gRenderer, NULL);

          // Player sprite
          for (int i = 0; i < 64; i++) {
            if (i == 0 || sprite[i] != sprite[i - 1]) {
              SDL_SetRenderDrawColor(gRenderer, PALETTE[sprite[i]][0], PALETTE[sprite[i]][1], PALETTE[sprite[i]][2], PALETTE[sprite[i]][1]);
            }
            SDL_RenderDrawPoint(gRenderer, player.x + (i % 8), player.y + (i / 8));
          }

          SDL_RenderPresent(gRenderer);

          frameTicks = frames / (SDL_GetTicks() - frameTimer.startTicks);
          if (frameTicks < TICKS_PER_FRAME) SDL_Delay(TICKS_PER_FRAME - frameTicks);
          frames++;
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
