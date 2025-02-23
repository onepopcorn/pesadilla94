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

    entity->collisionMask = TYPE_ENEMY_A | TYPE_ENEMY_B;

    entity->animation = ANIM_WHIP;
    entity->frame = 0;
    entity->hitbox = (Rect){1, 1, 1, 1};

    m_setFlag(entity->flags, ENTITY_ALIVE);

    if (!facingRight) {
        m_setFlag(entity->flags, ENTITY_FLIP);
    }

    return entity;
}

void whipUpdate(Entity *entity, uint8_t tileCollisions) {
    // Last frame makes hitbox bigger
    if (entity->frame == 2) {
        int8_t dir = m_isFlagSet(entity->flags, ENTITY_FLIP) ? -1 : 1;
        entity->hitbox = (Rect){2 * dir, 8, whipSprite->width, 8};
    }

    if (entity->frame == ANIM_WHIP_LEN - 2) {
        m_unsetFlag(entity->flags, ENTITY_ALIVE);
    }
}