#ifndef _MAP_FORMAT_H
#define _MAP_FORMAT_H

#define TILE_TYPE_BACKGROUND 0
#define TILE_TYPE_SOLID 1
#define TILE_TYPE_DOOR 2
#define TILE_TYPE_STAIRS 3

/**
 * Custom Tilemap format
 *
 * @param type Tile flags
 * @param id Tile ID
 */
typedef struct Tile {
    uint8_t type;
    uint8_t id;
} Tile;

typedef struct Map {
    uint8_t width;
    uint8_t height;
    uint8_t tileWidth;
    uint8_t tileHeight;
    Tile data[];
} Map;

#endif