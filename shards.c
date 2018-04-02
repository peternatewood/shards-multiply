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
  + Use to move to different regions; point in direction of desired region in the overworld
  + Moves in player's facing direction when activated, and player cannot rotate, just usual movement directions
  + Should mouse rotate player slowly or no? Does this impact navigation?
  + What enemies or other scenarios happen in thruster mode?
  + Add slight screen shake, stream of stars, and other visuals for cool factor
  + unlimited while using, but needs to recharge after leaving thrust mode
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

#include "shards.h"

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
// Initialize SDL, and create window and renderer. Returns whether all this was successful or not
bool init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  else {
    gWindow = SDL_CreateWindow("Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (gWindow == NULL) printf("Window creation failed! SDL Error: %s\n", SDL_GetError());
    else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) printf("Surface creation failed! SDL Error: %s\n", SDL_GetError());
      else if (SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND) < 0) printf("Failed to set render draw blend mode! SDL Error: %s\n", SDL_GetError());
      else {
        return true;
      }
    }
  }

  return false;
}

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
    titleX += 64;
    for (unsigned short i = 0; i < TITLE_LENGTH; i++) {
      for (unsigned short p = 0; p < TITLE_CHAR_COUNTS[i]; p++) {
        titlePoints[i][p].x = TITLE_POINTS[i][p].x + titleX;
      }
    }
  }
}

// Camera position
unsigned int camera[2] = { 0, 0 };
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

int main() {
  if (init()) {
    charset = loadTexture("charset.png");
    // Hide the mouse cursor
    if (SDL_ShowCursor(SDL_DISABLE) < 0) printf("Failed to hide system cursor. SDL Error: %s\n", SDL_GetError());

    // Seed random number generator
    srand(time(NULL));

    bool isRunning = true;
    SDL_Event event;
    enum Scene scene = TITLE;

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
            // // TODO: Stop jittering zigzags if the player nears a level boundary. Maybe loop levels?
            // player.xVel = THRUST_SPEED * cos(player.r);
            // player.yVel = THRUST_SPEED * sin(player.r);
            // // Update camera position
            // int camX = camera[0] + player.speed * player.xVel;
            // int centerXDist = (player.x - WINDOW_W / 2) - camX;
            // if (camX != centerXDist) {
            //   if (centerXDist > -THRUST_SPEED && centerXDist < THRUST_SPEED) {
            //     camX = player.x - WINDOW_W / 2;
            //   }
            //   else {
            //     camX += centerXDist > 0 ? THRUST_SPEED : -THRUST_SPEED;
            //   }
            // }
            // // int camX = player.x - WINDOW_W / 2;
            // if (camX < 0) {
            //   camX = 0;
            // }
            // else if (camX > LEVEL_SIZE - WINDOW_W) {
            //   camX = LEVEL_SIZE - WINDOW_W;
            // }
            // camera[0] = camX;

            // int camY = camera[1] + player.speed * player.yVel;
            // int centerYDist = (player.y - WINDOW_H / 2) - camY;
            // if (camY != centerYDist) {
            //   if (centerYDist > -THRUST_SPEED && centerXDist < THRUST_SPEED) {
            //     camY = player.y - WINDOW_H / 2;
            //   }
            //   else {
            //     camY += centerYDist > 0 ? THRUST_SPEED : -THRUST_SPEED;
            //   }
            // }
            // // int camY = player.y - WINDOW_H / 2;
            // if (camY < 0) {
            //   camY = 0;
            // }
            // else if (camY > LEVEL_SIZE - WINDOW_H) {
            //   camY = LEVEL_SIZE - WINDOW_H;
            // }
            // camera[1] = camY;
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
          if (player.x < camera[0] + UI_HEIGHT) {
            player.x = camera[0] + UI_HEIGHT;
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

              if (tileY < UI_HEIGHT) {
                levelTile.h = TILE_SIZE + tileY;
                tileY = UI_HEIGHT;
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
  }

  // Free up memory used by SDL objects and functions
  if (charset != NULL) {
    SDL_DestroyTexture(charset);
    charset = NULL;
  }
  if (gRenderer != NULL) {
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
  }
  if (gWindow != NULL) {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
  }

  SDL_Quit();

  return 0;
}
