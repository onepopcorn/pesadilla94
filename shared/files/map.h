#ifndef _MAP_FORMAT_H
#define _MAP_FORMAT_H

#define TILE_TYPE_BACKGROUND 0
#define TILE_TYPE_SOLID      1
#define TILE_TYPE_DOOR       2
#define TILE_TYPE_STAIRS     3

/**
 * Custom Tilemap format
 *
 * Tile flags are:
 *  - high nibble: type
 *  - low nibble: flags
 *    - bit 0: dirty
 *    - bit 1: flip
 *    - bit 2: unused
 *    - bit 3: unused
 *    - bit 4: unused
 * 
 */
typedef struct Tile
{
    uint8_t flags;
    uint8_t id;
} Tile;

typedef struct Map
{
    uint8_t width;
    uint8_t height;
    uint8_t tileWidth;
    uint8_t tileHeight;
    Tile data[];
} Map;

#endif