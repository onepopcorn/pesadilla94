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

    char start[] = "PRESS ENTER TO START";
    drawText(35, 80, font, start, 31);

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