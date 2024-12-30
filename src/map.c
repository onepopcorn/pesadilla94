#include <stdlib.h>
#include <stdint.h>

#include "settings/settings.h"
#include "io/resources.h"
#include "physics/geom.h"
#include "render/video.h"

#include "map.h"

#define SCREEN_Y_OFFSET 40

Map* currentMap;
Sprite* tileset;

// Approximate list of tiles to be redrawn using max entities number and max number of tiles a single entity can "touch"
#define MAX_DIRTY_TILES (MAX_ENTITIES * MAX_TILE_PER_ENTITY)

typedef struct DirtyTile {
    uint8_t entityIdx;
    int x, y;
} DirtyTile;

DirtyTile dirtyTiles[MAX_DIRTY_TILES] = {0};
int dirtyTilesCount = 0;

/**
 * Load map resources and set initial level
 *
 * @param level Level number to load the map from
 *
 */
void mapInit() {
    // load map sprite. Use a single tileset for all levels?
    tileset = loadSprite("tiles.spr");

    // load initial map data
    currentMap = loadMap("school1.map");
}

/**
 * Draw given map with offset coordinates with given tileset
 *
 * @param offset_x X coordinate to start drawing the tilemap
 * @param offset_y Y coordinate to start drawing the tilemap
 * @param tileset Pointer to spritesheet for the tileset
 *
 */
void drawMap() {
    Tile* data = currentMap->data;
    uint8_t mapWidth = currentMap->width;
    uint8_t mapHeight = currentMap->height;
    uint8_t tileWidth = currentMap->tileWidth;
    uint8_t tileHeight = currentMap->tileHeight;

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            Tile tile = data[j + i * mapWidth];
            // Tiled uses 1-based indexes for maps
            drawTile(j * tileWidth, i * tileHeight + SCREEN_Y_OFFSET, tileset, tile.id - 1);
        }
    }
}

/**
 * Repaint touched background tiles
 *
 */
void restoreMapTiles() {
    uint8_t mapWidth = currentMap->width;
    uint8_t tileWidth = currentMap->tileWidth;
    uint8_t tileHeight = currentMap->tileHeight;
    Tile* data = currentMap->data;

    for (int i = 0; i < dirtyTilesCount; i++) {
        int x = dirtyTiles[i].x;
        int y = dirtyTiles[i].y;

        Tile tile = data[x + y * mapWidth];
        drawTile(x * tileWidth, y * tileHeight + SCREEN_Y_OFFSET, tileset, tile.id - 1);
    }

    dirtyTilesCount = 0;
}

/**
 *
 * Get the tiles rectangle that a sprite is touching
 *
 * Using integers makes coordinates to round to nearest tile
 */
Rect getTilesRect(Rect spriteRect) {
    uint8_t tileWidth = currentMap->tileWidth;
    uint8_t tileHeight = currentMap->tileHeight;

    int minTileXIndex = spriteRect.x / tileWidth;
    int minTileYIndex = (spriteRect.y - SCREEN_Y_OFFSET) / tileHeight;

    int maxTileXIndex = (spriteRect.x + spriteRect.w) / tileWidth;
    int maxTileYIndex = ((spriteRect.y - SCREEN_Y_OFFSET) + spriteRect.h) / tileHeight;

    int wTiles = maxTileXIndex - minTileXIndex + 1;
    int hTiles = maxTileYIndex - minTileYIndex + 1;

    return (Rect){minTileXIndex, minTileYIndex, wTiles, hTiles};
}

int getTileType(int x, int y) {
    return currentMap->data[x + y * currentMap->width].type;
}

/**
 * Marks tile at given coordinates as dirty
 *
 */
int markDirtyTile(int entityIdx, int tileXIndex, int tileYIndex) {
    // dirty tiles buffer is full
    if (dirtyTilesCount >= MAX_DIRTY_TILES) {
        return -1;
    }

#ifdef DEBUG
    // NOTE This has a performance hit
    int tileWidth = currentMap->tileWidth;
    int tileHeight = currentMap->tileHeight;
    drawRectColor((Rect){tileXIndex * tileWidth, tileYIndex * tileHeight + SCREEN_Y_OFFSET, tileWidth, tileHeight}, 34);
#endif

    for (int k = 0; k < dirtyTilesCount; k++) {
        const DirtyTile dirtyTile = dirtyTiles[k];
        if (dirtyTile.x == tileXIndex && dirtyTile.y == tileYIndex) {
            // A previous entity already marked this tile return that one
            return dirtyTile.entityIdx;
        }
    }

    // No previous entity on this tile, mark it and return the entity index
    dirtyTiles[dirtyTilesCount++] = (DirtyTile){entityIdx, tileXIndex, tileYIndex};

    return entityIdx;
}

/**
 * Free loaded map resources
 *
 */
void mapFree() {
    free(currentMap);
    free(tileset);
}