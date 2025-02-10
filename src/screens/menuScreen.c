#include <stdint.h>
#include "render/video.h"
#include "io/keyboard.h"
#include "../assets.h"
#include "screens.h"
#include "menuScreen.h"

enum Screen menuScreen() {
    // On screen enter
    clearScreen();
    waitFrames(15);

    char start[] = "PRESS ENTER TO START";
    drawText(35, 80, font, start, 31);

    waitVSync();
    dumpBuffer();

    while (1) {
        if (keys[KEY_ENTER]) {
            return SCREEN_GAME;
        }

        if (keys[KEY_ESC]) {
            return SCREEN_EXIT;
        }
    }

    // On screen leave
}