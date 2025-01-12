#ifndef _MAP_H
#define _MAP_H

#include "physics/geom.h"

void mapInit();

void mapFree();

void drawMap();

Rect getTilesRect(Rect spriteRect);

int markDirtyTile(int entityIdx, int tileXIndex, int tileYIndex);

int getTileType(int x, int y);

void restoreMapTiles();

#endif  // _MAP_H