#include <stdbool.h>
#include <stdio.h>

#include "io/resources.h"
#include "physics/geom.h"
#include "entities.h"
#include "map.h"

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

        // reset collision check flags
        // TODO: This shouldn't be done here.
        // entity->flags &= ~ENTITY_CHECK_COLLISION;
        // other->flags &= ~ENTITY_CHECK_COLLISION;

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

    // TODO: get the tiles for the entity hitbox and apply tile collision limits
    Rect tilesRect = getTilesRect(spriteRect);
    int hTiles = tilesRect.h;
    int wTiles = tilesRect.w;
    int minTileXIndex = tilesRect.x;
    int minTileYIndex = tilesRect.y;

    // Use this byte to return the type of walls that the entity is touching
    uint8_t tileCollisions = 0;

    for (int i = 0; i < hTiles; i++) {
        for (int j = 0; j < wTiles; j++) {
            int tileXIndex = minTileXIndex + j;
            int tileYIndex = minTileYIndex + i;

            int entityIdxInTile = markDirtyTile(entityIdx, tileXIndex, tileYIndex);
            if (entityIdxInTile < 0) {
                // something wrong happend. dirtyTiles array is full
                // TODO: Log error to stdout
                break;
            }

            int tileType = getTileType(tileXIndex, tileYIndex);

            // flip the bit that represents the tile type
            // tileCollisions |= (1 << (tileType - 1));
            // TODO: Apply tile collision limits
            if (tileType == TILE_WALL_L) {
                tileCollisions |= 0x01;
            } else if (tileType == TILE_WALL_R) {
                tileCollisions |= 0x02;
            } else if (tileType == TILE_DOOR) {
                tileCollisions |= 0x04;
            } else if (tileType == TILE_STAIRS) {
                tileCollisions |= 0x08;
            } else if (tileType == TILE_FLOOR) {
                tileCollisions |= 0x10;
            }

            // Mark both entities for collision checks
            if (entityIdxInTile != entityIdx) {
                entities[entityIdx].flags |= ENTITY_CHECK_COLLISION;
                entities[entityIdxInTile].flags |= ENTITY_CHECK_COLLISION;
            }
        }
    }

    return tileCollisions;
}