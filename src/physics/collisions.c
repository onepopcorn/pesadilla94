#include <stdbool.h>
#include <stdio.h>

#include "io/resources.h"
#include "render/video.h"
#include "entities/entities.h"
#include "physics/geom.h"
#include "map.h"
#include "macros.h"
#include "entities/enemy.h"
#include "entities/player.h"
#include "hud/hud.h"

#ifdef DEBUG
#include "debug/logger.h"
#endif

#include "collisions.h"

/**
 *
 * TODO: Take a pointer to a function for when a collision happens
 */
void checkCollisionsBetweenEntities(uint8_t idx, uint8_t lastEntityIdx) {
    Entity *entity = &entities[idx];
    if (!m_isFlagSet(entity->flags, ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) return;

    Rect entityRect = m_getEntityRect(entity);
    m_unsetFlag(entity->flags, ENTITY_CHECK_COLLISION);

    for (uint8_t i = lastEntityIdx; i > 0; i--) {
        Entity *other = &entities[i];

        if (!(entity->collisionMask & other->type) || !(entity->type & other->collisionMask)) continue;

        // Check if they are colliding
        if (!checkAABBCollision(entityRect, m_getEntityRect(other))) continue;

        // Player collided with enemy
        if (entity->type == TYPE_PLAYER) {
            playerDie();
            continue;
        }

        // Whip collided with enemy
        if (other->type == TYPE_PLAYER_BULLET) {
#ifdef DEBUG
            logDebug("*? {idx:%d id:%d type:%d} Should Stunt enemy", idx, entity->id, entity->type);
#endif
            enemyStun(entity);
            updatePoints(POINTS_PER_STUNT);
        }
    }
}

/**
 * Simple AABB colllision checking
 *
 */

bool checkAABBCollision(Rect rect1, Rect rect2) {
    // rect1 is left of rect2 || rect1 is right of rect2 || rect1 is above rect2 || rect1 is below rect2
    if (rect1.x + rect1.w < rect2.x || rect1.x > rect2.x + rect2.w || rect1.y + rect1.h < rect2.y || rect1.y > rect2.y + rect2.h) {
        return false;
    }

    return true;
}

/**
 * Check collision between entities and tiles
 *
 * This function does three things:
 * - Mark the tiles that the entity is touching as dirty to be redrawn in the next frame
 * - Mark the entity to "check for collisions" with other entities when two entities are touching the same tile
 * - Return a bitmask with the type of tiles that the entity is touching for walls, doors & stairs
 *
 * Returns a collision bitmask to represent the tiles that are colliding with the entity
 * that can happen a the same time (e.g. floor, left wall, door, stairs...)
 */
uint8_t checkTilesCollision(uint8_t entityIdx) {
    // Get current entity
    Entity *entity = &entities[entityIdx];
    Rect spriteRect = {entity->x, entity->y, entity->sprite->width, entity->sprite->height};
    Rect hitboxRect = m_getEntityRect(entity);

    Rect tilesRect = getTilesRect(spriteRect);
    uint8_t hTiles = tilesRect.h;
    uint8_t wTiles = tilesRect.w;
    uint8_t minTileColumn = tilesRect.x;
    uint8_t minTileRow = tilesRect.y;

    // Use this byte to return the type of walls that the entity is touching
    uint8_t tileCollisions = 0;

    for (uint8_t i = 0; i < hTiles; i++) {
        for (uint8_t j = 0; j < wTiles; j++) {
            uint8_t tileColumn = minTileColumn + j;
            uint8_t tileRow = minTileRow + i;

            uint8_t entityIdxInTile = markDirtyTile(entityIdx, tileColumn, tileRow);
            if (entityIdxInTile < 0) {
                // something wrong happend. dirtyTiles array is full
                // TODO: Log error to stdout
                break;
            }

            uint8_t tileType = getTile(tileColumn, tileRow)->type;

            // flip the bit that represents the tile type
            // TODO: Apply tile collision limits. Refactor this to make it more clean
            uint16_t tileXPos = tileColumn * TILE_SIZE;
            uint16_t tileYPos = tileRow * TILE_SIZE + SCREEN_Y_OFFSET;
            Rect tileHitbox = {0, 0, 0, 0};

            switch (tileType) {
                case TILE_TYPE_WALL_LEFT:
                    tileHitbox = (Rect){tileXPos, tileYPos, 10, TILE_SIZE};
                    if (checkAABBCollision(hitboxRect, tileHitbox)) {
                        tileCollisions |= COLLISION_WALL_L;
                    }
                    break;
                case TILE_TYPE_WALL_RIGHT:
                    tileHitbox = (Rect){tileXPos + 8, tileYPos, 8, TILE_SIZE};
                    if (checkAABBCollision(hitboxRect, tileHitbox)) {
                        tileCollisions |= COLLISION_WALL_R;
                    }
                    break;
                case TILE_TYPE_DOOR:
                    tileHitbox = (Rect){tileXPos + 6, tileYPos + 10, 2, 2};
                    if (checkAABBCollision(hitboxRect, tileHitbox)) {
                        tileCollisions |= COLLISION_DOOR;
                    }
                    break;
                case TILE_TYPE_STAIRS:
                    tileHitbox = (Rect){tileXPos + 6, tileYPos + 10, 2, 2};
                    if (checkAABBCollision(hitboxRect, tileHitbox)) {
                        tileCollisions |= COLLISION_STAIRS;
                    }
                    break;
                case TILE_TYPE_VENDING:
                    tileHitbox = (Rect){tileXPos + 6, tileYPos + 10, 2, 2};
                    if (checkAABBCollision(hitboxRect, tileHitbox)) {
                        tileCollisions |= COLLISION_VENDING;
                    }
                    break;
            }

#ifdef DEBUG_COLLISIONS
            drawRectColor(tileHitbox, 3);
#endif

            // Mark both entities for collision checks
            if (entityIdxInTile != entityIdx) {
                entities[entityIdx].flags |= ENTITY_CHECK_COLLISION;
                entities[entityIdxInTile].flags |= ENTITY_CHECK_COLLISION;
            }
        }
    }

    return tileCollisions;
}