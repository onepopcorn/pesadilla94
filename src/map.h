#ifndef _MAP_H
#define _MAP_H

#include "physics/geom.h"

// Used to swap tiles for open/close doors or exhausting vending machine
#define TILE_DOOR_OPEN_TOP 25
#define TILE_DOOR_OPEN_BOTTOM 26
#define TILE_DOOR_CLOSED_TOP 16
#define TILE_DOOR_CLOSED_BOTTOM 17
#define TILE_VENDING_DISABLED_TOP 38
#define TILE_VENDING_DISABLED_BOTTOM 39

#define NUM_LEVELS 6

extern const LevelData levels[];
extern const Vec2 spawnPositions[];
extern uint8_t currentLevel;

uint8_t startLevel(uint8_t level);

void endLevel();

void drawMap();

Rect getTilesRect(Rect spriteRect);

uint8_t markDirtyTile(uint8_t entityIdx, uint8_t tileXIndex, uint8_t tileYIndex);

Tile* getTile(uint8_t col, uint8_t row);

Vec2 getStairsDestination(uint16_t x, uint16_t y, bool up);

Tile* openDoor(uint16_t x, uint16_t y);

void closeDoor(uint16_t x, uint16_t y);

void disableVendingMachine(uint16_t x, uint16_t y);

void restoreMapTiles();

#endif  // _MAP_H