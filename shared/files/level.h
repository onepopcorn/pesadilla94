#ifndef _MAP_FORMAT_H
#define _MAP_FORMAT_H

#define TILE_TYPE_BACKGROUND 0
#define TILE_TYPE_WALL_LEFT 1
#define TILE_TYPE_WALL_RIGHT 2
#define TILE_TYPE_DOOR 3
#define TILE_TYPE_STAIRS 4

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
            uint8_t item;
        } door;
    } data;
} Tile;

typedef struct Map {
    uint8_t width;
    uint8_t height;
    uint8_t tileWidth;
    uint8_t tileHeight;
    Tile data[];
} Map;

/**
 * To be used only for parsing stairs layer data, not in the game
 *
 */
typedef struct StairsData {
    uint8_t id;
    uint8_t up;
    uint8_t down;
} StairsData;

typedef struct DoorData {
    uint8_t id;
    uint8_t progress;
    uint8_t item;
} DoorData;

#endif