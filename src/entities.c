#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "physics/geom.h"
#include "render/video.h"
#include "animations.h"

#include "entities.h"
#include "physics/collisions.h"

struct Entity entities[MAX_ENTITIES] = {0};
int lastEntityIdx = -1;

struct Entity* createEntity(int x, int y, uint8_t type, Sprite* spr, void (*update)(struct Entity* entity, struct Entity* player, uint8_t tileCollisions)) {
    // Check if there's not enough space for more entities
    // if (&entities[lastEntityIdx] == &entities[MAX_ENTITIES]) return NULL;
    if (lastEntityIdx + 1 == MAX_ENTITIES) return NULL;

    lastEntityIdx++;

    // Initialize entity
    entities[lastEntityIdx].x = x;
    entities[lastEntityIdx].y = y;
    entities[lastEntityIdx].sprite = spr;
    entities[lastEntityIdx].type = type;
    entities[lastEntityIdx].flags |= ENTITY_ALIVE;  // Type is stored in high nibble, flags in low nibble
    entities[lastEntityIdx].update = update;

    struct Entity* entity = &entities[lastEntityIdx];
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
// static uint32_t accumulated_time = 0;
void updateEntities(uint32_t delta) {
    // Player is always entity 0
    struct Entity* entity = &entities[lastEntityIdx];

    static uint32_t accumulatedTime = 0;

    // Update player logic

    // Handle animations with a global counter
    accumulatedTime += delta;
    char frameNeedsUpdate = false;

    if (accumulatedTime >= 150) {
        accumulatedTime -= 150;  // Reset accumulator
        frameNeedsUpdate = true;
    }

    int entityIdx = lastEntityIdx;

    while (entity >= &entities[0]) {
        // TODO: Check collisions with player
        // TODO: Check collisions with player bullets
        // TODO: Check collision with grid tiles (e.g. background, walls, doors, stairs...)

        // FIRST APPROACH CHECKS COLLISIONS WITH OTHER ENTITIES NO MATTER THE TYPE
        // TODO: Extract this to collisions module and check if it's actually working. For some reason entity is always the player

        if (entity->flags & (ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) {
            entity->flags &= ~ENTITY_CHECK_COLLISION;
            // iterate through all entities to check collisions
            for (int i = lastEntityIdx; i > 0; i--) {
                struct Entity* other = &entities[i];

                // prevent checks between entities of the same type, dead entities or entities without collision check flag
                if (entity->type == other->type ||
                    (other->flags & (ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) == 0) continue;

                if (checkAABBCollision(entity, other)) {
                    // reset collision check flags
                    // entity->flags &= ~ENTITY_CHECK_COLLISION;
                    // other->flags &= ~ENTITY_CHECK_COLLISION;

                    // what to do with the collision?
                    entity->flags |= ENTITY_FLASHING;
                    other->flags |= ENTITY_FLASHING;
                    // TODO check if that's actually what we want
                    // other->flags &= (0xFF ^ ENTITY_ALIVE);
                }
            }
        }

        // We need to check collisions between entities and background tiles (walls, doors, interactive objects)
        // We need to use the tile collision to filter out entities that are close enough to use AABB collision checks between them
        uint8_t tileCollisionsBitmask = checkTilesCollision(entities, entityIdx);

        // update each entity logic
        entity->update(entity, &entities[0], tileCollisionsBitmask);

        // TODO: Move this inside update function or each entity
        // Move entity using delta time for smooth animations
        entity->x = entity->x + entity->vx * 70 * delta / 1000;  // Move 70 pixels/second
        entity->y = entity->y + entity->vy * 70 * delta / 1000;  // Move 70 pixels/second

        int frame = getAnimationFrame(frameNeedsUpdate, entity);
        int color = entity->flags & ENTITY_FLASHING ? COLOR_WHITE : COLOR_TRANSPARENT;
        drawSprite(entity->x, entity->y, entity->sprite, frame, entity->flags & ENTITY_FLIP, color);

        // Reset flashing flag
        entity->flags &= ~ENTITY_FLASHING;

        // Delete entity if dead
        if ((entity->flags & ENTITY_ALIVE) == 0) destroyEntity(entityIdx);

        entity--;
        entityIdx--;
    }
}

int getAnimationFrame(char nextFrame, struct Entity* entity) {
    int animationFrame = Animations[entity->frame];
    if (!nextFrame) return animationFrame;

    entity->frame++;
    animationFrame = Animations[entity->frame];

    if (animationFrame < 0) {
        entity->frame = entity->animation;
        animationFrame = Animations[entity->frame];
    }

    return animationFrame;
}

void destroyEntity(uint8_t index) {
    // Move destroyed entity to the last position
    struct Entity entity = entities[index];

    entities[index] = entities[lastEntityIdx];
    memset(&entity, 0, sizeof(Entitiy));
    entities[lastEntityIdx] = entity;

    // Update last entity inde
    lastEntityIdx--;
}

void markEntityForCollisionCheck(uint8_t index) {
    entities[index].flags |= ENTITY_CHECK_COLLISION;
}