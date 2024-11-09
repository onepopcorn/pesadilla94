#include <stdint.h>
#include "io/resources.h"
#include "render/video.h"

#include "map.h"

Map *currentMap;
Sprite *tileset;
int mapXOffset = 0;
int mapYOffset = 0;

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

    // Set all tiles to dirty
    for (int i = 0; i < currentMap->width * currentMap->height; i++) {
        currentMap->data[i].flags |= TILE_STATE_DIRTY;
    }
}

/**
 * Draw given map with offset coordinates with given tileset
 *
 * @param offset_x X coordinate to start drawing the tilemap
 * @param offset_y Y coordinate to start drawing the tilemap
 * @param tileset Pointer to spritesheet for the tileset
 *
 * TODO: Test performance with nested loops vs divison and modulo operations
 * TODO: Try to use memcpy to copy blocks if finally using nested blocks
 */
void drawMap(int offset_x, int offset_y) {
    mapXOffset = offset_x;
    mapYOffset = offset_y;

    for (int i = 0; i < currentMap->height; i++) {
        for (int j = 0; j < currentMap->width; j++) {
            Tile tile = currentMap->data[j + i * currentMap->width];
            drawSprite(j * currentMap->tileWidth + mapXOffset, i * currentMap->tileHeight + mapYOffset, tileset, tile.id - 1, tile.flags & TILE_STATE_FLIP);
        }
    }
}

/**
 * Find tiles overlapping with given rectangle and restore them
 *
 */
void restoreBackground(Rect rect) {
    // get tile from coordinate
    int minTileXIndex = rect.x / currentMap->tileWidth;
    int minTileYIndex = (rect.y - mapYOffset) / currentMap->tileHeight;

    // Round up to nearest tile
    int maxTileXIndex = (rect.x + rect.w) / currentMap->tileWidth + 0.9;
    int maxTileYIndex = ((rect.y - mapYOffset) + rect.h) / currentMap->tileHeight + 0.9;

    int wTiles = maxTileXIndex - minTileXIndex + 1;
    int hTiles = maxTileYIndex - minTileYIndex + 1;

    for (int i = 0; i < hTiles; i++) {
        for (int j = 0; j < wTiles; j++) {
            int tileXIndex = minTileXIndex + j;
            int tileYIndex = minTileYIndex + i;
            // TODO: Limits should be handled by update entities function
            if (tileXIndex < 0 || tileXIndex >= currentMap->width || tileYIndex < 0 || tileYIndex >= currentMap->height) continue;

            Tile tile = currentMap->data[tileXIndex + tileYIndex * currentMap->width];
            drawSprite(tileXIndex * currentMap->tileWidth, tileYIndex * currentMap->tileHeight + mapYOffset, tileset, tile.id - 1, tile.flags & TILE_STATE_FLIP);
        }
    }
}

#ifdef DEBUG
void restoreBackgroundDebug(Rect rect) {
    int minTileXIndex = rect.x / currentMap->tileWidth;
    int minTileYIndex = (rect.y - mapYOffset) / currentMap->tileHeight;

    // Round up to nearest tile
    int maxTileXIndex = (rect.x + rect.w) / currentMap->tileWidth + 0.9;
    int maxTileYIndex = ((rect.y - mapYOffset) + rect.h) / currentMap->tileHeight + 0.9;

    int numXTiles = maxTileXIndex - minTileXIndex + 1;
    int numYTiles = maxTileYIndex - minTileYIndex + 1;

    for (int i = 0; i < numYTiles; i++) {
        for (int j = 0; j < numXTiles; j++) {
            int tileXIndex = minTileXIndex + j;
            int tileYIndex = minTileYIndex + i;
            // TODO: Limits shoudl be handled by update entities function
            if (tileXIndex < 0 || tileXIndex >= currentMap->width || tileYIndex < 0 || tileYIndex >= currentMap->height) continue;

            Rect rect = {tileXIndex * currentMap->tileWidth, tileYIndex * currentMap->tileHeight + mapYOffset, currentMap->tileWidth, currentMap->tileHeight};
            drawRectColor(rect, 34);
        }
    }
}
#endif