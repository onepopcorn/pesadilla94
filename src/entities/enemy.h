#ifndef _ENEMY_H_
#define _ENEMY_H_

Entity *enemySpawn(int x, int y);

void enemyUpdate(struct Entity *e, struct Entity *player, uint8_t tileCollisions);

#endif  // _ENEMY_H_