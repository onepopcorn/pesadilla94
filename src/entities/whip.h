#ifndef _WHIP_H
#define _WHIP_H

void whipInit();

Entity *whipSpawn(int x, int y, bool facingRight);

void whipUpdate(struct Entity *e, struct Entity *player, uint8_t tileCollisions);

void whipFree();

#endif  // _WHIP_H