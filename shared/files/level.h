#ifndef _MAP_FORMAT_H
#define _MAP_FORMAT_H

enum TileType {
    TILE_TYPE_BACKGROUND = 0,
    TILE_TYPE_WALL_LEFT = 1,
    TILE_TYPE_WALL_RIGHT = 2,
    TILE_TYPE_DOOR = 3,
    TILE_TYPE_STAIRS = 4,
    TILE_TYPE_VENDING = 5,
};

/**
 * Custom Tilemap format
 *
 * @param type Tile flags
 * @param id Tile ID
 * @param data Tile data
 */
typedef struct Tile {
    uint8_t type;
    uint8_t id;
    union {
        struct {
            uint8_t up;
            uint8_t down;
        } stairs;
        struct {
            uint8_t progress;
        } door;
    } data;
} Tile;

typedef struct Map {
    uint8_t width;
    uint8_t height;
    uint8_t tileWidth;
    uint8_t tileHeight;
    uint8_t doorsCount;
    Tile data[];
} Map;

/**
 * To be used only for parsing stairs layer data, not in the game
 *
 */
typedef struct StairsData {
    uint8_t id;
    uint8_t tileIdx;
    uint8_t up;
    uint8_t down;
} StairsData;

typedef struct DoorData {
    uint8_t id;
    uint8_t progress;
    uint8_t item;
} DoorData;

typedef struct LevelData {
    char *filename;
    uint8_t time;
    uint8_t enemyCount;
    uint8_t positionsIdx;
} LevelData;

#endif