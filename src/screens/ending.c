#include <stdio.h>
#include <stdlib.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "io/resources.h"
#include "assets.h"
#include "screens.h"
#include "render/effects.h"
#include "settings/controls.h"
#include "text.h"

#include "ending.h"

enum Screen ending() {
    // Load resources
    Sprite *bg = loadSprite("ending.spr");
    if (!bg) {
        perror("Error opening ending.spr");
        return EXIT_FAILURE;
    }

    drawSprite(0, 0, bg, 0, false, COLOR_TRANSPARENT);
    dumpBuffer();
    fadeIn(255, 10);

    char *text = malloc(sizeof(STR_ENDING));
    text[0] = '\0';

    uint16_t charnum = 0;

    while (true) {
        if (isKeyJustPressed(m_SHOOT) || isKeyJustPressed(m_QUIT)) {
            break;
        }

        if (charnum < sizeof(STR_ENDING)) {
            for (uint8_t i = 0; i < charnum; i++) {
                text[i] = STR_ENDING[i];
                text[i + 1] = '\0';
            }
            charnum++;
            drawText(10, 30, font, text, COLOR_TRANSPARENT, 25);

            // Show second line
            if (charnum >= sizeof(STR_ENDING)) {
                waitFrames(60);
                drawText(10, 200 - 50, font, STR_ENDING2, COLOR_TRANSPARENT, 25);
            }
        }

        waitVSync();
        dumpBuffer();
    }

    free(bg);
    bg = NULL;

    free(text);
    text = NULL;

    fadeToBlack(255, 10);

    return SCREEN_MENU;
}