#include "screens.h"
#include "render/video.h"
#include "gameState.h"
#include "timer.h"
#include "hud/hud.h"
#include "render/effects.h"
#include "assets.h"
#include "text.h"
#include "map.h"
#include "io/keyboard.h"
#include "io/sound/sound.h"
#include "settings/controls.h"

#include "levelend.h"

#define END_TXT_X 160 - STR_LEVEL_END_LEN / 2 * font->width
#define END_TXT_Y 100

enum Screen levelend() {
    uint32_t previousTime = getMilliseconds();
    uint32_t secondsCounter = previousTime;

    // Do not clear screen just draw on top of it
    drawRectColor((Rect){END_TXT_X - 5, END_TXT_Y - 5, STR_LEVEL_END_LEN * font->width + 10, font->height + 10}, COLOR_BLACK);
    drawText(END_TXT_X, END_TXT_Y, font, STR_LEVEL_END, COLOR_TRANSPARENT, 0);
    dumpBuffer();
    // waitFrames(60);

    uint8_t voice = -1;

    if (gameState.timeLeft > 0) {
        voice = playSound(SFX_POINTS);
    };

    // TODO: Skip counting by key press
    while (gameState.timeLeft > 0) {
        if (isKeyJustPressed(m_SHOOT) || isKeyJustPressed(m_QUIT)) {
            break;
        }

        uint32_t now = getMilliseconds();
        previousTime = now;
        // Speed up seconds to count faster
        if (now - secondsCounter >= 30) {
            secondsCounter = now;
            updateTime(-1);
            updatePoints(1);
        }
        waitVSync();
        dumpBuffer();
    }

    if (voice > -1) stopSound(voice);

    waitFrames(60);
    fadeToBlack(255, 10);
    waitVSync();

    // TODO: Move to game ending instead of next game screen if there're no more levels
    return ++gameState.level < NUM_LEVELS ? SCREEN_GAME : SCREEN_ENDING;
}