#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct Player {
    uint8_t lives;
} Player;

struct Entity* playerInit(int x, int y);

void updatePlayer(struct Entity* entity, struct Entity* player, uint8_t tileCollisions);

void playerFree();

#endif  // _PLAYER_H