# Shards Multiply
A twin-stick shoot-em-up with free-movement. Each mission is full of enemy ships and priority targets. Eliminate the targets to summon the boss. All enemy ships are either shards or are composed of shards; larger ships break into shards as they take damage.

## Status
**Development:** Active<br/>
**Version:** Alpha<br/>
**Live on Website:** No

## Core Features
* [x] **Player Movement**
  - [x] Horizontal and vertical
  - [x] Ship points toward cursor
  - [x] Friction
  - [x] Move through mission area
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
  - [ ] Mulitply Plasma Bolts
  - [ ] Clone Ship
  - [ ] Missiles
  - [ ] Laser
  - [ ] Shield
  - [ ] Shard Armor
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

Spacebar - fire thrusters

Primary Mouse Button (left on most mice, right on left-handed mice) - fire plasma bolts (unlimited)<br/>
Secondary Mouse Button* - fire loaded secondary weapon (ammunition-based)

_*not yet implemented_

## Compile the C Version

The C version runs locally without any need for an internet connection, just like any old game. You'll need to [get SDL2 here](https://www.libsdl.org/download-2.0.php), but which package you download and how you use it will depend on your operating system of choice.
### OSX
You have two options in OSX: XCode and the command line. For both, you can download the .framework package and add it to `/Library/Frameworks`. If you know how to use XCode, just go ahead and compile from there.

If you prefer the command line like me, and avoid using XCode whenever possible, here's a simple Makefile to handle all the grunt work:
```
COMPILER = gcc
COMPILER_FLAGS = --std=c99 -w -Wall
LINKER_FLAGS = -framework SDL2
TARGET = shards

all: $(TARGET).c
	$(COMPILER) $(TARGET).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)
```
### Ubuntu, Debian-based systems
Maybe the easiest option, all you need for Debian is to `apt-get install libsdl2-2.0` (the latest version at the time of writing). The SDL Wiki provides a more thorough walkthrough for [Linux and Unix systems here](https://wiki.libsdl.org/Installation#Linux.2FUnix).<br/>

Your Makefile will look much like the OSX one, except for the linker flags:
```
COMPILER = gcc
COMPILER_FLAGS = --std=c99 -w -Wall
LINKER_FLAGS = -lSDL2
TARGET = shards

all: $(TARGET).c
	$(COMPILER) $(TARGET).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

```
### Windows
Coming soon? Windows makes this so hard...
