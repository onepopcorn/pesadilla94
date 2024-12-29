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

        if (!checkAABBCollision(entity, other)) continue;

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
 * TODO: Set sprite collision boxes
 *
 */

bool checkAABBCollision(Entity *e1, Entity *e2) {
    // Apply hitbox to entity position
    Rect e1Rect = {e1->x + e1->hitbox.x, e1->y + e1->hitbox.y, e1->hitbox.w, e1->hitbox.h};
    Rect e2Rect = {e2->x + e2->hitbox.x, e2->y + e2->hitbox.y, e2->hitbox.w, e2->hitbox.h};

    // e1 is left of e2 || e1 is right of e2 || e1 is above e2 || e1 is below e2
    if (e1Rect.x + e1Rect.w < e2Rect.x || e1Rect.x > e2Rect.x + e2Rect.w || e1Rect.y + e1Rect.h < e2Rect.y || e1Rect.y > e2Rect.y + e2Rect.h) {
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