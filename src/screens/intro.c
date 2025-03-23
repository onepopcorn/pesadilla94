#include <stdlib.h>
#include <stdio.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "io/resources.h"
#include "assets.h"
#include "screens.h"
#include "timer.h"
#include "text.h"
#include "io/sound/sound.h"
#include "settings/controls.h"
#include "render/effects.h"

#include "intro.h"

static enum Sequence currentSequence;

void switchToNextSequence(uint8_t sequence) {
    currentSequence = sequence;
}

enum Screen intro() {
    // Setup in screen enter
    enum Screen nextScreen = SCREEN_MENU;

    // TODO: Intro scroll & fake image camera movement
    Sprite *introBg = loadSprite("intro.spr");
    if (!introBg) {
        perror("Error opening intro.spr file");
        return EXIT_FAILURE;
    }

    Sprite *introBg2 = loadSprite("intro2.spr");
    if (!introBg2) {
        perror("Error opening intro2.spr file");
        return EXIT_FAILURE;
    }

    currentSequence = SEQ_DRAW_BRANDING;

    uint8_t running = true;
    uint8_t current_charnum = 0;

    char *text = malloc(sizeof(char) * STR_MAX_TEXT_BUFFER);
    text[0] = '\0';

    fadeToBlack(0, 0);

    while (running) {
        switch (currentSequence) {
            case SEQ_WAIT:
                // Just wait
                break;

            // Draw onepopcorn logo
            case SEQ_DRAW_BRANDING:
                drawText(100, 96, font, STR_BRAND, COLOR_TRANSPARENT, 30);

                dumpBuffer();
                fadeIn(255, 10);

                currentSequence = SEQ_WAIT;
                setTimeout(&switchToNextSequence, SEQ_DRAW_INTRO, 1000);
                break;

            // Draw first intro part
            case SEQ_DRAW_INTRO:
                fadeToBlack(255, 0);

                drawSprite(0, 0, introBg, 0, false, COLOR_TRANSPARENT);

                dumpBuffer();
                fadeIn(255, 10);

                currentSequence = SEQ_INTRO;
                break;

            // Run first intro part
            case SEQ_INTRO:
                for (uint8_t i = 0; i < current_charnum; i++) {
                    text[i] = STR_INTRO_L1[i];
                    text[i + 1] = '\0';
                }
                current_charnum++;
                drawText(35, 200 - 32, font, text, COLOR_TRANSPARENT, 31);

                // Jump to next intro screen when done
                if (current_charnum > sizeof(STR_INTRO_L1)) {
                    currentSequence = SEQ_WAIT;
                    setTimeout(&switchToNextSequence, SEQ_DRAW_INTRO2, 2000);
                }

                break;

            // Draw second intro part
            case SEQ_DRAW_INTRO2:
                fadeToBlack(255, 10);

                // Reset typewriting effect
                current_charnum = 0;
                text[0] = '\0';

                // Clear text
                drawRectColor((Rect){0, 200 - 32, 160, 32}, COLOR_BLACK);
                drawRectColor((Rect){160, 200 - 32, 160, 32}, COLOR_BLACK);

                // Draw next image
                drawSprite(0, 0, introBg2, 0, false, COLOR_TRANSPARENT);

                dumpBuffer();
                fadeIn(255, 10);

                currentSequence = SEQ_INTRO2;
                break;

            // Run second intro part
            case SEQ_INTRO2:
                for (uint8_t i = 0; i < current_charnum; i++) {
                    text[i] = STR_INTRO_L2[i];
                    text[i + 1] = '\0';
                }
                current_charnum++;
                drawText(35, 200 - 32, font, text, COLOR_TRANSPARENT, 34);

                if (current_charnum > sizeof(STR_INTRO_L2)) {
                    currentSequence = SEQ_WAIT;
                    setTimeout(&switchToNextSequence, SEQ_END, 3000);
                }
                break;

            case SEQ_END:
                running = false;
                break;
        }

        if (isKeyJustPressed(m_QUIT) || isKeyJustPressed(m_SHOOT)) {
            nextScreen = SCREEN_MENU;
            running = false;
        }

        waitVSync();
        dumpBuffer();
    }

    // Clean up on screen leave
    clearAllTimeouts();

    fadeToBlack(255, 10);
    clearScreen();

    free(text);
    text = NULL;

    free(introBg);
    introBg = NULL;

    free(introBg2);
    introBg2 = NULL;

    return nextScreen;
}