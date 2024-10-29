#ifndef _SPRITESHEET_FORMAT_H
#define _SPRITESHEET_FORMAT_H

/**
 * Custom Spritesheet file format
 * 
 */

typedef struct Sprite {
    int width;
    int height;
    int frames;
    uint8_t data[];
} Sprite;

#endif
