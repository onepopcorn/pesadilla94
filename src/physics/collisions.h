#ifndef _H_COLLISIONS_
#define _H_COLLISIONS_

void checkCollisionsBetweenEntities(struct Entity *entities, struct Entity *entity, int lastEntityIdx);

bool checkAABBCollision(struct Entity *e1, struct Entity *e2);

uint8_t checkTilesCollision(struct Entity *entities, uint8_t entityIdx);

#endif