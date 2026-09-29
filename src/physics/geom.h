#ifndef _GEOM_H
#define _GEOM_H

#include <stdint.h>

typedef struct Rect {
    int x, y;
    int w, h;
} Rect;

typedef struct Vec2 {
    int x, y;
} Vec2;

#endif  // _GEOM_H