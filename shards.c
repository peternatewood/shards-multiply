#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float TICKS_PER_FRAME = 1000 / 60;

const int PALETTE[32] = {
  0x00, 0x00, 0x00, 0x00,
  0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE,
  0x2E, 0x63, 0xB3, SDL_ALPHA_OPAQUE,
  0xB3, 0x63, 0x2E, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE
};

const int SPRITE_SIZE = 64;
const int SPRITES[SPRITE_SIZE * 2] = {
  0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,0,0,1,1,1,
  1,1,0,0,0,0,1,1,

  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3
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

void renderSprite(int *renderer, struct Actor actor, int spriteNumber) {
  int start = spriteNumber * SPRITE_SIZE;
  for (int i = start; i < start + SPRITE_SIZE; i++) {
    if (i == start || SPRITES[i] != SPRITES[i - 1]) {
      int index = SPRITES[i] * 4;
      SDL_SetRenderDrawColor(renderer, PALETTE[index], PALETTE[index + 1], PALETTE[index + 2], PALETTE[index + 1]);
    }
    SDL_RenderDrawPoint(renderer, actor.x + (i % 8), actor.y + (i / 8));
  }
}

int main() {
  bool isRunning = true;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  else {
    SDL_Window* gWindow = SDL_CreateWindow("Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (gWindow == NULL) printf("Window creation failed! SDL Error: %s\n", SDL_GetError());
    else {
      SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) printf("Surface creation failed! SDL Error: %s\n", SDL_GetError());
      else if (SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND) < 0) printf("Failed to set render draw blend mode! SDL Error: %s\n", SDL_GetError());
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
          renderSprite(gRenderer, player, 0);

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
