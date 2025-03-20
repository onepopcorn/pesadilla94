#include <stdint.h>
#include "render/video.h"
#include "io/keyboard.h"
#include "../assets.h"
#include "screens.h"
#include "text.h"
#include "settings/controls.h"
#include "render/effects.h"
#include "io/sound/sound.h"

#include "menu.h"

enum Screen menu() {
    // On screen enter
    clearScreen();
    waitFrames(15);

    playWhip();
    drawText(160 - (STR_MENU_L1_LEN / 2 * 8), 96, font, STR_MENU_L1, COLOR_ORANGE, 31);

    waitVSync();
    dumpBuffer();
    fadeIn(255, 10);

    while (1) {
        if (isKeyJustPressed(m_SHOOT)) {
            return SCREEN_GAME;
        }

        if (isKeyJustPressed(m_QUIT)) {
            return SCREEN_EXIT;
        }
    }

    // On screen leave
    fadeToBlack(255, 10);
}