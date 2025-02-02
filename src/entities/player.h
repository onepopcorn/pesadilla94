#ifndef _PLAYER_H
#define _PLAYER_H

#define PLAYER_SHOOTING 0x01
#define PLAYER_ON_STAIRS 0x02
#define PLAYER_SEARCHING 0x04
#define PLAYER_DYING 0x08

typedef struct Player {
    uint8_t lives;
} Player;

void playerInit();

Entity* playerSpawn(int x, int y);

void updatePlayer(struct Entity* entity, struct Entity* player, uint8_t tileCollisions);

void setAnimation();

void playerFree();

#endif  // _PLAYER_H