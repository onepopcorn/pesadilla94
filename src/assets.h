#ifndef _ASSETS_H
#define _ASSETS_H

#include "../vendor/bucket-lib/bucket-lib.h"

extern Sprite* font;
extern Sprite* playerSprite;
extern Sprite* enemySprite;
extern Sprite* whipSprite;
extern Sprite* tileset;
extern Sprite* miscSprite;

uint8_t assetsInit(Arena* arena);

#endif  // _ASSETS_H