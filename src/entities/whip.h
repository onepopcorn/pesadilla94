#ifndef _WHIP_H
#define _WHIP_H

Entity *whipSpawn(int x, int y, bool facingRight);

void whipUpdate(struct Entity *e, uint8_t tileCollisions);

#endif  // _WHIP_H