#include "render/video.h"
#include "io/sound/sound.h"
#include "assets.h"
#include "screens.h"
#include "gameState.h"
#include "text.h"
#include "settings/controls.h"
#include "render/effects.h"

#include "gameover.h"

enum Screen gameover() {
    waitFrames(60);
    clearScreen();

    playSound(SFX_GAME_OVER);

    drawText(160 - (sizeof(STR_GAMEOVER) * 4), 80, font, STR_GAMEOVER, COLOR_TRANSPARENT, 0);
    waitVSync();
    dumpBuffer();

    fadeIn(255, 10);
    waitFrames(60);
    fadeToBlack(255, 10);

    resetGameState();
    return SCREEN_MENU;
}