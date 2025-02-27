#include <dos.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/nearptr.h>

#include "physics/geom.h"
#include "io/resources.h"
#include "settings/settings.h"
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
 * NOTE: this function disables protected mode to have access to screen buffer. This seems not to be necessary
 *
 * @returns Success (1) or failure (0)
 *
 */
uint8_t openScreenBuffer(void) {
    // TODO: Investigate if we really need to disable protected mode. It seems to work fine without disabling it
    // Disable protected mode
    // if (__djgpp_nearptr_enable() == 0) {
    //     return EXIT_FAILURE;
    // }

    back_buffer = (uint8_t*)malloc(SCREEN_MEM_SIZE);

    if (!back_buffer) {
        perror("ERROR: failed to open frame buffer\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * Clean up backBuffer and enables protected mode again
 *
 */
void closeScreenBuffer(void) {
    // re-enable protected mode
    // __djgpp_nearptr_disable();
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
void waitFrames(uint8_t frames) {
    while (frames--) {
        waitVSync();
    }
}

/**
 * Draw text on screen at given position
 *
 * TODO: Refactor this routine to use nested loops instead of divisions and modulo which is way less efficient
 *
 * @param x Horizontal position where to start drawing text
 * @param y Vertical position where to start drawing text
 * @param font Pointer to font spritesheet
 * @param text Pointer to text string
 * @param max_length Max number of characters to break to the next line. If 0, there's no line break
 *
 */
void drawText(uint16_t x, uint16_t y, Sprite* font, char* text, uint8_t max_length) {
    uint16_t x_offset = 0;  // accumulate offset for when we trim next line whitespace
    uint16_t limit = max_length ? max_length : (SCREEN_WIDTH - x) / font->width;
    for (uint8_t i = 0; text[i]; i++) {
        uint16_t y_pos = y + i / limit * (font->height + 2);  // 2 = space between lines
        uint16_t x_pos = x + (i % limit) * font->width;

        if (text[i] == 32) {
            // Trim whitespace at beginning of line. 32 is ASCII value for whitespace
            x_offset += x_pos == x ? font->width * -1 : 0;
            continue;  // whitespaces don't need to be drawn, skip them after calculations
        }

        drawSprite(x_pos + x_offset, y_pos, font, text[i] - 32, false, 0);
    }
}

/**
 * Draw given sprite data into back buffer
 *
 * @param x Starting x position
 * @param y Starting y position
 * @param sprite Pointer to sprite struct
 * @param frame Sprite frame number to be printed
 * @param flip flip sprite horizontally
 * @param colorOverride Color for non transparent pixels if COLOR_TRANSPARENT is passed, the override takes no effect
 *
 * TODO: Consider using inlined assembly if performance is not good enough
 */
void drawSprite(uint16_t x, uint16_t y, Sprite* sprite, uint16_t frame, bool flip, uint8_t colorOverride) {
    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    const uint16_t offset = frame * width * height;
    uint8_t* src = &sprite->data[offset];
    uint8_t* dst = &back_buffer[(y * SCREEN_WIDTH) + x];

    for (uint16_t i = 0; i < height; i++) {
        uint8_t* src_row = src + i * width;
        uint8_t* dst_row = dst + i * SCREEN_WIDTH;

        for (uint16_t j = 0; j < width; j++) {
            /**
             * To flip the sprite we want to substract the column number to the position of the last byte of each row.
             * So, basically we are reading each row backwards
             */
            // TODO: Consider optimiznig this branching
            uint8_t idx = !flip ? src_row[j] : src_row[width - 1 - j];  // sprite->data
            if (idx == COLOR_TRANSPARENT) continue;
            dst_row[j] = colorOverride > COLOR_TRANSPARENT ? colorOverride : idx;
        }
    }

    /**
     *
     * NOTE: This was a "too clever" idea to avoid nested loop that has a terrible performance.
     *       It's left here as a lesson.
     *       idiv and modulo operations can be very slow on x86
     *
     */
    // for (uint8_t i = 0; i < sprite->width * sprite->height; i++) {
    //     /**
    //      * To flip the sprite we want to substract the column number to the position of the last byte of each row.
    //      * So, basically we are reading each row backwards
    //      *
    //      * (i / sprite->w + 1)  is the last column per each row
    //      * i % sprite->w        is the value if each column based on i value
    //      *
    //      */
    //     uint8_t idx = !flip ? sprite->data[i + offset] : sprite->data[offset + ((i / sprite->width + 1) * sprite->width) - i % sprite->width];
    //     if (idx == TRANSPARENT_IDX) continue;

    //     /**
    //      * Convert from lineal memory to rectangle on screen memory.
    //      *
    //      * i / w  is the row number. "i" is an int so all decimal numbers are dropped
    //      * i % w  is a way to get the column number by "resetting" the value to 0 when i = w
    //      */
    //     back_buffer[(i / sprite->width + y) * SCREEN_WIDTH + i % sprite->width + x] = idx;
    // }
}

/**
 * Draw a single tile.
 *
 * Tiles have fixed size and no transparency so they can be drawn
 * copying blocks of data which is way faster than a nested loop
 *
 */
void drawTile(uint16_t x, uint16_t y, Sprite* tileset, uint8_t id) {
    uint8_t width = TILE_SIZE;
    uint8_t height = TILE_SIZE;

    const uint16_t offset = id * width * height;
    uint8_t* src = &tileset->data[offset];
    uint8_t* dst = &back_buffer[(y * SCREEN_WIDTH) + x];

    for (uint8_t i = 0; i < height; i++) {
        uint8_t* src_row = src + i * width;
        uint8_t* dst_row = dst + i * SCREEN_WIDTH;
        memcpy(dst_row, src_row, width);
    }
}

void clearScreen() {
    fillScreen(0);
    dumpBuffer();
}

/**
 * Fill screen with given color
 *
 * @param color Color value to fill the screen with
 *
 */
void fillScreen(uint8_t color) {
    memset(back_buffer, color, SCREEN_MEM_SIZE);
}

/**
 * Copy backbuffer to screenbuffer
 *
 */
void dumpBuffer() {
    // Video memory address can't be accessed directly, we need to use DJGPP offset
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

/**
 * Draw given rectangle with given color
 *
 * Debug utility function. Needs to be compiled with DEBUG flag
 *
 * @param rect Rectangle {x, y, width, height} of the are to be drawn
 * @param color Color palette index to fill the rectangle with
 *
 * TODO Consider using assembly for this although it seems to be quite fast already
 */
void drawRectColor(Rect rect, uint8_t color) {
    for (uint8_t row = 0; row < rect.h; row++) {
        memset(back_buffer + rect.x + (rect.y + row) * SCREEN_WIDTH, color, rect.w);
    }
}

#ifdef DEBUG
/**
 * Draw outlines for given recangle with given color
 *
 * Debug utility function. Needs to be compiled with DEBUG flag
 *
 * @param rect Rectangle {x, y, width, height} of the are to be drawn
 * @param color Color palette index to fill the rectangle with
 *
 */
void drawBBoxColor(Rect rect, uint8_t color) {
    for (uint8_t row = 0; row < rect.h; row++) {
        for (uint8_t col = 0; col < rect.w; col++) {
            if (row == 0 || row == rect.h - 1 || col == 0 || col == rect.w - 1) {
                back_buffer[rect.x + col + (rect.y + row) * SCREEN_WIDTH] = color;
            }
        }
    }
}
#endif