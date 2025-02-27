#ifndef _GEOM_H
#define _GEOM_H

#include <stdint.h>

typedef struct Rect {
    uint16_t x, y;
    uint8_t w, h;
} Rect;

typedef struct Vec2 {
    uint16_t x, y;
} Vec2;

#endif  // _GEOM_H