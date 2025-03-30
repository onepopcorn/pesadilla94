#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "render/video.h"
#include "io/keyboard.h"
#include "io/sound/sound.h"
#include "../assets.h"
#include "screens.h"
#include "text.h"
#include "settings/controls.h"
#include "render/effects.h"

#include "menu.h"

enum Screen menu() {
    // Setup on screen enter
    enum Screen nextScreen = SCREEN_GAME;
    bool running = true;

    Sprite *menuBg = loadSprite("menu.spr");
    if (!menuBg) {
        perror("Error opening menu.spr file");
        return EXIT_FAILURE;
    }

    drawSprite(0, 0, menuBg, 0, false, COLOR_TRANSPARENT);
    drawText(160 - (STR_MENU_L1_LEN / 2 * 8), 110, font, STR_MENU_L1, 55, 30);
    drawText(160 - (STR_MENU_L2_LEN / 2 * 8), 120, font, STR_MENU_L2, 55, 30);

    waitVSync();
    dumpBuffer();

    fadeIn(255, 10);

    while (running) {
        if (isKeyJustPressed(m_SHOOT)) {
            nextScreen = SCREEN_GAME;
            running = false;
            playSound(SFX_MENU_START);
            waitFrames(30);
        }

        if (isKeyJustPressed(m_QUIT)) {
            nextScreen = SCREEN_EXIT;
            running = false;
        }
    }

    // On screen leave
    fadeToBlack(255, 10);

    clearScreen();

    free(menuBg);
    menuBg = NULL;

    return nextScreen;
}