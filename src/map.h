#ifndef _MAP_H
#define _MAP_H

#include "physics/geom.h"
#include "../shared/lib/bucket-lib.h"

// Used to swap tiles for open/close doors or exhausting vending machine
enum TileCode {
    TILE_DOOR_OPEN_TOP = 25,
    TILE_DOOR_OPEN_BOTTOM = 26,
    TILE_DOOR_CLOSED_TOP = 16,
    TILE_DOOR_CLOSED_BOTTOM = 17,
    TILE_VENDING_DISABLED_TOP = 38,
    TILE_VENDING_DISABLED_BOTTOM = 39,
};

#define NUM_LEVELS 10

extern const LevelData levels[];
extern const Vec2 spawnPositions[];
extern uint8_t currentLevel;

uint8_t startLevel(Arena* arena, uint8_t level);

void endLevel();

void drawMap();

Rect getTilesRect(Rect spriteRect);

int markDirtyTile(uint8_t entityIdx, uint8_t tileXIndex, uint8_t tileYIndex);

Tile* getTile(uint8_t col, uint8_t row);

Vec2 getStairsDestination(int x, int y, bool up);

Tile* openDoor(int x, int y);

void closeDoor(int x, int y);

void disableVendingMachine(int x, int y);

void restoreMapTiles();

#endif  // _MAP_H