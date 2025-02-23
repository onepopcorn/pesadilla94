#ifndef _ENEMY_H_
#define _ENEMY_H_

struct Entity;
struct Entity *enemySpawn(short x, short y);

void enemyUpdate(struct Entity *e, uint8_t tileCollisions);
void enemyStun(uint8_t idx);
void enemyWakeup(uint8_t id);

#endif  // _ENEMY_H_