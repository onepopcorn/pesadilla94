#ifndef RES_H
#define RES_H

#include <stdint.h>

#define RES_TYPE_SPRITE 1
#define RES_TYPE_MAP 2

#include "../shared/files/spritesheet.h"
#include "../shared/files/map.h"

Sprite *loadSprite(const char *filePath);

Map *loadMap(const char *filePath);

#define MAP_W 20
#define MAP_H 10

// TODO: This is temporary, remove when loading maps from file
static const uint8_t MAPS[MAP_W * MAP_H * 2] = {
    // Map 1
    2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 8, 8, 8, 8,
    15, 16, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 30, 5, 5, 7, 18, 19, 20, 8,
    23, 23, 4, 5, 48, 84, 5, 49, 50, 51, 52, 53, 54, 5, 5, 7, 36, 37, 38, 8,
    46, 46, 4, 5, 74, 84, 84, 84, 9, 5, 76, 77, 78, 5, 5, 107, 107, 107, 107, 107,
    72, 72, 100, 5, 93, 84, 84, 84, 94, 5, 95, 96, 97, 5, 5, 98, 33, 99, 107, 107,
    112, 112, 105, 106, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 57, 58, 108, 108,
    112, 112, 112, 113, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 115, 82, 112, 112,
    112, 112, 112, 116, 113, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 117, 112,
    121, 121, 121, 119, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 119, 121, 121, 121, 121,
    122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122,
    // Map 2
    2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 8, 8, 8, 8,
    15, 16, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 18, 19, 20, 8,
    23, 23, 4, 5, 5, 84, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 36, 37, 38, 8,
    46, 46, 4, 5, 5, 84, 5, 84, 5, 5, 5, 5, 5, 5, 5, 107, 107, 107, 107, 107,
    72, 72, 100, 5, 5, 84, 5, 84, 5, 5, 5, 5, 5, 5, 5, 98, 107, 107, 107, 107,
    112, 112, 105, 106, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 111, 117, 112, 112,
    112, 112, 112, 113, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 117, 112,
    112, 112, 112, 116, 113, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 117,
    121, 121, 121, 119, 121, 121, 121, 121, 118, 118, 121, 121, 121, 121, 121, 119, 121, 121, 121, 121,
    122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122};

#endif