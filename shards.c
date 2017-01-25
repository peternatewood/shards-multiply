#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float TICKS_PER_FRAME = 1000 / 60;

const int PALETTE[32] = {
  0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE,
  0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE,
  0x2E, 0x63, 0xB3, SDL_ALPHA_OPAQUE,
  0xB3, 0x63, 0x2E, SDL_ALPHA_OPAQUE,
  0x44, 0x99, 0xDD, SDL_ALPHA_OPAQUE,
  0xFF, 0xBB, 0x77, SDL_ALPHA_OPAQUE,
  0xFF, 0x00, 0x17, SDL_ALPHA_OPAQUE,
  0x00, 0xFF, 0x17, SDL_ALPHA_OPAQUE
};

#define SPRITE_SIZE 16
#define SPRITE_LENGTH (SPRITE_SIZE * SPRITE_SIZE)

const int SPRITES[SPRITE_LENGTH * 4] = {
  0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,
  0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,
  4,4,4,4,4,3,3,3,4,4,0,0,0,0,0,0,
  4,4,3,3,3,3,3,3,4,4,0,0,0,0,0,0,
  0,4,4,3,3,3,3,3,3,4,0,0,0,0,0,0,
  0,4,4,3,3,3,3,3,3,4,4,4,0,0,0,0,
  0,0,4,3,3,3,3,3,3,3,4,4,4,4,0,0,
  0,0,4,4,3,3,3,3,3,3,3,3,3,4,4,4,
  0,0,4,4,3,3,3,3,3,3,3,3,3,4,4,4,
  0,0,4,3,3,3,3,3,3,3,4,4,4,4,0,0,
  0,4,4,3,3,3,3,3,3,4,4,4,0,0,0,0,
  0,4,4,3,3,3,3,3,3,4,0,0,0,0,0,0,
  4,4,3,3,3,3,3,3,4,4,0,0,0,0,0,0,
  4,4,4,4,4,3,3,3,4,4,0,0,0,0,0,0,
  0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,
  0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,

  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,
  5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,
  5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,
  5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,
  5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,
  5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,
  5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,
  5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,
  5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,
  5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,
  5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,
  5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,
  5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,

  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,
  0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,
  0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,
  0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,
  0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,
  0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,
  0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,
  0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,
  0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

struct Timer {
  int startTicks;
  int pausedTicks;
  bool isStarted;
  bool isPaused;
};

const double FRICTION = 0.6;
const unsigned short PLAYER_SPEED = 4;
struct Actor {
  int x, y;
  double r;
  float xVel, yVel, xAcc, yAcc;
};

void setPaletteColor(int *renderer, int paletteNumber) {
  SDL_SetRenderDrawColor(renderer, PALETTE[paletteNumber], PALETTE[paletteNumber + 1], PALETTE[paletteNumber + 2], PALETTE[paletteNumber + 3]);
}

void renderSprite(int *renderer, SDL_Point pos, int spriteNumber) {
  int start = spriteNumber * SPRITE_LENGTH;

  for (unsigned int i = start; i < start + SPRITE_LENGTH; i++) {
    if (SPRITES[i] > 0) {
      if (i == start || SPRITES[i] != SPRITES[i - 1]) {
        setPaletteColor(renderer, (SPRITES[i] - 1) * 4);
      }
      SDL_RenderDrawPoint(renderer, pos.x + ((i - start) % SPRITE_SIZE), pos.y + ((i - start) / SPRITE_SIZE));
    }
  }
}

void moveActor(struct Actor *actor) {
  actor->xVel = (actor->xVel * FRICTION) + actor->xAcc;
  actor->yVel = (actor->yVel * FRICTION) + actor->yAcc;

  if ((actor->xVel > 0 && actor->xVel < 0.1) || (actor->xVel < 0 && actor->xVel > -0.1)) actor->xVel = 0;
  if ((actor->yVel > 0 && actor->yVel < 0.1) || (actor->yVel < 0 && actor->yVel > -0.1)) actor->yVel = 0;

  actor->x += actor->xVel * PLAYER_SPEED;
  actor->y += actor->yVel * PLAYER_SPEED;
}

void renderActor(int *renderer, struct Actor actor, int spriteNumber) {
  int x, y;
  int start = spriteNumber * SPRITE_LENGTH;
  int actX = actor.x - (SPRITE_SIZE / 2);
  int actY = actor.y - (SPRITE_SIZE / 2);

  for (unsigned int i = start; i < start + SPRITE_LENGTH; i++) {
    if (SPRITES[i] > 0) {
      x = ((i - start) % SPRITE_SIZE) - (SPRITE_SIZE / 2);
      y = ((i - start) / SPRITE_SIZE) - (SPRITE_SIZE / 2);
      if (i == start || SPRITES[i] != SPRITES[i - 1]) {
        setPaletteColor(renderer, (SPRITES[i] - 1) * 4);
      }
      SDL_RenderDrawPoint(renderer, actX + ((x * cos(actor.r)) - (y * sin(actor.r))), actY + ((y * cos(actor.r)) + (x * sin(actor.r))));
    }
  }
}

void fireBolt(struct Actor actor, struct Actor projectiles[], unsigned int index) {
  projectiles[index].x = actor.x;
  projectiles[index].y = actor.y;
  projectiles[index].r = actor.r;
  projectiles[index].xVel = 0;
  projectiles[index].yVel = 0;
  projectiles[index].xAcc = cos(actor.r);
  projectiles[index].yAcc = sin(actor.r);
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
        struct Actor player = { 100, 100, 0, 0, 0, 0, 0 };
        SDL_Point mouse = { 0, 0 };
        SDL_Point tilePoint = { SPRITE_SIZE, 0 };
        struct Actor projectiles[10];

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
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
              switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                  fireBolt(player, &projectiles, 0);
                  break;
              }
            }
            else if (event.type == SDL_MOUSEMOTION) {
              mouse.x = event.motion.x;
              mouse.y = event.motion.y;
            }
          }
          moveActor(&player);
          player.r = atan2(mouse.y - player.y, mouse.x - player.x);

          // Background color
          SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
          SDL_RenderFillRect(gRenderer, NULL);

          renderSprite(gRenderer, tilePoint, 1);

          for (unsigned int i = 0; i < 10; i++) {
            if (&projectiles[i] != NULL) {
              moveActor(&projectiles[i]);
              renderActor(gRenderer, projectiles[i], 2);
            }
          }

          // Player sprite
          renderActor(gRenderer, player, 0);

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
