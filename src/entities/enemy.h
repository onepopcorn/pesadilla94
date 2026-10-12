#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Entity;
struct Entity* enemySpawn(int x, int y);

void enemyUpdate(struct Entity* e, uint8_t tileCollisions);
void enemyStun(struct Entity* enemy);

#endif  // _ENEMY_H_