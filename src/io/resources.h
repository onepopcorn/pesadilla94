#ifndef RES_H
#define RES_H

#include <stdint.h>
#include "../shared/files/spritesheet.h"
#include "../shared/files/level.h"

#define RES_TYPE_SPRITE 1
#define RES_TYPE_MAP 2

Sprite *loadSprite(const char *filePath);
Map *loadMap(const char *filePath);

#endif