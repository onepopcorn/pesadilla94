#ifndef _VGA_H
#define _VGA_H

#include "stdbool.h"

#include "physics/geom.h"
#include "io/resources.h"

#define VIDEO_VGA_MODE 0x13
#define VIDEO_TEXT_MODE 0x03
#define VIDEO_MEM_START 0xA0000

#define VGA_STATUS_REG 0x3DA
#define VSYNC_BITMASK 0x08

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_MEM_SIZE 320 * 200

#define COLOR_TRANSPARENT 0
#define COLOR_WHITE 15
#define COLOR_BLACK 16

void setVideoMode(uint8_t mode);

uint8_t openScreenBuffer(void);

void closeScreenBuffer(void);

void waitVSync();

void waitFrames(uint8_t frames);

void drawText(uint16_t x, uint16_t y, Sprite *font, char *text, uint8_t max_length);

void drawSprite(uint16_t x, uint16_t y, Sprite *sprite, uint16_t frame, bool flip, uint8_t colorOverride);

void drawTile(uint16_t x, uint16_t y, Sprite *tileset, uint8_t id);

void clearScreen();

void fillScreen(uint8_t color);

void dumpBuffer();

void drawRectColor(Rect rect, uint8_t color);

#ifdef DEBUG

void drawBBoxColor(Rect rect, uint8_t color);
#endif

#endif  // _VGA_H