#include <string.h>

#include "render/video.h"
#include "assets.h"
#include "hud.h"

#define SEARCH_TEXT "HUSMEANDO"
#define SEARCH_TEXT_LEN 9

void writeMessage(char* message) {
    drawRectColor((Rect){5, 10, 16 * strlen(message), 16}, 16);
    drawText(5, 10, font, message, 31);
}

void showProgress(uint8_t progress) {
    // clean previous progress
    drawText(5, 19, font, SEARCH_TEXT, 31);
    uint8_t lineWidth = progress / 6;
    drawRectColor((Rect){80, 20, lineWidth + 2, 7}, COLOR_BLACK);
    drawRectColor((Rect){80, 20, lineWidth, 7}, COLOR_WHITE);
}

void clearProgress() {
    drawRectColor((Rect){5, 19, 8 * SEARCH_TEXT_LEN + 255 / 5, 8}, COLOR_BLACK);
}