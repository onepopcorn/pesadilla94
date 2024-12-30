#ifndef _RES_H
#define _RES_H

typedef unsigned char uint8_t;

#include "../shared/files/spritesheet.h"
#include "../shared/files/level.h"

#define RES_TYPE_SPRITE 1
#define RES_TYPE_MAP 2

Sprite *loadSprite(const char *filePath);
Map *loadMap(const char *filePath);

#endif  // _RES_H