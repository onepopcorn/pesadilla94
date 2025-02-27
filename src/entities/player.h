#ifndef _PLAYER_H
#define _PLAYER_H

typedef enum PlayerState {
    STATE_IDLE,
    STATE_WALKING,
    STATE_SEARCHING,
    STATE_STAIRS,
    STATE_DYING,
    STATE_SHOOTING,
} PlayerState;

Entity* playerSpawn(uint16_t x, uint16_t y);

void playerUpdate(struct Entity* entity, uint8_t tileCollisions);

void playerDie();

#endif  // _PLAYER_H