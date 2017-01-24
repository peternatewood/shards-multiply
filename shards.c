#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float TICKS_PER_FRAME = 1000 / 60;

const int PALETTE[32] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE,
  0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE,
  0x2E, 0x63, 0xB3, SDL_ALPHA_OPAQUE,
  0xB3, 0x63, 0x2E, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE,
  44, 98, 178, SDL_ALPHA_OPAQUE
};

const int SPRITE_SIZE = 16;
const int SPRITE_LENGTH = SPRITE_SIZE * SPRITE_SIZE;
const int SPRITES[SPRITE_LENGTH * 2] = {
  0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,
  0,0,0,4,4,4,3,3,4,0,0,0,0,0,0,0,
  4,4,4,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,4,3,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,4,3,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,4,3,3,3,3,3,3,3,3,4,4,0,0,0,0,
  0,0,4,3,3,3,3,3,3,3,3,3,4,4,0,0,
  0,0,4,3,3,3,3,3,3,3,3,3,3,3,4,4,
  0,0,4,3,3,3,3,3,3,3,3,3,3,3,4,4,
  0,0,4,3,3,3,3,3,3,3,3,3,4,4,0,0,
  0,4,3,3,3,3,3,3,3,3,4,4,0,0,0,0,
  0,4,3,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,4,3,3,3,3,3,3,3,4,0,0,0,0,0,0,
  4,4,4,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,0,0,4,4,4,3,3,4,0,0,0,0,0,0,0,
  0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,

  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};

struct Timer {
  int startTicks;
  int pausedTicks;
  bool isStarted;
  bool isPaused;
};

struct Actor {
  int x, y;
  double r;
  short xAcc, yAcc;
};

void renderSprite(int *renderer, struct Actor actor, int spriteNumber) {
  int x, y;
  int start = spriteNumber * SPRITE_LENGTH;
  int actX = actor.x - (SPRITE_SIZE / 2);
  int actY = actor.y - (SPRITE_SIZE / 2);

  for (int i = start; i < start + SPRITE_LENGTH; i++) {
    x = (i % SPRITE_SIZE) - (SPRITE_SIZE / 2);
    y = (i / SPRITE_SIZE) - (SPRITE_SIZE / 2);
    if (i == start || SPRITES[i] != SPRITES[i - 1]) {
      int index = SPRITES[i] * 4;
      SDL_SetRenderDrawColor(renderer, PALETTE[index], PALETTE[index + 1], PALETTE[index + 2], PALETTE[index + 3]);
    }
    SDL_RenderDrawPoint(renderer, actX + ((x * cos(actor.r)) - (y * sin(actor.r))), actY + ((y * cos(actor.r)) + (x * sin(actor.r))));
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
        SDL_Point mouse = { 0, 0 };

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
            else if (event.type == SDL_MOUSEMOTION) {
              mouse.x = event.motion.x;
              mouse.y = event.motion.y;
            }
          }
          player.y += player.yAcc;
          player.x += player.xAcc;
          player.r = atan2(mouse.y - player.y, mouse.x - player.x);

          // Background color
          SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
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
