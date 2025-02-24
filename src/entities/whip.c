#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "assets.h"
#include "entities.h"
#include "physics/collisions.h"
#include "animations.h"
#include "macros.h"

#include "whip.h"

Entity *whipSpawn(int x, int y, bool facingRight) {
    Entity *entity = createEntity(x, y, TYPE_PLAYER_BULLET, whipSprite, whipUpdate);
    if (!entity) return NULL;

    entity->collisionMask = TYPE_NONE;

    entity->animation = ANIM_WHIP;
    entity->frame = 0;
    entity->hitbox = (Rect){4, 8, whipSprite->width / 2, 8};

    m_setFlag(entity->flags, ENTITY_ALIVE);

    if (!facingRight) {
        m_setFlag(entity->flags, ENTITY_FLIP);
    }

    return entity;
}

void whipUpdate(Entity *entity, uint8_t tileCollisions) {
    // Last frame makes hitbox bigger
    if (entity->frame == 2) {
        entity->collisionMask = TYPE_ENEMY_A | TYPE_ENEMY_B;
        int8_t dir = m_isFlagSet(entity->flags, ENTITY_FLIP) ? -1 : 1;
        entity->hitbox = (Rect){dir, 8, whipSprite->width - dir, 8};
    }

    if (entity->frame == ANIM_WHIP_LEN - 2 || m_isFlagSet(tileCollisions, TILE_TYPE_WALL_LEFT | TILE_TYPE_WALL_RIGHT)) {
        m_unsetFlag(entity->flags, ENTITY_ALIVE);
    }
}