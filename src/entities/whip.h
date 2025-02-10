#ifndef _WHIP_H
#define _WHIP_H

Entity *whipSpawn(int x, int y, bool facingRight);

void whipUpdate(struct Entity *e, struct Entity *player, uint8_t tileCollisions);

#endif  // _WHIP_H