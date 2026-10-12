#ifndef _PLAYER_H
#define _PLAYER_H

typedef enum PlayerState {
    STATE_IDLE,
    STATE_WALKING,
    STATE_SEARCHING,
    STATE_STAIRS_IN,
    STATE_STAIRS_OUT,
    STATE_DYING,
    STATE_SHOOTING,
    STATE_PICKING_UP,
} PlayerState;

Entity* playerSpawn(int x, int y);

void playerUpdate(struct Entity* entity, uint8_t tileCollisions);

void playerDie();

#endif  // _PLAYER_H