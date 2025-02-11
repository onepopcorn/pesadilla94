#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "settings/settings.h"
#include "io/resources.h"
#include "assets.h"
#include "physics/geom.h"
#include "render/video.h"
#include "render/hud.h"

#include "map.h"

Map* currentMap;
uint8_t currentLevel = 0;
const char* maps[NUM_LEVELS] = {
    "school1.map",
    "school2.map",
    "schoole3.map",
};

// Approximate list of tiles to be redrawn using max entities number and max number of tiles a single entity can "touch"
#define MAX_DIRTY_TILES (MAX_ENTITIES * MAX_TILE_PER_ENTITY)

typedef struct DirtyTile {
    uint8_t entityIdx;
    int x, y;
} DirtyTile;

DirtyTile dirtyTiles[MAX_DIRTY_TILES] = {0};
int dirtyTilesCount = 0;

/**
 * Load map resources and set level
 *
 * @param level Level number to load the map from
 *
 */
uint8_t startLevel(uint8_t level) {
    if (level > NUM_LEVELS - 1) {
        perror("Trying to load a non existing level");
        return EXIT_FAILURE;
    }

    currentMap = loadMap(maps[level]);
    if (!currentMap) {
        perror("Error failed to open map data file");
        return EXIT_FAILURE;
    }

    currentLevel = level;
    return EXIT_SUCCESS;
}

void endLevel() {
    free(currentMap);
    currentMap = NULL;
}

/**
 * Draw current map
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

    int minTileColumn = spriteRect.x / tileWidth;
    int minTileRow = (spriteRect.y - SCREEN_Y_OFFSET) / tileHeight;

    int maxTileColumn = (spriteRect.x + spriteRect.w) / tileWidth;
    int maxTileRow = ((spriteRect.y - SCREEN_Y_OFFSET) + spriteRect.h) / tileHeight;

    int wTiles = maxTileColumn - minTileColumn + 1;
    int hTiles = maxTileRow - minTileRow + 1;

    return (Rect){minTileColumn, minTileRow, wTiles, hTiles};
}

/**
 * Get tile data at given coordinates
 *
 * @param col tile column
 * @param row tile row
 *
 * @return Tile* pointer of the tile in given coordinates
 *
 */
Tile* getTile(int col, int row) {
    return &currentMap->data[col + row * currentMap->width];
}

/**
 * Get x/y coordinates of the stair destination
 *
 * @param x sprite x coordinate
 * @param y sprite y coordinate
 *
 * @return Vec2 tile coordinates destination to move the sprite to
 */
Vec2 getStairsDestination(float x, float y, bool up) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    int destTileIdx = up ? tile->data.stairs.up : tile->data.stairs.down;
    int dx = destTileIdx % currentMap->width * TILE_SIZE;
    int dy = destTileIdx / currentMap->width * TILE_SIZE + SCREEN_Y_OFFSET;

    return (Vec2){dx, dy};
}

/**
 * Update the backgroud tiles to show the door open for given coordinates
 *
 * @param x sprite x coordinate
 * @param y sprite y coordinate
 *
 * @return Tile* pointer to background tile on given coordinates
 */
Tile* openDoor(float x, float y) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    Tile* above = tile - currentMap->width;
    // TODO: Use defines for changing tiles
    if (tile->data.door.progress > 0) {
        above->id = TILE_DOOR_OPEN_TOP;
        tile->id = TILE_DOOR_OPEN_BOTTOM;
    }
    return tile;
}

/**
 * Update background tiles to show the door closed for given coordinates
 *
 * @param x sprite x coordinate
 * @param y sprite y coordinate
 */
void closeDoor(float x, float y) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    Tile* above = tile - currentMap->width;
    // TODO: Use defines for changin tiles
    if (tile->data.door.progress > 0) {
        above->id = TILE_DOOR_CLOSED_TOP;
        tile->id = TILE_DOOR_CLOSED_BOTTOM;
    }
}

/**
 * Marks tile at given coordinates as dirty
 *
 */
int markDirtyTile(int entityIdx, int tileColumn, int tileRow) {
    // dirty tiles buffer is full
    if (dirtyTilesCount >= MAX_DIRTY_TILES) {
        return -1;
    }

#ifdef DEBUG_TILES
    // NOTE This has a performance hit
    int tileWidth = currentMap->tileWidth;
    int tileHeight = currentMap->tileHeight;
    drawRectColor((Rect){tileColumn * tileWidth, tileRow * tileHeight + SCREEN_Y_OFFSET, tileWidth, tileHeight}, 34);
#endif

    for (int k = 0; k < dirtyTilesCount; k++) {
        const DirtyTile dirtyTile = dirtyTiles[k];
        if (dirtyTile.x == tileColumn && dirtyTile.y == tileRow) {
            // A previous entity already marked this tile return that one
            return dirtyTile.entityIdx;
        }
    }

    // No previous entity on this tile, mark it and return the entity index
    dirtyTiles[dirtyTilesCount++] = (DirtyTile){entityIdx, tileColumn, tileRow};

    return entityIdx;
}