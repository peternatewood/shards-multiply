# Shards Multiply
A twin-stick shoot-em-up with free-movement. Each mission is full of enemy ships and priority targets. Eliminate the targets to summon the boss. All enemy ships are either shards or are composed of shards; larger ships break into shards as they take damage.

## Status
**Development:** Active<br/>
**Version:** Alpha<br/>
**Live on Website:** No

## Core Features
* [ ] **Player Movement**
  - [x] Horizontal and vertical
  - [x] Ship points toward cursor
  - [x] Friction
  - [ ] Move through mission area
* [x] **Custom Mouse Cursor**
* [ ] **Pre-game Hangar**
  - [x] Controls visualized on floor
  - [x] Controls respond to input
  - [ ] Options menus accessible by shooting them
  - [ ] Start chosen mission by entering start box
* [ ] **Customize Controls**
* [ ] **Customize Ship**
* [ ] **Multiple Missions**
* [ ] **Powerups**
  - [ ] Duplicate Plasma Bolts
  - [ ] Missile Ammo
  - [ ] Laser Beam Battery
  - [ ] Clone Ship
* [ ] **Missions**
  - [ ] Powerups placed throughout area
  - [ ] Unique boss per mission
* [ ] **Enemy Ships**
  - [ ] Several types of shard
  - [ ] Large enemies break into component shards
  - [ ] Shards maintain damage from previous stage
* [ ] **Audio**
  - [ ] Sound effects
  - [ ] Music

## How to Play
Aim your ship with the mouse and move using the keyboard. There are two modes of movement: moving in four directions relative to the screen, and moving through the mission area in the direction of the mouse. Normal, cardinal movement restricts the ship to the size of the screen, while firing thrusters propels the ship across the larger mission area, using the mouse cursor to determine which direction to go.

Shards are the basic components of enemy ships. Individual shards have their own attack patterns, and ships made of shards will shatter into their components once they've taken enough damage.

### Default Controls
W - move up<br/>
S - move down<br/>
A - move left<br/>
D - move right

Spacebar* - fire thrusters

Primary Mouse Button (left on most mice, right on left-handed mice) - fire plasma bolts (unlimited)<br/>
Secondary Mouse Button* - fire loaded secondary weapon (ammunition-based)

_*not yet implemented_

## Compile the C Version

To compile the C version of the game, run "make" in the terminal on Linux or Apple OSX computers, or use your favorite IDE on Windows.
