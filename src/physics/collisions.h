#ifndef _H_COLLISIONS_
#define _H_COLLISIONS_

#include <stdbool.h>
#include "geom.h"

enum Collisions {
    COLLISION_WALL_L = 0x01,
    COLLISION_WALL_R = 0x02,
    COLLISION_DOOR = 0x04,
    COLLISION_STAIRS = 0x08,
    COLLISION_VENDING = 0x10,

};

void checkCollisionsBetweenEntities(uint8_t idx, uint8_t lastEntityIdx);

bool checkAABBCollision(Rect rect1, Rect rect2);

uint8_t checkTilesCollision(uint8_t entityIdx);

#endif  // _H_COLLISIONS_