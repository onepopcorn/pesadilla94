#include <stdlib.h>
#include <stdio.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "assets.h"
#include "screens.h"

#include "intro.h"

static const char MESSAGE[] = "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!";
static const uint8_t MESSAGE_LENGTH = sizeof(MESSAGE);

enum Screen intro() {
    // TODO: Intro scroll & fake image camera movement
    Sprite *introBg = loadSprite("intro.spr");
    if (!introBg) {
        perror("Error opening intro.spr file");
        return EXIT_FAILURE;
    }

    drawSprite(0, 0, introBg, 0, false, COLOR_TRANSPARENT);

    uint8_t current_charnum = 0;
    char *text = malloc(sizeof(char) * MESSAGE_LENGTH);
    text[0] = '\0';
    uint8_t counter = 0;

    while (!isKeyJustPressed(KEY_ENTER)) {
        if (isKeyJustPressed(KEY_ESC)) {
            return SCREEN_EXIT;
        }

        if (current_charnum <= MESSAGE_LENGTH && ++counter) {
            for (uint8_t i = 0; i < current_charnum - 1; i++) {
                text[i] = MESSAGE[i];
                text[i + 1] = '\0';
            }
            current_charnum++;
            drawText(35, 200 - 32, font, text, 31);
        }

        waitVSync();

        dumpBuffer();
    }

    free(text);
    text = NULL;

    free(introBg);
    introBg = NULL;

    return SCREEN_MENU;
}