#ifndef _MAP_H
#define _MAP_H

#define TILE_TYPE_BACKGROUND 0
#define TILE_TYPE_SOLID 1
#define TILE_TYPE_DOOR 2
#define TILE_TYPE_STAIRS 3

void mapInit();

void mapFree();

void drawMap();

void markTouchedTiles(Rect rect);

void restoreMapTiles();

#endif