#include <stdint.h>
#include "render/video.h"
#include "io/keyboard.h"
#include "../assets.h"
#include "screens.h"
#include "menu.h"

enum Screen menu() {
    // On screen enter
    clearScreen();
    waitFrames(15);

    char start[] = "PRESS ENTER TO START\0";
    drawText(80, 96, font, start, COLOR_ORANGE, 31);

    waitVSync();
    dumpBuffer();

    while (1) {
        if (isKeyJustPressed(KEY_ENTER)) {
            return SCREEN_GAME;
        }

        if (isKeyJustPressed(KEY_ESC)) {
            return SCREEN_EXIT;
        }
    }

    // On screen leave
}