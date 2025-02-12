#ifndef _MAP_H
#define _MAP_H

#include "physics/geom.h"

// TODO: set defines for changing tiles (e.g. open/closed doors)
#define TILE_DOOR_OPEN_TOP 25
#define TILE_DOOR_OPEN_BOTTOM 26
#define TILE_DOOR_CLOSED_TOP 16
#define TILE_DOOR_CLOSED_BOTTOM 17

#define NUM_LEVELS 3

extern uint8_t currentLevel;
extern const char* maps[NUM_LEVELS];

uint8_t startLevel(uint8_t level);

void endLevel();

void drawMap();

Rect getTilesRect(Rect spriteRect);

int markDirtyTile(int entityIdx, int tileXIndex, int tileYIndex);

Tile* getTile(int col, int row);

Vec2 getStairsDestination(float x, float y, bool up);

Tile* openDoor(float x, float y);

Tile* closeDoor(float x, float y);

void restoreMapTiles();

#endif  // _MAP_H