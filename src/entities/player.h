#ifndef _PLAYER_H
#define _PLAYER_H

#define PLAYER_SHOOTING 0x01
#define PLAYER_ON_STAIRS 0x02
#define PLAYER_SEARCHING 0x04
#define PLAYER_DYING 0x08

#define NO_MOVE (PLAYER_DYING | PLAYER_ON_STAIRS | PLAYER_SHOOTING | PLAYER_SEARCHING)
#define NO_SHOOT (PLAYER_SHOOTING | PLAYER_ON_STAIRS | PLAYER_DYING | PLAYER_SEARCHING)

Entity* playerSpawn(int x, int y);

void playerUpdate(struct Entity* entity, struct Entity* player, uint8_t tileCollisions);

#endif  // _PLAYER_H