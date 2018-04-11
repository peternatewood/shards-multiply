/*
  Includes all constants, declarations of common functions, and certain
  preprocessor defines
*/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_Image.h>
#include <SDL2_mixer/SDL_Mixer.h>

#define WINDOW_W 800
#define WINDOW_H 600

#define LEVEL_SIZE 2048
#define TILES_PER_ROW 32
#define UI_HEIGHT 16

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define LATENCY 2048

#define THRUST_MUSIC "thrust.wav"
#define CHANGE_SOUND "change.wav"

#define TICKS_PER_FRAME 1000.f / 60.f
#define MAX_SPEED 2
#define THRUST_SPEED 8

#define BOLT_LIFE 120
#define BOLT_SPEED 12

// Handy replacers
#define clamp(value, min, max) value < min ? min : value > max ? max : value
#define withinDistance(a, b, dist) dist * dist > (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)
#define withoutDistance(a, b, dist) dist * dist < (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)
#define outsideBounds(actor) actor.x < 0 || actor.x > LEVEL_SIZE || actor.y < 0 || actor.y > LEVEL_SIZE

const float FRICTION = 0.6;
const unsigned int PLAYER_SPEED = 4;

const int PALETTE[32] = {
  0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE, // Black
  0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE, // Offwhite
  0x2E, 0x63, 0xB3, SDL_ALPHA_OPAQUE, // Blue
  0xB3, 0x63, 0x2E, SDL_ALPHA_OPAQUE, // Brown
  0x44, 0x99, 0xFF, SDL_ALPHA_OPAQUE, // Light blue
  0xFF, 0xBB, 0x33, SDL_ALPHA_OPAQUE, // Orange
  0xCC, 0x00, 0x17, SDL_ALPHA_OPAQUE, // Red
  0x77, 0xAA, 0x17, SDL_ALPHA_OPAQUE  // Green
};

#define TITLE_LENGTH 14
#define MAX_TITLE_POINTS 15
const int TITLE_CHAR_COUNTS[TITLE_LENGTH] = { 15,13, 9,15,13,15,13,14, 7, 9,11,12, 7,13 };
const SDL_Point TITLE_POINTS[TITLE_LENGTH][MAX_TITLE_POINTS] = {
  { // S
    {  24,  0 },
    {  96,  0 },
    {  93, 24 },
    {  34, 24 },
    {  32, 36 },
    {  76, 36 },
    {  87, 66 },
    {  73, 96 },
    {   0, 96 },
    {   3, 72 },
    {  63, 72 },
    {  65, 60 },
    {  20, 60 },
    {   9, 30 },
    {  24,  0 }
  },
  { // H
    { 108,  0 },
    { 132,  0 },
    { 128, 36 },
    { 163, 36 },
    { 168,  0 },
    { 192,  0 },
    { 180, 96 },
    { 156, 96 },
    { 160, 60 },
    { 125, 60 },
    { 120, 96 },
    {  96, 96 },
    { 108,  0 }
  },
  { // A
    { 192, 96 },
    { 237,  0 },
    { 261,  0 },
    { 282, 96 },
    { 258, 96 },
    { 244, 36 },
    { 244, 36 },
    { 216, 96 },
    { 192, 96 }
  },
  { // R
    { 306,  0 },
    { 378,  0 },
    { 387, 24 },
    { 370, 50 },
    { 355, 50 },
    { 381, 96 },
    { 357, 96 },
    { 334, 50 },
    { 336, 36 },
    { 363, 36 },
    { 365, 24 },
    { 330, 24 },
    { 318, 96 },
    { 294, 96 },
    { 306,  0 }
  },
  { // D
    { 402,  0 },
    { 462,  0 },
    { 483, 24 },
    { 477, 72 },
    { 450, 96 },
    { 423, 96 },
    { 428, 60 },
    { 459, 60 },
    { 462, 36 },
    { 423, 36 },
    { 414, 96 },
    { 390, 96 },
    { 402,  0 }
  },
  { // S
    { 511,  0 },
    { 583,  0 },
    { 580, 24 },
    { 521, 24 },
    { 519, 36 },
    { 563, 36 },
    { 574, 66 },
    { 560, 96 },
    { 487, 96 },
    { 489, 72 },
    { 550, 72 },
    { 552, 60 },
    { 507, 60 },
    { 496, 30 },
    { 511,  0 }
  },
  { // M
    {  12,112 },
    {  36,112 },
    {  50,148 },
    {  72,112 },
    {  96,112 },
    {  84,208 },
    {  60,208 },
    {  64,172 },
    {  46,184 },
    {  30,172 },
    {  24,208 },
    {   0,208 },
    {  12,112 }
  },
  { // U
    { 108,112 },
    { 132,112 },
    { 126,168 },
    { 136,186 },
    { 160,166 },
    { 168,112 },
    { 192,112 },
    { 180,208 },
    { 156,208 },
    { 157,196 },
    { 144,208 },
    { 120,208 },
    {  99,184 },
    { 108,112 }
  },
  { // L
    { 204,112 },
    { 228,112 },
    { 219,184 },
    { 264,184 },
    { 261,208 },
    { 192,208 },
    { 204,112 }
  },
  { // T
    { 250,112 },
    { 345,112 },
    { 342,136 },
    { 305,136 },
    { 296,208 },
    { 272,208 },
    { 281,136 },
    { 247,136 },
    { 250,112 }
  },
  { // I
    { 357,112 },
    { 417,112 },
    { 414,136 },
    { 378,136 },
    { 372,184 },
    { 408,184 },
    { 405,208 },
    { 308,208 },
    { 311,184 },
    { 348,184 },
    { 357,112 }
  },
  { // P
    { 429,112 },
    { 501,112 },
    { 510,137 },
    { 493,162 },
    { 457,162 },
    { 459,148 },
    { 486,148 },
    { 488,136 },
    { 453,136 },
    { 441,208 },
    { 417,208 },
    { 429,112 }
  },
  { // L
    { 526,112 },
    { 550,112 },
    { 543,184 },
    { 586,184 },
    { 583,208 },
    { 514,208 },
    { 526,112 }
  },
  { // Y
    { 583,112 },
    { 607,112 },
    { 604,136 },
    { 634,148 },
    { 639,112 },
    { 665,112 },
    { 653,208 },
    { 595,208 },
    { 598,184 },
    { 627,184 },
    { 629,174 },
    { 578,148 },
    { 583,112 }
  }
};
#define PLAYER_POINTS_COUNT 9
const SDL_Point PLAYER_POINTS[PLAYER_POINTS_COUNT] = {
  { 12,  0 },
  {  4,  4 },
  {  0, 12 },
  {-10, 10 },
  { -6,  0 },
  {-10,-10 },
  {  0,-12 },
  {  4, -4 },
  { 12,  0 }
};

#define BOLT_POINTS_COUNT 5
const SDL_Point BOLT_POINTS[BOLT_POINTS_COUNT] = {
  { 4, 0 },
  { 0, 4 },
  {-8, 0 },
  { 0,-4 },
  { 4, 0 }
};

#define MISSILE_POINTS_COUNT 5
const SDL_Point MISSILE_POINTS[MISSILE_POINTS_COUNT] = {
  { 8, 0 },
  {-8, 4 },
  {-4, 0 },
  {-8,-4 },
  { 8, 0 }
};

// x1, y1, x2, y2
#define RETICULE_LINES_COUNT 8
const signed char RETICULE_LINES[4 * RETICULE_LINES_COUNT] = {
  -10, -5,  8, -5,
  -10,  5,  8,  5,
   -9, -9, -5, -4,
   -9,  9, -5,  4,
   -2, -9,  2, -4,
   -2,  9,  2,  4,
    5, -9,  9, -4,
    5,  9,  9,  4
};

#define LIGHTNING_POINTS_COUNT 7
const SDL_Point LIGHTNING_POINTS[LIGHTNING_POINTS_COUNT] = {
  { 12,  0 },
  {  0,  2 },
  {  1,  5 },
  {-12,  0 },
  {  0, -2 },
  { -1, -5 },
  { 12,  0 }
};

#define DEFENDER_POINTS_COUNT 11
const SDL_Point DEFENDER_POINTS[DEFENDER_POINTS_COUNT] = {
  { 12, -3 },
  { 12,  3 },
  {  0,  5 },
  {  3, 12 },
  { -6, 10 },
  {-10,  0 },
  { -6,-10 },
  {  3,-12 },
  {  0, -5 },
  { 12, -3 },
  { 12,  3 }
};

#define SWOOPER_POINTS_COUNT 11
const SDL_Point SWOOPER_POINTS[SWOOPER_POINTS_COUNT] = {
  { 12,  0 },
  {  2,  3 },
  { -6,  9 },
  {-10,  8 },
  { -8,  2 },
  {-12,  0 },
  { -8, -2 },
  {-10, -8 },
  { -6, -9 },
  {  2, -3 },
  { 12,  0 }
};

#define LEECH_POINTS_COUNT 11
const SDL_Point LEECH_POINTS[LEECH_POINTS_COUNT] = {
  {  2,  0 },
  {  4,  4 },
  { 12,  6 },
  {  6, 10 },
  { -8,  8 },
  { -6,  0 },
  { -8, -8 },
  {  6,-10 },
  { 12, -6 },
  {  4, -4 },
  {  2,  0 }
};

#define POWERUP_POINTS_COUNT 9
const SDL_Point POWERUP_POINTS[POWERUP_POINTS_COUNT] = {
  {  0, 6 },
  {  6, 0 },
  { 18, 0 },
  { 24, 6 },
  { 24,18 },
  { 18,24 },
  {  6,24 },
  {  0,18 },
  {  0, 6 }
};

#define TILE_SIZE 64
#define LEVEL_RECTS_COUNT 1024
const unsigned char LEVEL_SOURCE[LEVEL_RECTS_COUNT] = {
  6,6,6,3,3,3,7,7,7,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,7,7,7,7,7,
  6,6,3,3,3,3,3,7,7,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,7,7,7,7,
  6,3,3,3,3,3,7,7,7,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,7,7,7,
  3,3,3,3,3,7,7,7,7,7,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,7,
  3,3,3,3,3,3,7,7,7,7,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,
  3,3,3,3,3,7,7,7,7,7,7,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  7,7,3,3,7,7,7,7,5,7,7,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  7,7,7,7,7,7,7,7,5,7,7,7,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  7,7,7,7,7,7,7,7,5,7,7,7,7,7,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,
  7,7,7,5,5,5,5,5,5,7,7,7,7,7,7,7,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,3,3,3,3,3,3,3,2,2,
  3,3,3,7,7,7,7,7,7,7,7,7,5,5,5,7,7,7,7,7,7,7,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,7,7,7,7,7,7,5,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,
  2,2,3,3,3,3,3,3,7,7,7,7,5,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  2,2,2,2,2,3,3,3,3,7,7,7,5,7,7,7,3,3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,
  2,2,2,2,2,2,3,3,3,7,7,7,5,7,7,3,7,3,3,3,7,7,3,3,7,7,7,7,7,7,7,7,
  2,2,2,2,2,2,2,3,3,3,7,7,5,7,7,3,3,3,3,3,5,3,3,3,3,7,7,7,7,7,7,7,
  2,2,2,2,2,2,2,3,3,3,7,7,7,7,3,3,3,3,3,6,5,6,3,3,3,7,7,7,7,7,7,7,
  2,2,2,2,2,2,2,2,3,3,7,7,7,7,3,3,3,3,3,6,5,6,3,3,3,3,7,7,7,7,7,7,
  2,2,2,2,2,2,2,2,3,3,3,7,7,7,3,3,3,3,6,6,5,6,6,3,3,3,7,7,7,7,7,7,
  2,2,2,2,2,2,2,2,3,3,3,7,7,7,7,5,5,5,5,5,5,5,5,5,5,5,7,7,7,7,7,7,
  2,2,2,2,2,2,2,2,3,3,3,7,7,7,7,3,3,3,6,6,5,6,6,3,3,3,7,7,7,7,7,7,
  2,2,2,2,2,2,2,3,3,3,7,7,7,7,7,3,3,3,3,6,5,6,3,3,3,3,7,7,7,7,7,7,
  2,2,2,2,2,3,3,3,3,7,7,7,7,7,7,7,3,3,3,6,5,6,3,3,3,7,7,7,7,7,7,7,
  2,2,2,3,3,3,3,7,7,7,7,7,7,7,7,7,7,3,3,3,5,3,3,3,7,7,7,7,7,7,7,7,
  2,3,3,3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,5,3,3,3,7,7,7,7,7,7,7,7,
  3,3,3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,
  3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,3,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,3,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,3,3,3,3,3,3
};

struct Timer {
  int startTicks;
  int pausedTicks;
  bool isStarted;
  bool isPaused;
};

enum Scene {
  TITLE,
  GAME
};

enum Mode {
  BOLT,
  MISSILE,
  LASER,
  LAST
};

#define MAX_RENDER_POINTS 32
struct Actor {
  float x, y, xVel, yVel;
  int speed;
  unsigned char life;
  double r;
  signed char xAcc, yAcc;
  SDL_Point points[MAX_RENDER_POINTS];
};

// Types: bolt, missile, laser, satellite, shield
struct Powerup {
  int x, y;
  unsigned char type;
  SDL_Point points[POWERUP_POINTS_COUNT];
};

void aimActorAtPlayer(struct Actor* actor, struct Actor* player, bool setAcc) {
  actor->r = atan2(player->y - actor->y, player->x - actor->x);
  if (setAcc) {
    float xDist = player->x - actor->x;
    if (xDist < -MAX_SPEED * actor->speed) {
      actor->xAcc = -1;
    }
    else if (xDist > MAX_SPEED * actor->speed) {
      actor->xAcc = 1;
    }
    else {
      actor->xAcc = 0;
    }

    float yDist = player->y - actor->y;
    if (yDist < -MAX_SPEED * actor->speed) {
      actor->yAcc = -1;
    }
    else if (yDist > MAX_SPEED * actor->speed) {
      actor->yAcc = 1;
    }
    else {
      actor->yAcc = 0;
    }
  }
}

// TODO: Consider generalizing for all actors, so enemies have similar movement to the player (may require float values for acc)
void updateActorVelocity(struct Actor* actor) {
  float xVel = actor->xVel;
  if (actor->xAcc == 0) {
    xVel *= FRICTION;
    if (xVel > -0.02 && xVel < 0.02) xVel = 0;
  }
  else {
    xVel += actor->xAcc;
    if (xVel > MAX_SPEED) {
      xVel = MAX_SPEED;
    }
    else if (xVel < -MAX_SPEED) {
      xVel = -MAX_SPEED;
    }
  }
  actor->xVel = xVel;

  float yVel = actor->yVel;
  if (actor->yAcc == 0) {
    yVel *= FRICTION;
    if (yVel > -0.02 && yVel < 0.02) yVel = 0;
  }
  else {
    yVel += actor->yAcc;
    if (yVel > MAX_SPEED) {
      yVel = MAX_SPEED;
    }
    else if (yVel < -MAX_SPEED) {
      yVel = -MAX_SPEED;
    }
  }
  actor->yVel = yVel;
}

void moveActor(struct Actor *actor) {
  actor->x += actor->xVel * actor->speed;
  actor->y += actor->yVel * actor->speed;
}

void fireBolt(struct Actor* actor, struct Actor projectiles[], unsigned int index) {
  float actorCos = cos(actor->r);
  float actorSin = sin(actor->r);
  int speed = BOLT_SPEED + sqrt(actor->xVel * actor->xVel + actor->yVel * actor->yVel);

  projectiles[index].x = actor->x;
  projectiles[index].y = actor->y;
  projectiles[index].speed = speed;
  projectiles[index].life = BOLT_LIFE;
  projectiles[index].r = actor->r;
  projectiles[index].xVel = actorCos;
  projectiles[index].yVel = actorSin;
}

void setPaletteColor(SDL_Renderer* renderer, int paletteNumber) {
  SDL_SetRenderDrawColor(renderer, PALETTE[4 * paletteNumber], PALETTE[4 * paletteNumber + 1], PALETTE[4 * paletteNumber + 2], PALETTE[4 * paletteNumber + 3]);
}

// Detect whether a point is inside a closed polygon
bool isPointInPolygon(float x, float y, SDL_Point* points, int numOfCorners) {
  unsigned int i, j = numOfCorners - 1;
  float iX, iY, jX, jY; // An SDL_Point uses int x,y values
  bool oddNumOfNodes = false;

  for (i = 0; i < numOfCorners; i++) {
    iX = points[i].x;
    iY = points[i].y;
    jX = points[j].x;
    jY = points[j].y;

    if ((iY < y && y <= jY) || (jY < y && y <= iY)) {
      if (iX + (y - iY) / (jY - iY) * (jX - iX) < x) {
        oddNumOfNodes = !oddNumOfNodes;
      }
    }
    j = i;
  }

  return oddNumOfNodes;
}
// Fill every point that falls inside a closed polygon as defined by SDL_Points
void fillPolygon(SDL_Renderer* renderer, SDL_Point* points, unsigned int length) {
  unsigned int xMin = WINDOW_W;
  unsigned int xMax = 0;
  unsigned int yMin = WINDOW_H;
  unsigned int yMax = 0;

  // Set min and max for both dimensions
  for (unsigned int i = 0; i < length; i++) {
    if (points[i].x < xMin) xMin = points[i].x;
    if (points[i].x > xMax) xMax = points[i].x;
    if (points[i].y < yMin) yMin = points[i].y;
    if (points[i].y > yMax) yMax = points[i].y;
  }
  if (xMin < 0) xMin = 0;
  if (xMax > WINDOW_W) xMax = WINDOW_W;
  if (yMin < 0) yMin = 0;
  if (yMax > WINDOW_H) yMax = WINDOW_H;

  // Fill each row of pixels
  for (unsigned int y = yMin; y <= yMax; y++) {
    for (unsigned int x = xMin; x <= xMax; x++) {
      if (isPointInPolygon(x, y, points, length - 1)) {
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }
}

/*
  If the user was holding any keys or mouse buttons before the game started,
  we need to ensure we don't trigger a keyup event until they press that key
  or mouse button first.
*/
bool activatedInputs[7] = { false, false, false, false, false, false, false };
void activateInput(SDL_Keycode keycode) {
  switch (keycode) {
    case SDLK_w: if (!activatedInputs[0]) { activatedInputs[0] = true; } break;
    case SDLK_s: if (!activatedInputs[1]) { activatedInputs[1] = true; } break;
    case SDLK_a: if (!activatedInputs[2]) { activatedInputs[2] = true; } break;
    case SDLK_d: if (!activatedInputs[3]) { activatedInputs[3] = true; } break;
    case SDLK_SPACE : if (!activatedInputs[4]) { activatedInputs[4] = true; } break;
  }
}
void activateMouseInput(Uint32 button) {
  switch (button) {
    case SDL_BUTTON_LEFT  : if (!activatedInputs[5]) { activatedInputs[5] = true; } break;
    case SDL_BUTTON_RIGHT : if (!activatedInputs[6]) { activatedInputs[6] = true; } break;
  }
}
bool wasInputActive(SDL_Keycode keycode) {
  switch (keycode) {
    case SDLK_w: return activatedInputs[0]; break;
    case SDLK_s: return activatedInputs[1]; break;
    case SDLK_a: return activatedInputs[2]; break;
    case SDLK_d: return activatedInputs[3]; break;
    case SDLK_SPACE : return activatedInputs[4]; break;
  }
  return false;
}
bool wasMouseInputActive(Uint32 button) {
  switch (button) {
    case SDL_BUTTON_LEFT  : return activatedInputs[5]; break;
    case SDL_BUTTON_RIGHT : return activatedInputs[6]; break;
  }
  return false;
}
