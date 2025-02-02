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

Entity entities[MAX_ENTITIES] = {0};
int lastEntityIdx = -1;

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
    entities[lastEntityIdx].flags |= ENTITY_ALIVE;  // Type is stored in high nibble, flags in low nibble
    entities[lastEntityIdx].update = update;

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

    int entityIdx = lastEntityIdx;

    while (entityIdx >= 0) {
        // TODO: Check collisions with player bullets
        Entity* entity = &entities[entityIdx];

        Rect entityRect = m_getEntityRect(entity);

        // TODO: Extract this to collisions module (checkCollisionsBetweenEntities) and check if it's actually working. For some reason entity is always the player
        if (entity->flags & (ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) {
            // reset check collision flag
            entity->flags &= ~ENTITY_CHECK_COLLISION;
            // iterate through all entities to check collisions
            for (int i = lastEntityIdx; i > 0; i--) {
                Entity* other = &entities[i];

                // prevent checks between entities of the same type, dead entities or entities without collision check flag
                if (entity->type == other->type || (entity->flags & ENTITY_INVULNERABLE) ||
                    (other->flags & (ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) == 0) continue;

                if ((entity->type == TYPE_PLAYER && other->type == TYPE_PLAYER_BULLET) || (entity->type == TYPE_PLAYER_BULLET && other->type == TYPE_PLAYER)) continue;

                if (checkAABBCollision(entityRect, m_getEntityRect(other))) {
                    // what to do with the collision?
                    entity->flags |= ENTITY_FLASHING;
                    other->flags |= ENTITY_FLASHING;
                    // TODO check if that's actually what we want
                    // other->flags &= ~ENTITY_ALIVE;

                    if (entity->type == TYPE_PLAYER_BULLET && (other->type == TYPE_ENEMY_A || other->type == TYPE_ENEMY_B)) {
                        other->flags &= ~ENTITY_ALIVE;
                    }
                }
            }
        }

        // We need to check collisions between entities and background tiles (walls, doors, interactive objects)
        // We need to use the tile collision to filter out entities that are close enough to use AABB collision checks between them
        uint8_t tileCollisionsBitmask = checkTilesCollision(entities, entityIdx);

        //
        int currentAnimation = entity->animation;

        // update each entity logic
        entity->update(entity, &entities[0], tileCollisionsBitmask);

        // TODO: Move this inside update function for each entity (?)
        // Move entity using delta time for smooth animations
        if ((entity->flags & ENTITY_BLOCKED) == 0) {
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

#ifdef DEBUG
        drawBBoxColor((Rect){entity->x + entity->hitbox.x, entity->y + entity->hitbox.y, entity->hitbox.w, entity->hitbox.h}, 48);
#endif

        // Reset flashing flag
        entity->flags &= ~ENTITY_FLASHING;

        // Delete entity if dead
        if ((entity->flags & ENTITY_ALIVE) == 0) destroyEntity(entityIdx);

        entityIdx--;
    }
}

int getAnimationFrame(char nextFrame, Entity* entity) {
    int currentAnimation = entity->animation;

    // No need to update the frame, just return current frame
    if (!nextFrame) return Animations[currentAnimation + entity->frame];

    // Get next animation frame
    int animationFrame = Animations[++entity->frame + currentAnimation];

    // Reset frame when at the end of animation to loop back
    if (animationFrame == ANIM_LOOP) {
        entity->frame = 0;
        animationFrame = Animations[currentAnimation];
    }

    // Substract 1 frame to hold to the last animation frame
    if (animationFrame == ANIM_HOLD) {
        animationFrame = Animations[--entity->frame + currentAnimation];
    }

    // Use frame +1 to get the next animation
    if (animationFrame == ANIM_JMP) {
        int nextAnimation = Animations[entity->frame + 2 + currentAnimation];
        entity->frame = 0;
        entity->animation = nextAnimation;
        animationFrame = Animations[nextAnimation];
    }

    return animationFrame;
}

void destroyEntity(uint8_t index) {
    // Move destroyed entity to the last position
    Entity entity = entities[index];

    entities[index] = entities[lastEntityIdx];
    memset(&entity, 0, sizeof(Entity));
    entities[lastEntityIdx] = entity;

    // Update last entity inde
    lastEntityIdx--;
}

void markEntityForCollisionCheck(uint8_t index) {
    entities[index].flags |= ENTITY_CHECK_COLLISION;
}