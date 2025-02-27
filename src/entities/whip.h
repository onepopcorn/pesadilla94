#ifndef _WHIP_H
#define _WHIP_H

Entity *whipSpawn(uint16_t x, uint16_t y, bool facingRight);

void whipUpdate(struct Entity *e, uint8_t tileCollisions);

#endif  // _WHIP_H