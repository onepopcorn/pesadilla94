#include <string.h>

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