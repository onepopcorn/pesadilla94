#ifndef VGA_H
#define VGA_H

#include "stdbool.h"

#include "geom.h"
#include "io/resources.h"

#define VIDEO_VGA_MODE 0x13
#define VIDEO_TEXT_MODE 0x03
#define VIDEO_MEM_START 0xA0000

#define VGA_STATUS_REG 0x3DA
#define VSYNC_BITMASK 0x08

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_MEM_SIZE 320 * 200

#define TRANSPARENT_IDX 0

void setVideoMode(uint8_t mode);

uint8_t openScreenBuffer(void);

void closeScreenBuffer(void);

void waitVSync();

void waitFrames(int frames);

void drawText(int x, int y, Sprite *font, char *text, int max_length);

void drawSprite(int x, int y, Sprite *sprite, int frame, bool flip);

void drawTile(int x, int y, Sprite *tileset, int id);

void drawRect(Rect rect, uint8_t *data);

void drawRectColor(Rect rect, int color);

// TODO: Deprecated, consider removing
// void getBufferData(Rect rect, uint8_t *dest);

void fillScreen(int color);

void dumpBuffer();

#endif