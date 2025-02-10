#ifndef _H_COLLISIONS_
#define _H_COLLISIONS_

#include <stdbool.h>
#include "geom.h"

struct Entity;

#define COLLISION_WALL_L 0x01
#define COLLISION_WALL_R 0x02
#define COLLISION_DOOR 0x04
#define COLLISION_STAIRS 0x08

void checkCollisionsBetweenEntities(struct Entity *entities, struct Entity *entity, int lastEntityIdx);

bool checkAABBCollision(Rect rect1, Rect rect2);

uint8_t checkTilesCollision(struct Entity *entities, uint8_t entityIdx);

#endif  // _H_COLLISIONS_