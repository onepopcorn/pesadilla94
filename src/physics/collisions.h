#ifndef _H_COLLISIONS_
#define _H_COLLISIONS_

void checkCollisionsBetweenEntities(Entity *entities, Entity *entity, int lastEntityIdx);

bool checkAABBCollision(Rect rect1, Rect rect2);

uint8_t checkTilesCollision(Entity *entities, uint8_t entityIdx);

#endif  // _H_COLLISIONS_