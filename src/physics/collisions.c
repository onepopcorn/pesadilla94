#include <stdbool.h>
#include <stdio.h>

#include "io/resources.h"
#include "physics/geom.h"
#include "entities/entities.h"
#include "map.h"

#include "render/video.h"

#include "collisions.h"

/**
 *
 * TODO: Take a pointer to a function for when a collision happens
 */
void checkCollisionsBetweenEntities(Entity *entities, Entity *entity, int lastEntityIdx) {
    entity->flags &= ~ENTITY_CHECK_COLLISION;
    for (int i = lastEntityIdx; i > 0; i--) {
        Entity *other = &entities[i];

        if (entity->type == other->type ||
            (other->flags & (ENTITY_ALIVE | ENTITY_CHECK_COLLISION)) == 0) continue;

        if (!checkAABBCollision(m_getEntityRect(entity), m_getEntityRect(other))) continue;

        // what to do with the collision?
        // entity->flags |= ENTITY_FLASHING;
        other->flags |= ENTITY_FLASHING;
        // TODO check if that's actually what we want
        // other->flags &= (0xFF ^ ENTITY_ALIVE);
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
 * - Mark the entity to check for collisions with other entities when two entities are touching the same tile
 * - Return a bitmask with the type of tiles that the entity is touching
 *
 * Returns a collision bitmask to represent the tiles that are colliding with the entity
 * that can happen a the same time (e.g. floor, left wall, door, stairs...)
 */
uint8_t checkTilesCollision(Entity *entities, uint8_t entityIdx) {
    // Get current entity
    Entity *entity = &entities[entityIdx];
    Rect spriteRect = {entity->x, entity->y, entity->sprite->width, entity->sprite->height};
    Rect hitboxRect = m_getEntityRect(entity);

    Rect tilesRect = getTilesRect(spriteRect);
    int hTiles = tilesRect.h;
    int wTiles = tilesRect.w;
    int minTileColumn = tilesRect.x;
    int minTileRow = tilesRect.y;

    // Use this byte to return the type of walls that the entity is touching
    uint8_t tileCollisions = 0;

    for (int i = 0; i < hTiles; i++) {
        for (int j = 0; j < wTiles; j++) {
            int tileColumn = minTileColumn + j;
            int tileRow = minTileRow + i;

            int entityIdxInTile = markDirtyTile(entityIdx, tileColumn, tileRow);
            if (entityIdxInTile < 0) {
                // something wrong happend. dirtyTiles array is full
                // TODO: Log error to stdout
                break;
            }

            int tileType = getTile(tileColumn, tileRow)->type;

            // flip the bit that represents the tile type
            // TODO: Apply tile collision limits. Refactor this to make it more clean
            Rect tileHitbox = {0, 0, 0, 0};
            int tileXPos = tileColumn * TILE_SIZE;
            int tileYPos = tileRow * TILE_SIZE + SCREEN_Y_OFFSET;

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
            }

#ifdef DEBUG
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