#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "assets.h"
#include "entities.h"
#include "physics/collisions.h"
#include "animations.h"
#include "macros.h"

#include "whip.h"

Entity *whipSpawn(uint16_t x, uint16_t y, bool facingRight) {
    Entity *entity = createEntity(x, y, TYPE_PLAYER_BULLET, whipSprite, whipUpdate);
    if (!entity) return NULL;

    entity->collisionMask = TYPE_ENEMY_A | TYPE_ENEMY_B;
    entity->hitbox = (Rect){0, 6, whipSprite->width, 8};

    m_setAnimation(entity, ANIM_WHIP);
    m_setFlag(entity->flags, ENTITY_ALIVE);

    if (!facingRight) {
        m_setFlag(entity->flags, ENTITY_FLIP);
    }

    return entity;
}

void whipUpdate(Entity *entity, uint8_t tileCollisions) {
    if (entity->frame == ANIM_WHIP_LEN - 2 || m_isFlagSet(tileCollisions, TILE_TYPE_WALL_LEFT | TILE_TYPE_WALL_RIGHT)) {
        m_unsetFlag(entity->flags, ENTITY_ALIVE);
    }
}