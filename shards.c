/* Development build; alter error messages for production release */

/*
Visuals:
  + Add thruster visuals for player movement
Health and Damage:
  + Player has limited health, can restore with packs in the levels
  + Shard collisions will damage player and certain shards will be destroyed by this
  + Shield upgrade adds layers and bubbles that take one impact
  + Invulnerable timer for shields so sustained damage can't cut through
Weapons:
  + Bolt: hold to fire at max rate, upgrade to fire multiple at once
  + Missile: limited amount, hold to lock target or press for heat-seeker (only seeks once close to enemy), reloads over time, upgrade for more launchers at different points on the ship
  + Laser: fires a beam until button released or charge expended, recharges over time, upgrade for different beam shape
  + Shield: activate to add more hits before player gets destroyed, recharges once depleted, destroys shards on contact (and is destroyed itself), upgrade for more layers/bubbles at positions
  + Satellites: orbit/follow player (depending on settings before mission) and fire lowest level version of player's weapon when spawned (so different ones can have different weapons), upgrade for more satellites, can be destroyed?
  + Shards: catching a shard before it explodes attaches to player ship, and adds feature, maximum: 4?, include button to detach oldest shard or automatically detach on colliding with a new shard?
Thruster mode:
  + Add slight screen shake and other visuals for cool factor
  + intangible? (can't interact with objects)
  + unlimited while using, but needs to recharge after leaving thrust mode
  + cannot use weapons or items in this mode (same for satellites)
Shards:
  + Several varieties. Individual shards have specific behavior and abilities. Multiple shards can combine to form new vessels. Each has a different death timer; if the player collides while the shard is dying but hasn't yet been destroyed, the shard attaches and adds a feature to the player
  + Lightning: charges at player and shatters on impact, adds electrical discharge: random electrical arcs that can damage nearby enemies
  + Defender: keeps distance while firing bolts at player, adds directional shield that moves with reticule, positioned based on angle of player when collecting
  + Swooper: crosses in front of player and fires missile volley, fastest individual shard, adds missile volley when coming out of thruster mode
  + Leech: latches onto player and drains health, destroyed by shield impact or thruster mode, slower than lightning, (adds health restore when attaching a shard)?
Shard Vessels:
  + Defender + 2 Lightning (Smiter): Behaves like defender, but as lightning shards take damage they detach, if defender core reaches 25% health it charges player and fires both shards
  + Lightning + Leech (): Leech attached behind while Lightning takes damage, if Lightning destroyed Leech keeps momentum but reverts to usual speed if changing direction
  + 2 Swooper + 3 Lightning (Bomber): Moves faster (1.5 times?), makes one pass for each Lightning and launches at player when closest, behaves like regular Swooper when out of Lightning but doesn't detach
*/

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_Image.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define LEVEL_SIZE 2048
#define TILES_PER_ROW 32
#define UI_HEIGHT 16

#define TICKS_PER_FRAME 1000.f / 60.f
#define MAX_SPEED 2
#define THRUST_SPEED 8
// Handy replacers
#define clamp(value, min, max) value < min ? min : value > max ? max : value
#define withinDistance(a, b, dist) dist * dist > (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)
#define withoutDistance(a, b, dist) dist * dist < (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)
#define outsideBounds(actor) actor.x < 0 || actor.x > LEVEL_SIZE || actor.y < 0 || actor.y > LEVEL_SIZE

SDL_Renderer* gRenderer;

SDL_Texture* loadTexture(char path[]) {
  // The final texture
  SDL_Texture* newTexture = NULL;
  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path);
  if (loadedSurface == NULL) printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
  else {
    // Don't use color keying if the source has a transparency layer already
    // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}

SDL_Texture* charset = NULL;
// Clip charset image for each character and render
void renderText(SDL_Renderer* renderer, char string[], unsigned int length, int x, int y) {
  SDL_Rect source = { 0, 0, 8, 8 };
  SDL_Rect dest = { x, y, 16, 16 };

  for (unsigned int i = 0; i < length && string[i] != "\0"; i++) {
    source.x = 8 * (string[i] % 16);
    source.y = 8 * (string[i] >> 4);
    SDL_RenderCopy(gRenderer, charset, &source, &dest);
    dest.x += 16;
  }
}

enum Scene {
  TITLE,
  GAME
} scene = TITLE;

float FRICTION = 0.6;
unsigned int PLAYER_SPEED = 4;
#define BOLT_LIFE 120
#define BOLT_SPEED 12

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
SDL_Point titlePoints[TITLE_LENGTH][MAX_TITLE_POINTS];
int titleX = -864;
int titleY = 160;

void setTitlePoints() {
  for (unsigned short i = 0; i < TITLE_LENGTH; i++) {
    for (unsigned short p = 0; p < TITLE_CHAR_COUNTS[i]; p++) {
      titlePoints[i][p].x = TITLE_POINTS[i][p].x + titleX;
      titlePoints[i][p].y = TITLE_POINTS[i][p].y + titleY;
    }
  }
}
void updateTitlePoints() {
  if (titleX < 64) {
    titleX += 48;
    for (unsigned short i = 0; i < TITLE_LENGTH; i++) {
      for (unsigned short p = 0; p < TITLE_CHAR_COUNTS[i]; p++) {
        titlePoints[i][p].x = TITLE_POINTS[i][p].x + titleX;
      }
    }
  }
}

#define MAX_RENDER_POINTS 32

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

// Camera position
unsigned int camera[2] = { 0, 0 };

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

struct Timer {
  int startTicks;
  int pausedTicks;
  bool isStarted;
  bool isPaused;
};

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

struct Actor {
  float x, y, xVel, yVel;
  int speed;
  unsigned char life;
  double r;
  signed char xAcc, yAcc;
  SDL_Point points[MAX_RENDER_POINTS];
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
// Types: bolt, missile, laser, satellite, shield
struct Powerup {
  int x, y;
  unsigned char type;
  SDL_Point points[POWERUP_POINTS_COUNT];
};

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

// Update an actor's render points to match its position and orientation
void updateRenderPoints(struct Actor *actor, SDL_Point* points, unsigned int length) {
  float actorSin = sin(actor->r);
  float actorCos = cos(actor->r);
  float x, y;

  for (unsigned char i = 0; i < length; i++) {
    x = points[i].x;
    y = points[i].y;
    actor->points[i].x = actor->x + x * actorCos + y * actorSin - camera[0];
    actor->points[i].y = actor->y + x * actorSin - y * actorCos - camera[1];
  }
}
// Update powerup's render points
void updatePowerupPoints(struct Powerup *powerup, SDL_Point* points, unsigned int length) {
}

void setPaletteColor(int *renderer, int paletteNumber) {
  SDL_SetRenderDrawColor(renderer, PALETTE[4 * paletteNumber], PALETTE[4 * paletteNumber + 1], PALETTE[4 * paletteNumber + 2], PALETTE[4 * paletteNumber + 3]);
}

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

int main() {
  bool isRunning = true;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  else {
    SDL_Window* gWindow = SDL_CreateWindow("Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (gWindow == NULL) printf("Window creation failed! SDL Error: %s\n", SDL_GetError());
    else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) printf("Surface creation failed! SDL Error: %s\n", SDL_GetError());
      else if (SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND) < 0) printf("Failed to set render draw blend mode! SDL Error: %s\n", SDL_GetError());
      else {
        charset = loadTexture("charset.png");
        // Hide the mouse cursor
        if (SDL_ShowCursor(SDL_DISABLE) < 0) printf("Failed to hide system cursor. SDL Error: %s\n", SDL_GetError());

        int frames = 0;
        int frameTicks = 0;
        struct Timer frameTimer = { SDL_GetTicks(), 0, false, false };
        setTitlePoints();

        bool thrustMode = false;
        struct Actor player = { 100.f, 100.f, 0.f, 0.f, PLAYER_SPEED, 100, 0, 0, 0 };
        SDL_Point mouse = { 0, 0 };
        // Visual reticule
        int reticule[2] = { 0, 0 };
        // Reuseable rect for rendering level tiles
        SDL_Rect levelTile = { 0, 0, TILE_SIZE, TILE_SIZE };
        const SDL_Rect UI_RECT = { 0, 0, WINDOW_W, UI_HEIGHT };

        #define POWERUP_COUNT 5
        struct Powerup powerups[POWERUP_COUNT] = {
          {  512, 256, 0 },
          {  256, 512, 1 },
          { 1024, 512, 2 },
          {  640, 768, 3 },
          { 1280,1984, 4 }
        };

        // Shards
        struct Actor lightning  = { 256.f, 128.f, 0.f, 0.f, PLAYER_SPEED / 2, 100, 0, 0, 0 };
        struct Actor defender   = { 256.f, 256.f, 0.f, 0.f, PLAYER_SPEED / 2, 100, 0, 0, 0 };
        struct Actor swooper    = { 256.f, 512.f, 0.f, 0.f, PLAYER_SPEED / 2, 100, 0, 0, 0 };
        struct Actor leech      = { 512.f, 128.f, 0.f, 0.f, PLAYER_SPEED / 2, 100, 0, 0, 0 };

        #define BOLTS_COUNT 16
        struct Actor enemyBolts[BOLTS_COUNT];
        for (char i = 0; i < BOLTS_COUNT; i++) {
          enemyBolts[i].x = 0.f;
          enemyBolts[i].y = 0.f;
          enemyBolts[i].xVel = 0.f;
          enemyBolts[i].yVel = 0.f;
          enemyBolts[i].speed = PLAYER_SPEED / 2;
          enemyBolts[i].life = 0;
          enemyBolts[i].r = 0.0;
          enemyBolts[i].xAcc = 0;
          enemyBolts[i].yAcc = 0;
        }

        bool shouldFireBolts = false;
        struct Actor projectiles[10] = {
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 },
          { -1.f, -1.f, 0.f, 0.f, 0, 0 }
        };
        int boltIndex = 0;
        int fireDelay = 0;

        while (isRunning) {
          SDL_RenderClear(gRenderer);

          while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
              isRunning = false;
            }
            else if (!event.key.repeat && event.type == SDL_KEYDOWN) {
              if (scene == TITLE) scene = GAME;

              activateInput(event.key.keysym.sym);
              switch (event.key.keysym.sym) {
                case SDLK_w: player.yAcc -= 1; break;
                case SDLK_s: player.yAcc += 1; break;
                case SDLK_a: player.xAcc -= 1; break;
                case SDLK_d: player.xAcc += 1; break;
                case SDLK_SPACE: thrustMode = true; break;
              }
            }
            else if (event.type == SDL_KEYUP) {
              if (wasInputActive(event.key.keysym.sym)) {
                switch (event.key.keysym.sym) {
                  case SDLK_w: player.yAcc += 1; break;
                  case SDLK_s: player.yAcc -= 1; break;
                  case SDLK_a: player.xAcc += 1; break;
                  case SDLK_d: player.xAcc -= 1; break;
                  case SDLK_SPACE: thrustMode = false; break;
                }
              }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
              if (scene == TITLE) scene = GAME;

              activateMouseInput(event.button.button);
              switch (event.button.button) {
                case SDL_BUTTON_LEFT  : shouldFireBolts = true; break;
                case SDL_BUTTON_RIGHT : break;
              }
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
              if (wasMouseInputActive(event.button.button)) {
                switch (event.button.button) {
                  case SDL_BUTTON_LEFT  : shouldFireBolts = false; break;
                  case SDL_BUTTON_RIGHT : break;
                }
              }
            }
            else if (event.type == SDL_MOUSEMOTION) {
              mouse.x = event.motion.x;
              mouse.y = event.motion.y;
            }
          }

          switch (scene) {
            case TITLE:
              SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
              SDL_RenderFillRect(gRenderer, NULL);

              updateTitlePoints();
              // Render title
              for (unsigned short i = 0; i < TITLE_LENGTH; i++) {
                SDL_SetRenderDrawColor(gRenderer, 0xBB, 0x66, 0x11, SDL_ALPHA_OPAQUE);
                fillPolygon(gRenderer, &titlePoints[i], TITLE_CHAR_COUNTS[i]);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xDD, 0x88, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLines(gRenderer, &titlePoints[i], TITLE_CHAR_COUNTS[i]);
              }
              // Render start flash
              if (titleX >= 64 && frames % 40 < 24) {
                renderText(gRenderer, "Press any key to start", 22, WINDOW_W / 2 - 176, 440);
              }
              break;
            case GAME:
              if (shouldFireBolts && fireDelay == 0) {
                fireBolt(&player, &projectiles, boltIndex);
                boltIndex = (boltIndex + 1) % 10;
                fireDelay = 6;
              }
              else if (fireDelay > 0) fireDelay--;

              if (thrustMode) {
                // TODO: Stop jittering zigzags if the player nears a level boundary. Maybe loop levels?
                player.xVel = THRUST_SPEED * cos(player.r);
                player.yVel = THRUST_SPEED * sin(player.r);
                // Update camera position
                int camX = camera[0] + player.speed * player.xVel;
                int centerXDist = (player.x - WINDOW_W / 2) - camX;
                if (camX != centerXDist) {
                  if (centerXDist > -THRUST_SPEED && centerXDist < THRUST_SPEED) {
                    camX = player.x - WINDOW_W / 2;
                  }
                  else {
                    camX += centerXDist > 0 ? THRUST_SPEED : -THRUST_SPEED;
                  }
                }
                // int camX = player.x - WINDOW_W / 2;
                if (camX < 0) {
                  camX = 0;
                }
                else if (camX > LEVEL_SIZE - WINDOW_W) {
                  camX = LEVEL_SIZE - WINDOW_W;
                }
                camera[0] = camX;

                int camY = camera[1] + player.speed * player.yVel;
                int centerYDist = (player.y - WINDOW_H / 2) - camY;
                if (camY != centerYDist) {
                  if (centerYDist > -THRUST_SPEED && centerXDist < THRUST_SPEED) {
                    camY = player.y - WINDOW_H / 2;
                  }
                  else {
                    camY += centerYDist > 0 ? THRUST_SPEED : -THRUST_SPEED;
                  }
                }
                // int camY = player.y - WINDOW_H / 2;
                if (camY < 0) {
                  camY = 0;
                }
                else if (camY > LEVEL_SIZE - WINDOW_H) {
                  camY = LEVEL_SIZE - WINDOW_H;
                }
                camera[1] = camY;
              }
              else {
                updateActorVelocity(&player);
              }
              moveActor(&player);
              // if (player.x < 0) {
              //   player.x = 0;
              // }
              // else if (player.x > LEVEL_SIZE) {
              //   player.x = LEVEL_SIZE;
              // }

              // if (player.y < 0) {
              //   player.y = 0;
              // }
              // else if (player.y > LEVEL_SIZE) {
              //   player.y = LEVEL_SIZE;
              // }

              // Restrict player to camera space
              if (player.x < camera[0]) {
                player.x = camera[0];
              }
              else if (player.x > camera[0] + WINDOW_W) {
                player.x = camera[0] + WINDOW_W;
              }
              if (player.y < camera[1]) {
                player.y = camera[1];
              }
              else if (player.y > camera[1] + WINDOW_H) {
                player.y = camera[1] + WINDOW_H;
              }
              player.r = atan2(mouse.y - (player.y - camera[1]), mouse.x - (player.x - camera[0]));
              updateRenderPoints(&player, PLAYER_POINTS, PLAYER_POINTS_COUNT);

              // Background color
              SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
              SDL_RenderFillRect(gRenderer, NULL);

              // TODO: Render background tiles
              int tileX, tileY;
              for (int i = 0; i < LEVEL_RECTS_COUNT; i++) {
                setPaletteColor(gRenderer, LEVEL_SOURCE[i]);
                // SDL_RenderFillRect(gRenderer, &levelRects[i]);
                tileX = TILE_SIZE * (i % TILES_PER_ROW) - camera[0];
                tileY = TILE_SIZE * (i / TILES_PER_ROW) - camera[1];
                // If the tile is on screen
                if (tileX > -TILE_SIZE && tileX < WINDOW_W && tileY > -TILE_SIZE && tileY < WINDOW_H) {
                  if (tileX < 0) {
                    levelTile.w = TILE_SIZE + tileX;
                    tileX = 0;
                  }
                  else if (tileX > WINDOW_W - TILE_SIZE) {
                    levelTile.w = WINDOW_W - tileX;
                  }
                  else if (levelTile.w != TILE_SIZE) {
                    levelTile.w = TILE_SIZE;
                  }
                  levelTile.x = tileX;

                  if (tileY < 0) {
                    levelTile.h = TILE_SIZE + tileY;
                    tileY = 0;
                  }
                  else if (tileY > WINDOW_H - TILE_SIZE) {
                    levelTile.h = WINDOW_H - tileY;
                  }
                  else if (levelTile.h != TILE_SIZE) {
                    levelTile.h = TILE_SIZE;
                  }
                  levelTile.y = tileY;

                  SDL_RenderFillRect(gRenderer, &levelTile);
                }
              }

              // Draw powerups
              for (unsigned char i = 0; i < POWERUP_COUNT; i++) {
                for (unsigned char p = 0; p < POWERUP_POINTS_COUNT; p++) {
                  powerups[i].points[p].x = powerups[i].x + POWERUP_POINTS[p].x - camera[0];
                  powerups[i].points[p].y = powerups[i].y + POWERUP_POINTS[p].y - camera[1];
                }
                setPaletteColor(gRenderer, powerups[i].type);
                fillPolygon(gRenderer, &powerups[i].points, POWERUP_POINTS_COUNT);
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLines(gRenderer, &powerups[i].points, POWERUP_POINTS_COUNT);
              }

              // Update and draw shards
              // Lighting
              if (lightning.life != 0) {
                aimActorAtPlayer(&lightning, &player, true);
                updateActorVelocity(&lightning);
                moveActor(&lightning);
              }

              updateRenderPoints(&lightning, LIGHTNING_POINTS, LIGHTNING_POINTS_COUNT);
              setPaletteColor(gRenderer, 4);
              fillPolygon(gRenderer, &lightning.points, LIGHTNING_POINTS_COUNT);
              setPaletteColor(gRenderer, 1);
              SDL_RenderDrawLines(gRenderer, &lightning.points, LIGHTNING_POINTS_COUNT);

              // Defender
              if (defender.life != 0) {
                aimActorAtPlayer(&defender, &player, false);
                float playerXDist, playerYDist;
                if (withinDistance(defender, player, 128)) {
                  playerXDist = defender.x - player.x;
                  playerYDist = defender.y - player.y;
                  // defender.xAcc = playerXDist > MAX_SPEED ? 1 : -1;
                  // defender.yAcc = playerYDist > MAX_SPEED ? 1 : -1;
                  defender.xVel = MAX_SPEED * (playerXDist > 0 ? 1 : -1);
                  defender.yVel = MAX_SPEED * (playerYDist > 0 ? 1 : -1);
                }
                else if (withoutDistance(defender, player, 256)) {
                  playerXDist = defender.x - player.x;
                  playerYDist = defender.y - player.y;
                  // defender.xAcc = playerXDist > MAX_SPEED ? -1 : 1;
                  // defender.yAcc = playerYDist > MAX_SPEED ? -1 : 1;
                  defender.xVel = MAX_SPEED * (playerXDist > 0 ? -1 : 1);
                  defender.yVel = MAX_SPEED * (playerYDist > 0 ? -1 : 1);
                }
                else {
                  // defender.xAcc = 0;
                  // defender.yAcc = 0;
                  defender.xVel = 0;
                  defender.yVel = 0;
                }
                // updateActorVelocity(&defender);
                moveActor(&defender);
              }

              // Fire defender bolts
              if (frames % 45 == 0) {
                for (char i = 0; i < BOLTS_COUNT; i++) {
                  if (enemyBolts[i].life == 0) {
                    fireBolt(&defender, &enemyBolts, i);
                    break;
                  }
                }
              }
              updateRenderPoints(&defender, DEFENDER_POINTS, DEFENDER_POINTS_COUNT);
              setPaletteColor(gRenderer, 6);
              fillPolygon(gRenderer, &defender.points, DEFENDER_POINTS_COUNT);
              setPaletteColor(gRenderer, 0);
              SDL_RenderDrawLines(gRenderer, &defender.points, DEFENDER_POINTS_COUNT);

              updateRenderPoints(&swooper, SWOOPER_POINTS, SWOOPER_POINTS_COUNT);
              setPaletteColor(gRenderer, 1);
              fillPolygon(gRenderer, &swooper.points, SWOOPER_POINTS_COUNT);
              setPaletteColor(gRenderer, 0);
              SDL_RenderDrawLines(gRenderer, &swooper.points, SWOOPER_POINTS_COUNT);

              updateRenderPoints(&leech, LEECH_POINTS, LEECH_POINTS_COUNT);
              setPaletteColor(gRenderer, 3);
              fillPolygon(gRenderer, &leech.points, LEECH_POINTS_COUNT);
              setPaletteColor(gRenderer, 1);
              SDL_RenderDrawLines(gRenderer, &leech.points, LEECH_POINTS_COUNT);

              // Enemy bolts
              for (char i = 0; i < BOLTS_COUNT; i++) {
                if (enemyBolts[i].life > 0) {
                  enemyBolts[i].life--;
                  moveActor(&enemyBolts[i]);
                  if (outsideBounds(enemyBolts[i])) {
                    enemyBolts[i].life = 0;
                  }
                  else {
                    updateRenderPoints(&enemyBolts[i], BOLT_POINTS, BOLT_POINTS_COUNT);
                    setPaletteColor(gRenderer, 5);
                    fillPolygon(gRenderer, &enemyBolts[i].points, BOLT_POINTS_COUNT);

                    setPaletteColor(gRenderer, 6);
                    SDL_RenderDrawLines(gRenderer, &enemyBolts[i].points, BOLT_POINTS_COUNT);
                    // Collisions
                    if (withinDistance(enemyBolts[i], player, 8)) {
                      enemyBolts[i].life = 0;
                      // player.life = 0;
                    }
                  }
                }
              }

              // Projectiles
              for (unsigned int i = 0; i < 10; i++) {
                if (&projectiles[i] != NULL && projectiles[i].life > 0) {
                  projectiles[i].life--;
                  moveActor(&projectiles[i]);
                  if (outsideBounds(projectiles[i])) {
                    projectiles[i].life = 0;
                  }
                  else {
                    updateRenderPoints(&projectiles[i], BOLT_POINTS, BOLT_POINTS_COUNT);
                    setPaletteColor(gRenderer, 1);
                    fillPolygon(gRenderer, &projectiles[i].points, BOLT_POINTS_COUNT);

                    setPaletteColor(gRenderer, 7);
                    SDL_RenderDrawLines(gRenderer, &projectiles[i].points, BOLT_POINTS_COUNT);
                    // Collisions
                    if (withinDistance(projectiles[i], lightning, 16)) {
                      projectiles[i].life = 0;
                      lightning.life = 0;
                    }
                  }
                }
              }

              // Draw player
              setPaletteColor(gRenderer, 4);
              fillPolygon(gRenderer, &player.points, PLAYER_POINTS_COUNT);
              setPaletteColor(gRenderer, 5);
              SDL_RenderDrawLines(gRenderer, &player.points, PLAYER_POINTS_COUNT);

              // Render UI
              float playerX = player.x - camera[0];
              float playerY = player.y - camera[1];
              // Update reticule position to not overlap player
              int xDist = mouse.x - playerX;
              int yDist = mouse.y - playerY;
              float distance = sqrt(xDist * xDist + yDist * yDist);
              if (distance < 32) {
                float mSin = sin(player.r);
                float mCos = cos(player.r);
                reticule[0] = playerX + 32 * mCos;
                reticule[1] = playerY + 32 * mSin;
              }
              else {
                reticule[0] = mouse.x;
                reticule[1] = mouse.y;
              }

              // Target Reticule for Bolt
              float pSin = sin(player.r);
              float pCos = cos(player.r);
              // setPaletteColor(gRenderer, 5);
              SDL_SetRenderDrawColor(gRenderer, 0x33, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
              float x1, y1, x2, y2;
              for (unsigned char i = 0; i < RETICULE_LINES_COUNT; i++) {
                x1 = RETICULE_LINES[4 * i]     * pCos + RETICULE_LINES[4 * i + 1] * pSin;
                y1 = RETICULE_LINES[4 * i]     * pSin - RETICULE_LINES[4 * i + 1] * pCos;
                x2 = RETICULE_LINES[4 * i + 2] * pCos + RETICULE_LINES[4 * i + 3] * pSin;
                y2 = RETICULE_LINES[4 * i + 2] * pSin - RETICULE_LINES[4 * i + 3] * pCos;
                SDL_RenderDrawLine(gRenderer, reticule[0] + x1, reticule[1] + y1, reticule[0] + x2, reticule[1] + y2);
              }
              x1 = -8 * pCos;
              y1 = -8 * pSin;
              x2 =  7 * pCos;
              y2 =  7 * pSin;
              SDL_RenderDrawLine(gRenderer, reticule[0] + x1, reticule[1] + y1, reticule[0] + x2, reticule[1] + y2);

              SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
              SDL_RenderFillRect(gRenderer, &UI_RECT);
              renderText(gRenderer, "Lives", 5, 0, 0);
              break;
          }

          // Actually render to the window
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
