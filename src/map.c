#include <stdlib.h>
#include <stdint.h>

#include "settings/settings.h"
#include "io/resources.h"
#include "render/video.h"

#include "map.h"

#define MAP_Y_OFFSET 40

Map* currentMap;
Sprite* tileset;

// Approximate list of tiles to be redrawn using max entities number and max number of tiles a single entity can "touch"
// TODO: Find a safer way of handling this
#define MAX_DIRTY_TILES MAX_ENTITIES* MAX_TILE_PER_ENTITY

typedef struct DirtyTile {
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
    tileset = loadSprite("tileset.spr");

    // load initial map data
    currentMap = loadMap("level1.map");
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
            drawTile(j * tileWidth, i * tileHeight + MAP_Y_OFFSET, tileset, tile.id - 1);
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
        drawTile(x * tileWidth, y * tileHeight + MAP_Y_OFFSET, tileset, tile.id - 1);
    }

    dirtyTilesCount = 0;
}

/**
 * Find tiles overlapping with given rectangle and restore them
 * TODO: Add tiles to dirty tiles array instead of rendering this here
 */
void markTouchedTiles(Rect rect) {
    uint8_t tileWidth = currentMap->tileWidth;
    uint8_t tileHeight = currentMap->tileHeight;

    // get tile from coordinate
    int minTileXIndex = rect.x / tileWidth;
    int minTileYIndex = (rect.y - MAP_Y_OFFSET) / tileHeight;

    // Round up to nearest tile
    int maxTileXIndex = (rect.x + rect.w) / tileWidth + 0.9;
    int maxTileYIndex = ((rect.y - MAP_Y_OFFSET) + rect.h) / tileHeight + 0.9;

    int wTiles = maxTileXIndex - minTileXIndex + 1;
    int hTiles = maxTileYIndex - minTileYIndex + 1;

    for (int i = 0; i < hTiles; i++) {
        for (int j = 0; j < wTiles; j++) {
            int tileXIndex = minTileXIndex + j;
            int tileYIndex = minTileYIndex + i;
            // TODO: Limits should be handled by update entities function
            // if (tileXIndex < 0 || tileXIndex >= currentMap->width || tileYIndex < 0 || tileYIndex >= currentMap->height) continue;

            // Mark tile to be redrawn
            // TODO: prevent duplicate tiles
            if (dirtyTilesCount < MAX_DIRTY_TILES) {
                dirtyTiles[dirtyTilesCount++] = (DirtyTile){tileXIndex, tileYIndex};
            }
#ifdef DEBUG
            // NOTE This has a performance hit
            drawRectColor((Rect){tileXIndex * tileWidth, tileYIndex * tileHeight + MAP_Y_OFFSET, tileWidth, tileHeight}, 34);
#endif
        }
    }
}

/**
 * Free loaded map resources
 *
 */
void mapFree() {
    free(currentMap);
    free(tileset);
}