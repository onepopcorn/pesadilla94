#ifndef _RES_H
#define _RES_H

typedef unsigned char uint8_t;

#include "../shared/files/spritesheet.h"
#include "../shared/files/level.h"
#include "../shared/lib/bucket-lib.h"
enum ResoureType {
    RES_TYPE_SPRITE,
    RES_TYPE_MAP,
    RES_TYPE_SOUND,
};

Sprite* loadSprite(Arena* arena, const char* filePath);
Map* loadMap(Arena* arena, const char* filePath);
// SAMPLE *loadSfx(Arena* arena, const char *filePath);

#endif  // _RES_H