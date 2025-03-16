#include <stdint.h>
#include <string.h>

#include "physics/geom.h"
#include "render/video.h"
#include "animations.h"
#include "macros.h"
#include "enemy.h"

#include "physics/collisions.h"
#include "entities.h"

#ifdef DEBUG
#include "debug/logger.h"
#endif

Entity entities[MAX_ENTITIES] = {0};
int8_t lastEntityIdx = -1;
uint8_t nextId = 0;

Entity* createEntity(uint16_t x, uint16_t y, uint8_t type, Sprite* spr, void (*update)(Entity* entity, uint8_t tileCollisions)) {
    // Check if there's not enough space for more entities
    if (lastEntityIdx + 1 == MAX_ENTITIES) {
#ifdef DEBUG
        logError("Trying to create entity passed MAX_ENTITIES %d", lastEntityIdx);
#endif
        return NULL;
    };

    lastEntityIdx++;
    nextId--;

    // TODO: Remove this and set props on each spawn function
    // Initialize entity
    entities[lastEntityIdx].id = nextId;
    entities[lastEntityIdx].x = x;
    entities[lastEntityIdx].y = y;
    entities[lastEntityIdx].hitbox = (Rect){x, y, spr->width, spr->height};
    entities[lastEntityIdx].sprite = spr;
    entities[lastEntityIdx].type = type;
    entities[lastEntityIdx].update = update;

    m_setFlag(entities[lastEntityIdx].flags, ENTITY_ALIVE);

#ifdef DEBUG
    logDebug("○ {idx:%d id:%d type:%d} Entity created", lastEntityIdx, nextId, type);
#endif

    Entity* entity = &entities[lastEntityIdx];
    return entity;
}

/**
 * Update all entities
 *
 * Should:
 * - Check input for player entity
 * - Update other entities based on behavior/state/logic
 * - Check physics and boundaries
 * - Check collisions
 * - Render entities to avoid looping through all entities twice??? Not sure about this one
 *   - having a separete render loop allows to render them backwards
 *   - render them here avoids looping through all entities twice
 *
 */
void updateEntities(uint32_t delta) {
    // Skip the whole update if there are no entities
    if (lastEntityIdx < 0) return;

    static uint32_t accumulatedTime = 0;

    // Handle animations with a global counter
    accumulatedTime += delta;
    char frameNeedsUpdate = false;

    if (accumulatedTime >= 160) {
        accumulatedTime -= 160;  // Reset accumulator
        frameNeedsUpdate = true;
    }

    // uint8_t entityIdx = lastEntityIdx;
    uint8_t entityIdx = 0;

    do {
        Entity* entity = &entities[entityIdx];

        if (!m_isFlagSet(entity->flags, ENTITY_ALIVE)) continue;

        // We need to check collisions between entities and background tiles (walls, doors, interactive objects)
        // We need to use the tile collision to filter out entities that are close enough to use AABB collision checks between them
        uint8_t tileCollisionsBitmask = checkTilesCollision(entityIdx);

        checkCollisionsBetweenEntities(entityIdx, lastEntityIdx);

        // Keep a reference to the current animation
        uint8_t currentAnimation = entity->animation;

        // update each entity logic
        entity->update(entity, tileCollisionsBitmask);

        // TODO: Move this inside update function for each entity (?)
        // Move entity using delta time for smooth animations
        if (!m_isFlagSet(entity->flags, ENTITY_BLOCKED)) {
            entity->x = entity->x + entity->vx * 70 * delta / 1000;  // Move 70 pixels/second
        }

        // Reset frame number if animation changed during update
        if (currentAnimation != entity->animation) {
            entity->frame = 0;
        }

        renderEntity(entityIdx, frameNeedsUpdate);

        // TODO: Remove the flashing when collisions have consequences (kill player or enemy, etc)
        // if (m_isFlagSet(entity->flags, ENTITY_FLASHING)) m_unsetFlag(entity->flags, ENTITY_FLASHING);

        // Delete entity if dead
        if (!m_isFlagSet(entity->flags, ENTITY_ALIVE)) destroyEntity(entityIdx);

    } while (entityIdx++ < lastEntityIdx);
}

void renderEntity(uint8_t index, char frameNeedsUpdate) {
    Entity* entity = &entities[index];
    uint16_t frame = getAnimationFrame(frameNeedsUpdate, entity);
    uint8_t color = m_isFlagSet(entity->flags, ENTITY_FLASHING) && (entity->frame & 0x01) ? COLOR_WHITE : COLOR_TRANSPARENT;
    drawSprite(entity->x, entity->y, entity->sprite, frame, entity->flags & ENTITY_FLIP, color);

#ifdef DEBUG_ENTITIES
    showEntityId(entity->x, entity->y, index);
#endif

#ifdef DEBUG_COLLISIONS
    drawBBoxColor((Rect){entity->x + entity->hitbox.x, entity->y + entity->hitbox.y, entity->hitbox.w, entity->hitbox.h}, 48);
#endif
}

Entity* findEntityById(uint8_t id) {
    for (uint8_t i = lastEntityIdx; i >= 0; i--) {
        Entity* entity = &entities[i];
        if (entity->id == id) {
#ifdef DEBUG
            logDebug("Found id:%d at i:%d", id, i);
#endif
            return entity;
        }
    }

#ifdef DEBUG
    logError("Entity with id:%d not found", id);
#endif
    return NULL;
}

void destroyEntity(uint8_t idx) {
    // Trying to destroy an entity which is out of boundaries
    if (idx > lastEntityIdx) {
#ifdef DEBUG
        logError("Trying to destroy out of bounds entity %d", lastEntityIdx);
#endif
        return;
    }

#ifdef DEBUG
    logDebug("■ {idx:%d id:%d type:%d} Entity destroyed", lastEntityIdx, entities[idx].id, entities[idx].type);
#endif

    // Copy last position entity to where the destroyed entity is
    if (idx != lastEntityIdx) {
#ifdef DEBUG
        Entity* last = &entities[lastEntityIdx];
        Entity* e = &entities[idx];
        logDebug("↔ Swapped entity {idx:%d id:%d type:%d} with entity {idx:%d id:%d type:%d}", idx, e->id, e->type, lastEntityIdx, last->id, last->type);
#endif
        entities[idx] = entities[lastEntityIdx];
    }

    // Clear the last entity slot
    memset(&entities[lastEntityIdx], 0, sizeof(Entity));

    // Update last entity idx
    lastEntityIdx--;
}

void destroyAllEntities() {
    memset(entities, 0, sizeof(entities));
    lastEntityIdx = -1;
    nextId = 0;
}

void markEntityForCollisionCheck(uint8_t index) {
    m_setFlag(entities[index].flags, ENTITY_CHECK_COLLISION);
}