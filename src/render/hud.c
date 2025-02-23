#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "render/video.h"
#include "assets.h"
#include "hud.h"

// TODO: Function to draw lives
// TODO: Function to draw score
// TODO: Function to draw current weapon
// TODO: Function to draw time left

void writeMessage(char* message) {
    drawRectColor((Rect){5, 10, 16 * strlen(message), 16}, 16);
    drawText(5, 10, font, message, 31);
}

#ifdef DEBUG
void writeNumber(short x, short y, uint8_t num) {
    char buffer[4];
    sprintf(buffer, "%i", num);
    drawText(x, y, font, buffer, 2);
}
#endif