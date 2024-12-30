#ifndef _MAP_H
#define _MAP_H

#include "physics/geom.h"

#define TILE_NONE 0
#define TILE_WALL_L 1
#define TILE_WALL_R 2
#define TILE_DOOR 3
#define TILE_STAIRS 4
#define TILE_FLOOR 5

void mapInit();

void mapFree();

void drawMap();

Rect getTilesRect(Rect spriteRect);

int markDirtyTile(int entityIdx, int tileXIndex, int tileYIndex);

int getTileType(int x, int y);

void restoreMapTiles();

#endif  // _MAP_H