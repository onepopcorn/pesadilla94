#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "settings/settings.h"
#include "io/resources.h"
#include "assets.h"
#include "physics/geom.h"
#include "render/video.h"

#include "map.h"

#define FLOOR_1_Y 62
#define FLOOR_2_Y 110
#define FLOOR_3_Y 158
#define FLOOR_PLAYER_Y_OFFSET 4

// Static data

// Map filenames
Map* currentMap;

// level file | time | enemy count | positions array index
const LevelData levels[NUM_LEVELS] = {
    {"level1.map", 15, 0, 0},
    {"level2.map", 24, 0, 1},
    {"level3.map", 40, 1, 2},
    {"level4.map", 60, 1, 4},
    {"level5.map", 80, 3, 6},
    {"level6.map", 90, 4, 10},
    {"level7.map", 60, 4, 15},
    {"level8.map", 100, 5, 20},
    {"level9.map", 90, 6, 26},
    {"level10.map", 60, 7, 33},
};

// Spawn positions. Player is always first
const Vec2 spawnPositions[] = {
    // Level 1 Enemies: 0
    {120, FLOOR_2_Y - FLOOR_PLAYER_Y_OFFSET},
    // Level 2 Enemies: 0
    {112, FLOOR_2_Y - FLOOR_PLAYER_Y_OFFSET},
    // Level 3 Enemies: 1
    {20, FLOOR_2_Y - FLOOR_PLAYER_Y_OFFSET},
    {100, FLOOR_2_Y},
    // Level 4 Enemies: 1
    {20, FLOOR_2_Y - FLOOR_PLAYER_Y_OFFSET},
    {100, FLOOR_3_Y},
    // Level 5 Enemies: 3
    {20, FLOOR_1_Y - FLOOR_PLAYER_Y_OFFSET},
    {280, FLOOR_1_Y},
    {150, FLOOR_2_Y},
    {100, FLOOR_3_Y},
    // Level 6 Enemies 4
    {20, FLOOR_1_Y - FLOOR_PLAYER_Y_OFFSET},
    {100, FLOOR_2_Y},
    {160, FLOOR_1_Y},
    {130, FLOOR_3_Y},
    {210, FLOOR_2_Y},
    // Level 7 Enemies 4
    {20, 154},
    {130, FLOOR_3_Y},
    {50, FLOOR_2_Y},
    {200, FLOOR_2_Y},
    {300, FLOOR_3_Y},
    // Level 8 Enemies 5
    {20, FLOOR_1_Y - FLOOR_PLAYER_Y_OFFSET},
    {70, FLOOR_1_Y},
    {300, FLOOR_1_Y},
    {45, FLOOR_2_Y},
    {200, FLOOR_2_Y},
    {10, FLOOR_3_Y},
    // Level 9 Enemies 6
    {100, FLOOR_1_Y - FLOOR_PLAYER_Y_OFFSET},
    {10, FLOOR_2_Y},
    {80, FLOOR_2_Y},
    {200, FLOOR_2_Y},
    {300, FLOOR_2_Y},
    {10, FLOOR_3_Y},
    {300, FLOOR_3_Y},
    // Level 10 Enemies 7
    {20, FLOOR_1_Y - FLOOR_PLAYER_Y_OFFSET},
    {20, FLOOR_2_Y},
    {90, FLOOR_2_Y},
    {200, FLOOR_1_Y},
    {180, FLOOR_2_Y},
    {290, FLOOR_3_Y},
    {190, FLOOR_3_Y},
    {300, FLOOR_2_Y},
    // END
};

// Approximate list of tiles to be redrawn using max entities number and max number of tiles a single entity can "touch"
#define MAX_DIRTY_TILES (MAX_ENTITIES * MAX_TILE_PER_ENTITY)

typedef struct DirtyTile {
    uint8_t entityIdx;
    uint8_t x, y;
} DirtyTile;

DirtyTile dirtyTiles[MAX_DIRTY_TILES] = {0};
uint8_t dirtyTilesCount = 0;

/**
 * Load map resources and set level
 *
 * @param level Level number to load the map from
 *
 */
uint8_t startLevel(uint8_t level) {
    if (level > NUM_LEVELS) {
        perror("Trying to load a non existing level");
        return EXIT_FAILURE;
    }

    currentMap = loadMap(levels[level].filename);
    if (!currentMap) {
        perror("Error failed to open map data file");
        return EXIT_FAILURE;
    }

    return currentMap->doorsCount;
}

/**
 * Free up current level
 *
 */
void endLevel() {
    // unload current map
    free(currentMap);
    currentMap = NULL;

    // reset dirty tiles
    dirtyTilesCount = 0;
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

    for (uint8_t i = 0; i < mapHeight; i++) {
        for (uint8_t j = 0; j < mapWidth; j++) {
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

    for (uint8_t i = 0; i < dirtyTilesCount; i++) {
        uint8_t x = dirtyTiles[i].x;
        uint8_t y = dirtyTiles[i].y;

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

    uint8_t minTileColumn = spriteRect.x / tileWidth;
    uint8_t minTileRow = (spriteRect.y - SCREEN_Y_OFFSET) / tileHeight;

    uint8_t maxTileColumn = (spriteRect.x + spriteRect.w) / tileWidth;
    uint8_t maxTileRow = ((spriteRect.y - SCREEN_Y_OFFSET) + spriteRect.h) / tileHeight;

    uint8_t wTiles = maxTileColumn - minTileColumn + 1;
    uint8_t hTiles = maxTileRow - minTileRow + 1;

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
Tile* getTile(uint8_t col, uint8_t row) {
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
Vec2 getStairsDestination(uint16_t x, uint16_t y, bool up) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    uint16_t destTileIdx = up ? tile->data.stairs.up : tile->data.stairs.down;
    uint16_t dx = destTileIdx % currentMap->width * TILE_SIZE;
    uint16_t dy = destTileIdx / currentMap->width * TILE_SIZE + SCREEN_Y_OFFSET;

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
Tile* openDoor(uint16_t x, uint16_t y) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    Tile* above = tile - currentMap->width;
    // TODO: Use macros for changing tiles
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
void closeDoor(uint16_t x, uint16_t y) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    Tile* above = tile - currentMap->width;
    if (tile->data.door.progress > 0) {
        above->id = TILE_DOOR_CLOSED_TOP;
        tile->id = TILE_DOOR_CLOSED_BOTTOM;
    }
}

void disableVendingMachine(uint16_t x, uint16_t y) {
    Tile* tile = getTile(x / currentMap->tileWidth, (y - SCREEN_Y_OFFSET) / currentMap->tileHeight);
    Tile* above = tile - currentMap->width;
    above->id = TILE_VENDING_DISABLED_TOP;
    tile->id = TILE_VENDING_DISABLED_BOTTOM;
}

/**
 * Marks tile at given coordinates as dirty
 *
 */
uint8_t markDirtyTile(uint8_t entityIdx, uint8_t tileColumn, uint8_t tileRow) {
    // dirty tiles buffer is full
    if (dirtyTilesCount >= MAX_DIRTY_TILES) {
        return -1;
    }

#ifdef DEBUG_TILES
    // NOTE This has a performance penalty
    uint8_t tileWidth = currentMap->tileWidth;
    uint8_t tileHeight = currentMap->tileHeight;
    drawRectColor((Rect){tileColumn * tileWidth, tileRow * tileHeight + SCREEN_Y_OFFSET, tileWidth, tileHeight}, 34);
#endif

    for (uint8_t k = 0; k < dirtyTilesCount; k++) {
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