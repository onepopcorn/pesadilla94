#include <stdlib.h>
#include <stdio.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "assets.h"
#include "screens.h"
#include "timer.h"

#include "intro.h"

static const char CRAWL_TEXT[] = "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!";
#define CRAWL_TEXT_LEN sizeof(CRAWL_TEXT)

static char BRAND_TEXT[] = "ONEPOPCORN 2025\0";

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
    char *text = malloc(sizeof(char) * CRAWL_TEXT_LEN);
    text[0] = '\0';

    while (running) {
        switch (curentSequence) {
            case SEQ_DRAW_BRANDING:
                drawText(100, 96, font, BRAND_TEXT, COLOR_ORANGE, 30);
                setTimeout(&nextSequence, SEQ_DRAW_INTRO, 4000);
                curentSequence = SEQ_BRANDING;
                break;

            case SEQ_BRANDING:
                if (isKeyJustPressed(KEY_ENTER)) {
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
                if (isKeyJustPressed(KEY_ENTER)) {
                    curentSequence = SEQ_INTRO;
                }
                break;

            case SEQ_INTRO:
                if (current_charnum <= CRAWL_TEXT_LEN && ++counter) {
                    for (uint8_t i = 0; i < current_charnum - 1; i++) {
                        text[i] = CRAWL_TEXT[i];
                        text[i + 1] = '\0';
                    }
                    current_charnum++;
                    drawText(35, 200 - 32, font, text, COLOR_ORANGE, 31);
                } else {
                    setTimeout(&nextSequence, SEQ_END, 4000);
                    curentSequence = SEQ_OUTRO;
                }
                break;

            case SEQ_OUTRO:
                if (isKeyJustPressed(KEY_ENTER)) {
                    running = false;
                }
                break;

            case SEQ_END:
                running = false;
                break;
        }

        if (isKeyJustPressed(KEY_ESC)) {
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