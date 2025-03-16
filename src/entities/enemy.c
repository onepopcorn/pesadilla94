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

#define ENEMY_SPEED_R 0.6
#define ENEMY_SPEED_L -ENEMY_SPEED_R

#define ENEMY_COLLISION_MASK TYPE_PLAYER | TYPE_PLAYER_BULLET
#define ENEMY_STUNT_TIME 3000

// PRIVATE METHODS

void enemyWakeup(uint8_t id) {
    struct Entity *enemy = findEntityById(id);

    if (enemy == NULL || id != enemy->id || !m_isFlagSet(enemy->flags, ENTITY_ALIVE)) {
#ifdef DEBUG
        logError("{idx:? id:%d} Failed waking up enemy", enemy->id);
#endif
        return;
    }

    m_unsetFlag(enemy->flags, ENTITY_STUNT);
    m_setAnimation(enemy, ANIM_ENEMY1_WALK);
    enemy->vx = m_isFlagSet(enemy->flags, ENTITY_FLIP) ? ENEMY_SPEED_L : ENEMY_SPEED_R;
    enemy->collisionMask = ENEMY_COLLISION_MASK;

#ifdef DEBUG
    logDebug("^ {idx:(?) id:%d} Waken up enemy", enemy->id, enemy->type);
#endif
}

// PUBLIC METHODS

struct Entity *enemySpawn(short x, short y) {
    struct Entity *entity = createEntity(x, y, TYPE_ENEMY_A, enemySprite, enemyUpdate);
    if (!entity) return NULL;

    m_setAnimation(entity, ANIM_ENEMY1_WALK);
    entity->collisionMask = ENEMY_COLLISION_MASK;
    entity->vx = ENEMY_SPEED_R;
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
    m_setAnimation(enemy, ANIM_ENEMY1_STUNT);
    enemy->vx = 0;
    enemy->collisionMask = TYPE_NONE;

    setTimeout(enemyWakeup, enemy->id, ENEMY_STUNT_TIME);
}

// TODO: Make this cleaner
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
        entity->vx = ENEMY_SPEED_R;
        m_unsetFlag(entity->flags, ENTITY_FLIP);
    } else if (m_isFlagSet(tileCollisions, TILE_TYPE_WALL_RIGHT)) {
        entity->vx = ENEMY_SPEED_L;
        m_setFlag(entity->flags, ENTITY_FLIP);
    }
}