#include <dos.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/nearptr.h>

#include "video.h"

static uint8_t* screen_buffer;
static uint8_t* back_buffer;

/**
 * Set video to given mode
 *
 * @param mode Video mode to set
 */
void setVideoMode(uint8_t mode) {
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

/**
 * Initialize both screen and back buffers.
 *
 * NOTE: this function disables protected mode to have access to screen buffer
 *
 * @returns Success (1) or failure (0)
 *
 */
uint8_t openScreenBuffer(void) {
    if (__djgpp_nearptr_enable() == 0) {
        return EXIT_FAILURE;
    }

    // TODO: __djgpp_conventional_base may change every once in a while. Consider recalculate the pointer on dumpBuffer
    screen_buffer = (uint8_t*)(VIDEO_MEM_START + __djgpp_conventional_base);
    back_buffer = (uint8_t*)malloc(SCREEN_MEM_SIZE);

    if (!screen_buffer || !back_buffer) {
        fprintf(stderr, "ERROR: failed to open frame buffer\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * Clean up backBuffer and enables protected mode again
 *
 */
void closeScreenBuffer(void) {
    __djgpp_nearptr_disable();
    free(back_buffer);
}

/**
 * Wait for vertical sync to start (CRT trace finishes)
 *
 */
void waitVSync() {
    // wait until retrace starts
    while ((inp(VGA_STATUS_REG) & VSYNC_BITMASK) == 0);
    // wait until retrace ends
    while ((inp(VGA_STATUS_REG) & VSYNC_BITMASK) != 0);
}

/**
 * Wait given amount of frames
 *
 */
void waitFrames(int frames) {
    while (frames--) {
        waitVSync();
    }
}

/**
 * Draw text on screen at given position
 *
 * @param x Horizontal position where to start drawing text
 * @param y Vertical position where to start drawing text
 * @param font Pointer to font spritesheet
 * @param text Pointer to text string
 * @param max_length Max number of characters to break to the next line. If 0, there's no line break
 *
 */
void drawText(int x, int y, Sprite* font, char* text, int max_length) {
    int x_offset = 0;  // accumulate offset for when we trim next line whitespace
    int limit = max_length ? max_length : (SCREEN_WIDTH - x) / font->width;
    for (int i = 0; text[i]; i++) {
        int y_pos = y + i / limit * (font->height + 2);  // 2 = space between lines
        int x_pos = x + (i % limit) * font->width;

        if (text[i] == 32) {
            // Trim whitespace at beginning of line. 32 is ASCII value for whitespace
            x_offset += x_pos == x ? font->width * -1 : 0;
            continue;  // whitespaces don't need to be drawn, skip them after calculations
        }

        drawSprite(x_pos + x_offset, y_pos, font, text[i] - 32, 0);
    }
}

/**
 * Draw given sprite data into back buffer
 * TODO: Consider using inlined assembly for this
 * TODO: Test perf using nested loops and memcpy instead of division and modulo operators
 *
 * @param x Starting x position
 * @param y Starting y position
 * @param sprite Pinter to sprite struct
 * @param frame Sprite frame number to be printed
 * @param flip flip sprite horizontally
 *
 */
void drawSprite(int x, int y, Sprite* sprite, int frame, bool flip) {
    const int offset = frame * sprite->width * sprite->height;
    for (int i = 0; i < sprite->width * sprite->height; i++) {
        /**
         * To flip the sprite we want to substract the column number to the position of the last byte of each row.
         * So, basically we are reading each row backwards
         *
         * (i / sprite->w + 1)  is the last column per each row
         * i % sprite->w        is the value if each column based on i value
         *
         */
        uint8_t idx = !flip ? sprite->data[i + offset] : sprite->data[offset + ((i / sprite->width + 1) * sprite->width) - i % sprite->width];
        if (idx == TRANSPARENT_IDX) continue;

        /**
         * Convert from lineal memory to rectangle on screen memory.
         *
         * i / w  is the row number. "i" is an int so all decimal numbers are dropped
         * i % w  is a way to get the column number by "resetting" the value to 0 when i = w
         */
        back_buffer[(i / sprite->width + y) * SCREEN_WIDTH + i % sprite->width + x] = idx;
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
void drawMap(int offset_x, int offset_y, Sprite* tileset, int map_num) {
    int map_offset = MAP_H * MAP_W * map_num;
    for (int i = 0; i < MAP_W * MAP_H; i++) {
        drawSprite(i % MAP_W * tileset->width + offset_x, i / MAP_W * tileset->height + offset_y, tileset, MAPS[map_offset + i] - 1, false);
    }
}

/**
 * Draw given rectangle with given data pointer
 *
 * @param rect Rectangle {x, y, width, height} of the are to be drawn
 * @param data Pointer to where that do be drawn is stored
 *
 * TODO Consider using assembly for this
 */
void drawRect(Rect rect, uint8_t* data) {
    // for(int i = 0; i < rect.w * rect.h; i++) {
    //     backBuffer[(i / rect.w + rect.y) * SCREEN_WIDTH + i % rect.w + rect.x] = data[i];
    // }

    for (int row = 0; row < rect.h; row++) {
        memcpy(back_buffer + rect.x + (rect.y + row) * SCREEN_WIDTH, data + row * rect.w, sizeof(uint8_t) * rect.w);
    }
}

/**
 * Fills given pointer with the given rect portion of the data in the screen buffer
 *
 * @param rect Rectangle {x, y, width, height} of the area to get the data from
 * @param dest Pointer to where the data is gonna be stored
 *
 */
void getBufferData(Rect rect, uint8_t* dest) {
    // Efficient but uses idiv
    // for(int i = 0; i < rect.w * rect.h; i++) {
    //     dest[i] = backBuffer[(i / rect.w + rect.y) * SCREEN_WIDTH + i % rect.w + rect.x];
    // }

    // Less efficient
    // for(int row = 0; row < rect.h; row++) {
    //     for(int col = 0; col < rect.w; col++) {
    //         dest[row * rect.w + col] = backBuffer[(rect.y + row) * SCREEN_WIDTH + col + rect.x];
    //     }
    // }

    // Seems to be the most efficient
    for (int row = 0; row < rect.h; row++) {
        memcpy(dest + row * rect.w, back_buffer + rect.x + (rect.y + row) * SCREEN_WIDTH, sizeof(uint8_t) * rect.w);
    }
}

/**
 * Fill screen with given color
 *
 * @param color Color value to fill the screen with
 *
 */
void fillScreen(int color) {
    memset(back_buffer, color, SCREEN_MEM_SIZE);
}

/**
 * Copy backbuffer to screenbuffer
 *
 * NOTE: Consider to recalculate screenBuffer pointer since __djgpp_conventional_base may vary over time
 */
void dumpBuffer() {
    // memcpy(screen, backBuffer, SCREEN_MEM_SIZE);

    screen_buffer = (uint8_t*)(VIDEO_MEM_START + __djgpp_conventional_base);

    // I think memcpy with -O3 does the same
    asm volatile(
        "cld\n\t"                   // Clear direction flag (forward copying)
        "rep movsd\n\t"             // Copy ECX DWORDs from ESI (backbuffer) to EDI (VGA memory)
        :                           /* no output */
        : "S"(back_buffer),         // ESI = backbuffer source
          "D"(screen_buffer),       // EDI = VGA memory destination
          "c"(SCREEN_MEM_SIZE / 4)  // ECX = Number of DWORDs to copy (320*200/4)
        : "memory");
}