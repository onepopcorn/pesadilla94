#include <stdlib.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "assets.h"
#include "screens.h"

#include "intro.h"

static const char MESSAGE[] = "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!";
static const int MESSAGE_LENGTH = sizeof(MESSAGE);

enum Screen intro() {
    int current_charnum = 0;
    char *text = malloc(sizeof(char) * MESSAGE_LENGTH);
    text[0] = '\0';
    int counter = 0;

    while (!isKeyJustPressed(KEY_ENTER)) {
        if (isKeyJustPressed(KEY_ESC)) {
            return SCREEN_EXIT;
        }

        if (current_charnum <= MESSAGE_LENGTH && ++counter) {
            for (int i = 0; i < current_charnum - 1; i++) {
                text[i] = MESSAGE[i];
                text[i + 1] = '\0';
            }
            current_charnum++;
            drawText(35, 80, font, text, 31);
        }

        waitVSync();

        dumpBuffer();
    }

    free(text);
    text = NULL;

    return SCREEN_MENU;
}