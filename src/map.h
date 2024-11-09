#ifndef _MAP_H
#define _MAP_H

#define TILE_TYPE_BACKGROUND 0
#define TILE_TYPE_SOLID 1
#define TILE_TYPE_DOOR 2
#define TILE_TYPE_STAIRS 3

#define TILE_STATE_DIRTY 0x01
#define TILE_STATE_FLIP 0x02

void mapInit();

void drawMap(int offset_x, int offset_y);

void restoreBackground(Rect rect);

#ifdef DEBUG
void restoreBackgroundDebug(Rect rect);
#endif

#endif