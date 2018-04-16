/* Development build; alter error messages for production release */

/*
Movement:
  + On reaching level border, loop around to other side
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
SDL_Texture* charset;
// Load a texture from an image file at the path provided
SDL_Texture* loadTexture(char path[]) {
  // The final texture
  SDL_Texture* newTexture = NULL;
  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path);
  if (loadedSurface == NULL) printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
  else {
    // Don't use color keying if the source has a transparency layer already (.png, or .tga extension)
    // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}

Mix_Chunk* gBattleMusic = NULL;
Mix_Chunk* gShipSound = NULL;
Mix_Chunk* gThrustSound = NULL;
Mix_Chunk* gStart = NULL;
Mix_Chunk* gBoltSound = NULL;
Mix_Chunk* gMissileSound = NULL;
Mix_Chunk* gLaserSound = NULL;

// Initialize SDL, and create window and renderer. Returns whether all this was successful or not
bool init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, CHANNELS, LATENCY) < 0) {
    printf("Failed to initialize SDL_Mixer! SDL_mixer Error: %s\n", Mix_GetError());
    return false;
  }

  gWindow = SDL_CreateWindow("Shards", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (gWindow == NULL) {
    printf("Window creation failed! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL) {
    printf("Surface creation failed! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  if (SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND) < 0) {
    printf("Failed to set render draw blend mode! SDL Error: %s\n", SDL_GetError());
    return false;
  }
  // Load character set image for rendering text
  charset = loadTexture("charset.png");
  if (charset == NULL) {
    printf("Failed to load charset texture! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Load audio files
  gBattleMusic = Mix_LoadWAV(BATTLE_MUSIC);
  if (gBattleMusic == NULL) {
    printf("Failed to load music \"%s\"! SDL_mixer Error: %s\n", BATTLE_MUSIC, Mix_GetError());
    return false;
  }
  gShipSound = Mix_LoadWAV(SHIP_SOUND);
  if (gShipSound == NULL) {
    printf("Failed to load music \"%s\"! SDL_mixer Error: %s\n", SHIP_SOUND, Mix_GetError());
    return false;
  }
  gThrustSound = Mix_LoadWAV(THRUST_SOUND);
  if (gThrustSound == NULL) {
    printf("Failed to load music \"%s\"! SDL_mixer Error: %s\n", THRUST_SOUND, Mix_GetError());
    return false;
  }
  gStart = Mix_LoadWAV(CHANGE_SOUND);
  if (gStart == NULL) {
    printf("Failed to load wav \"%s\"! SDL_mixer Error: %s\n", CHANGE_SOUND, Mix_GetError());
    return false;
  }
  gBoltSound = Mix_LoadWAV(BOLT_SOUND);
  if (gBoltSound == NULL) {
    printf("Failed to load wav \"%s\"! SDL_mixer Error: %s\n", BOLT_SOUND, Mix_GetError());
    return false;
  }
  gMissileSound = Mix_LoadWAV(MISSILE_SOUND);
  if (gMissileSound == NULL) {
    printf("Failed to load wav \"%s\"! SDL_mixer Error: %s\n", MISSILE_SOUND, Mix_GetError());
    return false;
  }
  gLaserSound = Mix_LoadWAV(LASER_SOUND);
  if (gLaserSound == NULL) {
    printf("Failed to load music \"%s\"! SDL_mixer Error: %s\n", LASER_SOUND, Mix_GetError());
    return false;
  }

  return true;
}

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
    // int weaponMode = 0;
    enum Mode weaponMode = BOLT;
    struct Actor player = { 100.f, 100.f, 0.f, 0.f, PLAYER_SPEED, 100, 0, 0, 0 };
    SDL_Point mouse = { 0, 0 };
    // x, y
    #define STARS_COUNT 64
    int starStreams[2 * STARS_COUNT];
    for (char i = 0; i < STARS_COUNT; i++) {
      starStreams[2 * i]      = rand() % WINDOW_W; // x position
      starStreams[2 * i + 1]  = rand() % WINDOW_H; // y position
    }
    // Visual reticule
    int reticule[2] = { 0, 0 };
    // Reuseable rect for rendering level tiles
    SDL_Rect levelTile = { 0, 0, TILE_SIZE, TILE_SIZE };
    const SDL_Rect UI_RECT = { 0, 0, WINDOW_W, UI_HEIGHT };

    #define LASER_POINTS_COUNT 6
    const SDL_Point LASER_POINTS[LASER_POINTS_COUNT] = {
      {   0, 6 },
      { 480, 4 },
      { 640, 0 },
      { 480,-4 },
      {   0,-6 },
      {   0, 6 }
    };
    SDL_Point laserPoints[LASER_POINTS_COUNT] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

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
    bool startLaser = false;
    bool enginesOn = false;

    int musicChannel, engineChannel, thrustChannel, laserChannel;

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
    struct Actor missiles[10] = {
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
    int missileIndex = 0;
    int fireDelay = 0;

    musicChannel = Mix_PlayChannel(-1, gBattleMusic, -1);

    while (isRunning) {
      // Background color
      SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
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
            case SDLK_SPACE:
              thrustMode = true;
              Mix_HaltChannel(musicChannel);
              thrustChannel = Mix_PlayChannel(-1, gThrustSound, -1);
              break;
            case SDLK_LSHIFT:
              weaponMode++;
              if (weaponMode == LAST) weaponMode = BOLT;
              Mix_PlayChannel(-1, gStart, 0);
              break;
          }
          if (!enginesOn && (player.xAcc || player.yAcc)) {
            enginesOn = true;
            engineChannel = Mix_PlayChannel(-1, gShipSound, -1);
          }
        }
        else if (event.type == SDL_KEYUP) {
          if (wasInputActive(event.key.keysym.sym)) {
            switch (event.key.keysym.sym) {
              case SDLK_w: player.yAcc += 1; break;
              case SDLK_s: player.yAcc -= 1; break;
              case SDLK_a: player.xAcc += 1; break;
              case SDLK_d: player.xAcc -= 1; break;
              case SDLK_SPACE:
                thrustMode = false;
                musicChannel = Mix_PlayChannel(-1, gBattleMusic, -1);
                Mix_HaltChannel(thrustChannel);
                break;
            }
            if (enginesOn && player.xAcc == 0 && player.yAcc == 0) {
              enginesOn = false;
              Mix_HaltChannel(engineChannel);
            }
          }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
          if (scene == TITLE) scene = GAME;

          activateMouseInput(event.button.button);
          switch (event.button.button) {
            case SDL_BUTTON_LEFT:
              shouldFireBolts = true;
              if (weaponMode == LASER) {
                startLaser = true;
              }
              break;
            case SDL_BUTTON_RIGHT : break;
          }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
          if (wasMouseInputActive(event.button.button)) {
            switch (event.button.button) {
              case SDL_BUTTON_LEFT:
              if (weaponMode == LASER && shouldFireBolts) {
                Mix_HaltChannel(laserChannel);
              }
              shouldFireBolts = false;
                break;
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
          updateTitlePoints();
          // Render title
          for (unsigned short i = 0; i < TITLE_LENGTH; i++) {
            SDL_SetRenderDrawColor(gRenderer, 0xBB, 0x66, 0x11, SDL_ALPHA_OPAQUE);
            fillPolygon(gRenderer, &titlePoints[i], TITLE_CHAR_COUNTS[i]);
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xDD, 0x88, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLines(gRenderer, &titlePoints[i], TITLE_CHAR_COUNTS[i]);
          }
          // Render start flash
          if (titleX >= 64 && frames % 36 < 18) {
            renderText(gRenderer, "Press any key to start", 22, WINDOW_W / 2 - 176, 440);
          }
          break;
        case GAME:
          switch (weaponMode) {
            case BOLT:
              if (shouldFireBolts && fireDelay == 0) {
                fireBolt(&player, &projectiles, boltIndex);
                boltIndex++;
                if (boltIndex >= 10) boltIndex = 0;
                fireDelay = 6;
                Mix_PlayChannel(-1, gBoltSound, 0);
              }
              else if (fireDelay > 0) fireDelay--;
              break;
            case MISSILE:
              if (shouldFireBolts && fireDelay == 0) {
                fireBolt(&player, &missiles, missileIndex);
                missileIndex++;
                if (missileIndex >= 10) missileIndex = 0;
                fireDelay = 12;
                Mix_PlayChannel(-1, gMissileSound, 0);
              }
              else if (fireDelay > 0) fireDelay--;
              break;
            case LASER:
              break;
          }

          if (startLaser) {
            laserChannel = Mix_PlayChannel(-1, gLaserSound, -1);
            startLaser = false;
          }

          updateActorVelocity(&player);
          moveActor(&player);

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
          if (!thrustMode) {
            player.r = atan2(mouse.y - (player.y - camera[1]), mouse.x - (player.x - camera[0]));
          }
          updateRenderPoints(&player, PLAYER_POINTS, PLAYER_POINTS_COUNT);

          if (thrustMode) {
            // Render star streams
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
            float xVel = cos(player.r);
            float yVel = sin(player.r);
            for (char i = 0; i < STARS_COUNT; i++) {
              starStreams[2 * i]     -= 32 * xVel;
              starStreams[2 * i + 1] -= 32 * yVel;

              if (starStreams[2 * i] < 0)             starStreams[2 * i] += WINDOW_W;
              else if (starStreams[2 * i] > WINDOW_W) starStreams[2 * i] -= WINDOW_W;

              if (starStreams[2 * i + 1] < 0)             starStreams[2 * i + 1] += WINDOW_H;
              else if (starStreams[2 * i + 1] > WINDOW_H) starStreams[2 * i + 1] -= WINDOW_H;

              SDL_RenderDrawLine(gRenderer, starStreams[2 * i], starStreams[2 * i + 1], starStreams[2 * i] - 32 * xVel, starStreams[2 * i + 1] - 32 * yVel);
            }
          }
          else {
            int tileX, tileY;
            for (int i = 0; i < LEVEL_RECTS_COUNT; i++) {
              setPaletteColor(gRenderer, LEVEL_SOURCE[i]);
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
              defender.xVel = MAX_SPEED * (playerXDist > 0 ? 1 : -1);
              defender.yVel = MAX_SPEED * (playerYDist > 0 ? 1 : -1);
            }
            else if (withoutDistance(defender, player, 256)) {
              playerXDist = defender.x - player.x;
              playerYDist = defender.y - player.y;
              defender.xVel = MAX_SPEED * (playerXDist > 0 ? -1 : 1);
              defender.yVel = MAX_SPEED * (playerYDist > 0 ? -1 : 1);
            }
            else {
              defender.xVel = 0;
              defender.yVel = 0;
            }
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
            // Missile
            if (&missiles[i] != NULL && missiles[i].life > 0) {
              missiles[i].life--;
              moveActor(&missiles[i]);
              if (outsideBounds(missiles[i])) missiles[i].life = 0;
              else {
                updateRenderPoints(&missiles[i], MISSILE_POINTS, MISSILE_POINTS_COUNT);
                setPaletteColor(gRenderer, 1);
                fillPolygon(gRenderer, &missiles[i].points, MISSILE_POINTS_COUNT);

                setPaletteColor(gRenderer, 0);
                SDL_RenderDrawLines(gRenderer, &missiles[i].points, MISSILE_POINTS_COUNT);
                // Collisions
                if (withinDistance(missiles[i], lightning, 16)) {
                  missiles[i].life = 0;
                  lightning.life = 0;
                }
              }
            }
          }

          // TODO: Draw laser
          if (shouldFireBolts && weaponMode == LASER) {
            float pSin = sin(player.r);
            float pCos = cos(player.r);

            // laserPoints[0].x = player.x + 6 * pCos;
            // laserPoints[0].y = player.y + 6 * pSin;
            // laserPoints[1].x = player.x - 6 * pCos;
            // laserPoints[1].y = player.y - 6 * pSin;
            for (char i = 0; i < LASER_POINTS_COUNT; i++) {
              laserPoints[i].x = player.x + (LASER_POINTS[i].x * pCos + LASER_POINTS[i].y * pSin) - camera[0];
              laserPoints[i].y = player.y + (LASER_POINTS[i].x * pSin - LASER_POINTS[i].y * pCos) - camera[1];
            }
            setPaletteColor(gRenderer, 1);
            fillPolygon(gRenderer, &laserPoints, LASER_POINTS_COUNT);
            setPaletteColor(gRenderer, 7);
            SDL_RenderDrawLines(gRenderer, &laserPoints, LASER_POINTS_COUNT);
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
          renderText(gRenderer, "Weapon:", 7, 0, 0);
          switch (weaponMode) {
            case BOLT: renderText(gRenderer, "Bolts", 5, 128, 0); break;
            case MISSILE: renderText(gRenderer, "Missiles", 8, 128, 0); break;
            case LASER: renderText(gRenderer, "Laser", 5, 128, 0); break;
          }
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
  if (gStart != NULL) {
    Mix_FreeChunk(gStart);
    gStart = NULL;
  }
  if (gBoltSound != NULL) {
    Mix_FreeChunk(gBoltSound);
    gBoltSound = NULL;
  }
  if (gMissileSound != NULL) {
    Mix_FreeChunk(gMissileSound);
    gMissileSound = NULL;
  }
  if (gLaserSound != NULL) {
    Mix_FreeChunk(gLaserSound);
    gLaserSound = NULL;
  }
  if (gThrustSound != NULL) {
    Mix_FreeChunk(gThrustSound);
    gThrustSound = NULL;
  }
  if (gShipSound != NULL) {
    Mix_FreeChunk(gShipSound);
    gShipSound = NULL;
  }
  if (gBattleMusic != NULL) {
    Mix_FreeChunk(gBattleMusic);
    gBattleMusic = NULL;
  }
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

  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
