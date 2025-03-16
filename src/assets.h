#ifndef _ASSETS_H
#define _ASSETS_H

extern Sprite* font;
extern Sprite* playerSprite;
extern Sprite* enemySprite;
extern Sprite* whipSprite;
extern Sprite* tileset;
extern Sprite* miscSprite;

uint8_t assetsInit();
void assetsFree();

#endif  // _ASSETS_H