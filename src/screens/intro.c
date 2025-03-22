#include <stdlib.h>
#include <stdio.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "assets.h"
#include "screens.h"
#include "timer.h"
#include "text.h"
#include "io/sound/sound.h"
#include "settings/controls.h"
#include "render/effects.h"

#include "intro.h"

enum Sequence curentSequence;

void nextSequence(uint8_t sequence) {
    curentSequence = sequence;
}

enum Screen intro() {
    // TODO: Intro scroll & fake image camera movement
    Sprite *introBg = loadSprite("intro.spr");
    if (!introBg) {
        perror("Error opening intro.spr file");
        return EXIT_FAILURE;
    }

    curentSequence = SEQ_DRAW_BRANDING;

    uint8_t running = true;
    uint8_t current_charnum = 0;
    uint8_t counter = 0;
    char *text = malloc(sizeof(char) * sizeof(STR_INTRO_L1));
    text[0] = '\0';

    while (running) {
        switch (curentSequence) {
            case SEQ_DRAW_BRANDING:
                drawText(100, 96, font, STR_BRAND, COLOR_TRANSPARENT, 30);
                setTimeout(&nextSequence, SEQ_DRAW_INTRO, 4000);
                curentSequence = SEQ_BRANDING;
                break;

            case SEQ_BRANDING:
                if (isKeyJustPressed(m_SHOOT)) {
                    clearAllTimeouts();
                    curentSequence = SEQ_DRAW_INTRO;
                }
                break;

            case SEQ_DRAW_INTRO:
                fillScreen(COLOR_BLACK);
                drawSprite(0, 0, introBg, 0, false, COLOR_TRANSPARENT);
                curentSequence = SEQ_WAIT_INTRO;
                break;

            case SEQ_WAIT_INTRO:
                setTimeout(&nextSequence, SEQ_INTRO, 3000);
                if (isKeyJustPressed(m_SHOOT)) {
                    curentSequence = SEQ_INTRO;
                }
                break;

            case SEQ_INTRO:
                if (current_charnum <= sizeof(STR_INTRO_L1) && ++counter) {
                    for (uint8_t i = 0; i < current_charnum - 1; i++) {
                        text[i] = STR_INTRO_L1[i];
                        text[i + 1] = '\0';
                    }
                    current_charnum++;
                    drawText(35, 200 - 32, font, text, COLOR_TRANSPARENT, 31);
                } else {
                    setTimeout(&nextSequence, SEQ_END, 4000);
                    curentSequence = SEQ_OUTRO;
                }
                break;

            case SEQ_OUTRO:
                if (isKeyJustPressed(m_SHOOT)) {
                    running = false;
                }
                break;

            case SEQ_END:
                running = false;
                break;
        }

        if (isKeyJustPressed(m_QUIT)) {
            return SCREEN_EXIT;
        }

        waitVSync();
        dumpBuffer();
    }

    free(text);
    text = NULL;

    free(introBg);
    introBg = NULL;

    clearAllTimeouts();

    return SCREEN_MENU;
}