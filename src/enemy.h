#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Entity *enemyInit(int x, int y);

void enemyUpdate(struct Entity *e, struct Entity *player, uint8_t tileCollisions);

void enemyFree();

#endif  // _ENEMY_H_