#include <stdlib.h>
#include <stdint.h>
#include <crt0.h>

#include "render/video.h"
#include "io/keyboard.h"
#include "assets.h"
#include "timer.h"
#include "io/sound/sound.h"

#include "screens/screens.h"
#include "screens/menu.h"
#include "screens/intro.h"
#include "screens/game.h"
#include "screens/gameover.h"
#include "screens/levelend.h"
#include "settings/settings.h"
#include "render/effects.h"

#include "gameState.h"

/**
 *
 * Disable memory paging because we use C handler for interrupts
 * That is a drastic solution for not having to lock memory in interrupts
 * more info https://delorie.com/djgpp/v2faq/faq18_9.html
 *
 */
int _crt0_startup_flags = _CRT0_FLAG_LOCK_MEMORY;

/**
 * Systems initializer
 *
 *
 */
uint8_t initializSystems() {
    uint8_t bufferError = openScreenBuffer();
    if (bufferError) return EXIT_FAILURE;

    // Interrupt based systems
    timerInit();
    keyboardInit();

    // Resources
    uint8_t assetsError = assetsInit();
    if (assetsError) return EXIT_FAILURE;

    // Sound
    uint8_t soundError = soundInit();
    if (soundError) return EXIT_FAILURE;

    // Initialize video system
    setVideoMode(VIDEO_VGA_MODE);

    initVideoEffects();

    return EXIT_SUCCESS;
}

/**
 * System uninitializer
 *
 */
void uninitializSystems() {
    // Restore interrupts
    timerFree();
    keyboardFree();

    // Free memory
    closeScreenBuffer();

    assetsFree();

    // Return to text mode
    setVideoMode(VIDEO_TEXT_MODE);

    // Free sound drivers
    soundFree();
}

int main(int argc, char *argv[]) {
    uint8_t systemErrors = initializSystems();
    if (systemErrors) return EXIT_FAILURE;

    waitFrames(3);
    fillScreen(0);

    enum Screen currentScreen = SCREEN_INTRO;

    while (currentScreen != SCREEN_EXIT) {
        switch (currentScreen) {
            case SCREEN_INTRO:
                currentScreen = intro();
                break;
            case SCREEN_MENU:
                resetGameState();
                currentScreen = menu();
                break;
            case SCREEN_GAME:
                currentScreen = game();
                break;
            case SCREEN_LEVEL_END:
                currentScreen = levelend();
                break;
            case SCREEN_GAME_OVER:
                currentScreen = gameover();
                break;
            default:
                currentScreen = SCREEN_EXIT;
        }
    }

    uninitializSystems();

    return EXIT_SUCCESS;
}
