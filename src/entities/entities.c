#include <stdint.h>
#include <string.h>

#include "physics/geom.h"
#include "render/video.h"
#include "animations.h"
#include "macros.h"

#include "physics/collisions.h"
#include "entities.h"

Entity entities[MAX_ENTITIES] = {0};
uint8_t lastEntityIdx = -1;

Entity* createEntity(int x, int y, uint8_t type, Sprite* spr, void (*update)(Entity* entity, Entity* player, uint8_t tileCollisions)) {
    // Check if there's not enough space for more entities
    // if (&entities[lastEntityIdx] == &entities[MAX_ENTITIES]) return NULL;
    if (lastEntityIdx + 1 == MAX_ENTITIES) return NULL;

    lastEntityIdx++;

    // Initialize entity
    entities[lastEntityIdx].x = x;
    entities[lastEntityIdx].y = y;
    entities[lastEntityIdx].hitbox = (Rect){x, y, spr->width, spr->height};
    entities[lastEntityIdx].sprite = spr;
    entities[lastEntityIdx].type = type;
    entities[lastEntityIdx].update = update;

    m_setFlag(entities[lastEntityIdx].flags, ENTITY_ALIVE);

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
    static uint32_t accumulatedTime = 0;

    // Handle animations with a global counter
    accumulatedTime += delta;
    char frameNeedsUpdate = false;

    if (accumulatedTime >= 160) {
        accumulatedTime -= 160;  // Reset accumulator
        frameNeedsUpdate = true;
    }

    uint8_t entityIdx = 0;

    while (entityIdx <= lastEntityIdx) {
        // TODO: Check collisions with player bullets
        Entity* entity = &entities[entityIdx];

        Rect entityRect = m_getEntityRect(entity);

        // TODO: Extract this to collisions module (checkCollisionsBetweenEntities) and check if it's actually working. For some reason entity is always the player
        if (m_isFlagSet(entity->flags, ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) {
            // reset check collision flag
            m_unsetFlag(entity->flags, ENTITY_CHECK_COLLISION);

            // iterate through all entities to check collisions
            for (uint8_t i = lastEntityIdx; i > 0; i--) {
                Entity* other = &entities[i];

                // prevent checks between entities of the same type, dead entities or entities without collision check flag
                if (entity->type == other->type || m_isFlagSet(entity->flags, ENTITY_INVULNERABLE) || m_isFlagSet(other->flags, ENTITY_STUNT) ||
                    !m_isFlagSet(other->flags, ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) continue;

                if ((entity->type == TYPE_PLAYER && other->type == TYPE_PLAYER_BULLET) || (entity->type == TYPE_PLAYER_BULLET && other->type == TYPE_PLAYER)) continue;

                if (checkAABBCollision(entityRect, m_getEntityRect(other))) {
                    if (entity->type == TYPE_PLAYER) m_setFlag(entity->flags, ENTITY_FLASHING);

                    if (entity->type == TYPE_PLAYER_BULLET && (other->type == TYPE_ENEMY_A || other->type == TYPE_ENEMY_B)) {
                        m_setFlag(other->flags, ENTITY_STUNT);
                        m_setFlag(entity->flags, ENTITY_FLASHING);
                    }
                }
            }
        }

        // We need to check collisions between entities and background tiles (walls, doors, interactive objects)
        // We need to use the tile collision to filter out entities that are close enough to use AABB collision checks between them
        uint8_t tileCollisionsBitmask = checkTilesCollision(entities, entityIdx);

        // Keep a reference to the current animation
        int currentAnimation = entity->animation;

        // update each entity logic
        entity->update(entity, &entities[0], tileCollisionsBitmask);

        // TODO: Move this inside update function for each entity (?)
        // Move entity using delta time for smooth animations
        if (!m_isFlagSet(entity->flags, ENTITY_BLOCKED)) {
            entity->x = entity->x + entity->vx * 70 * delta / 1000;  // Move 70 pixels/second
            // entity->y = entity->y + entity->vy * 70 * delta / 1000;  // Move 70 pixels/second
        }

        // Reset frame number if animation changed during update
        if (currentAnimation != entity->animation) {
            entity->frame = 0;
        }

        int frame = getAnimationFrame(frameNeedsUpdate, entity);
        int color = entity->flags & ENTITY_FLASHING ? COLOR_WHITE : COLOR_TRANSPARENT;
        drawSprite(entity->x, entity->y, entity->sprite, frame, entity->flags & ENTITY_FLIP, color);

#ifdef DEBUG_COLLISIONS
        drawBBoxColor((Rect){entity->x + entity->hitbox.x, entity->y + entity->hitbox.y, entity->hitbox.w, entity->hitbox.h}, 48);
#endif

        // TODO: Remove the flashing when collisions have consequences (kill player or enemy, etc)
        if (m_isFlagSet(entity->flags, ENTITY_FLASHING)) m_unsetFlag(entity->flags, ENTITY_FLASHING);

        // Delete entity if dead
        if (!m_isFlagSet(entity->flags, ENTITY_ALIVE)) destroyEntity(entityIdx);

        entityIdx++;
    }
}

void destroyEntity(uint8_t index) {
    if (index > lastEntityIdx) return;

    // Move destroyed entity to the last position
    entities[index] = entities[lastEntityIdx];

    // Clear the last entity slot
    memset(&entities[lastEntityIdx], 0, sizeof(Entity));

    // Update last entity index
    lastEntityIdx--;
}

void destroyAllEntities() {
    memset(entities, 0, sizeof(entities));
    lastEntityIdx = -1;
}

void markEntityForCollisionCheck(uint8_t index) {
    m_setFlag(entities[index].flags, ENTITY_CHECK_COLLISION);
}