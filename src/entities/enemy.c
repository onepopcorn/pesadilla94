#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "entities.h"
#include "assets.h"
#include "animations.h"
#include "macros.h"
#include "timer.h"

#include "enemy.h"

#ifdef DEBUG
#include "debug/logger.h"
#endif

#define ENEMY_SPEED 0.6
#define ENEMY_COLLISION_MASK TYPE_PLAYER | TYPE_PLAYER_BULLET

struct Entity *enemySpawn(short x, short y) {
    struct Entity *entity = createEntity(x, y, TYPE_ENEMY_A, enemySprite, enemyUpdate);
    if (!entity) return NULL;

    entity->collisionMask = ENEMY_COLLISION_MASK;

    entity->animation = ANIM_ENEMY1_WALK;
    entity->frame = 0;
    entity->vx = 1 * ENEMY_SPEED;
    entity->hitbox = (Rect){enemySprite->width / 4, 2, enemySprite->width / 2, enemySprite->height - 2};

    return entity;
}

// TODO: Use entity unique ID instead of position or pointer because we move entities around when an entity is destroyed
void enemyStun(uint8_t idx) {
    Entity *enemy = &entities[idx];

#ifdef DEBUG
    logDebug("* {idx:%d id:%d type:%d} Stunt enemy", idx, enemy->id, enemy->type);
#endif

    m_setFlag(enemy->flags, ENTITY_STUNT);
    enemy->animation = ANIM_ENEMY1_STUNT;
    // ALWAYS reset animation frame when updating animation or it could land out of boundaries
    // This caused a bug that took me 1 day to figure out...
    enemy->frame = 0;
    enemy->collisionMask = TYPE_NONE;
    enemy->vx = 0;

    setTimeout(enemyWakeup, enemy->id, 2000);
}

void enemyWakeup(uint8_t id) {
    struct Entity *enemy = findEntityById(id);

    if (enemy == NULL || id != enemy->id || !m_isFlagSet(enemy->flags, ENTITY_ALIVE)) {
#ifdef DEBUG
        logError("{idx:? id:%d} Failed waking up enemy", enemy->id);
#endif
        return;
    }

    m_unsetFlag(enemy->flags, ENTITY_STUNT);
    enemy->animation = ANIM_ENEMY1_WALK;
    enemy->frame = 0;
    enemy->vx = m_isFlagSet(enemy->flags, ENTITY_FLIP) ? ENEMY_SPEED * -1 : ENEMY_SPEED;
    enemy->collisionMask = ENEMY_COLLISION_MASK;

#ifdef DEBUG
    logDebug("^ {idx:(?) id:%d} Waken up enemy", enemy->id, enemy->type);
#endif
}

void enemyUpdate(struct Entity *entity, uint8_t tileCollisions) {
    // enemy stunt
    if (m_isFlagSet(entity->flags, ENTITY_STUNT)) {
        return;
    }

    // prevent movement on the first 2 frames
    if (entity->frame <= 2) {
        m_setFlag(entity->flags, ENTITY_BLOCKED);
    } else if ((entity->flags & ENTITY_BLOCKED)) {
        m_unsetFlag(entity->flags, ENTITY_BLOCKED);
    }

    if (m_isFlagSet(tileCollisions, TILE_TYPE_WALL_LEFT)) {
        // if ((tileCollisions & TILE_TYPE_WALL_LEFT) > 0) {
        entity->vx = 1 * ENEMY_SPEED;
    } else if (m_isFlagSet(tileCollisions, TILE_TYPE_WALL_RIGHT)) {
        // } else if ((tileCollisions & TILE_TYPE_WALL_RIGHT) > 0) {
        entity->vx = -1 * ENEMY_SPEED;
    }

    // Flip sprite when needed
    if (entity->vx > 0) {
        m_unsetFlag(entity->flags, ENTITY_FLIP);
    } else {
        m_setFlag(entity->flags, ENTITY_FLIP);
    }
}