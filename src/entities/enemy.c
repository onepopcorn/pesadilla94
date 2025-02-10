#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "entities.h"
#include "assets.h"
#include "animations.h"
#include "macros.h"

#include "enemy.h"

#define ENEMY_SPEED 0.6

Entity *enemySpawn(int x, int y) {
    struct Entity *entity = createEntity(x, y, TYPE_ENEMY_A, enemySprite, enemyUpdate);
    if (!entity) return NULL;

    entity->animation = ANIM_ENEMY1_WALK;
    entity->frame = 0;
    entity->vx = 1 * ENEMY_SPEED;
    entity->hitbox = (Rect){enemySprite->width / 4, 2, enemySprite->width / 2, enemySprite->height - 2};

    return entity;
}

void enemyUpdate(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    // enemy stunt
    if (m_isFlagSet(entity->flags, ENTITY_STUNT)) {
        entity->animation = ANIM_ENEMY1_STUNT;
        entity->vx = 0;
        return;
    }

    // prevent movement on the first 2 frames
    if (entity->frame <= 2) {
        m_setFlag(entity->flags, ENTITY_BLOCKED);
    } else if ((entity->flags & ENTITY_BLOCKED)) {
        m_unsetFlag(entity->flags, ENTITY_BLOCKED);
    }

    if ((tileCollisions & TILE_TYPE_WALL_LEFT) > 0) {
        entity->vx = 1 * ENEMY_SPEED;
    } else if ((tileCollisions & TILE_TYPE_WALL_RIGHT) > 0) {
        entity->vx = -1 * ENEMY_SPEED;
    }

    // Flip sprite when needed
    if (entity->vx > 0) {
        m_unsetFlag(entity->flags, ENTITY_FLIP);
    } else {
        m_setFlag(entity->flags, ENTITY_FLIP);
    }
}