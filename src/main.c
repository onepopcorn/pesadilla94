#define BUCKET_LIB_IMPLEMENTATION
#include "../vendor/bucket-lib/bucket-lib.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <crt0.h>
#include <dos.h>

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
#include "screens/ending.h"
#include "settings/settings.h"
#include "render/effects.h"
#include "text.h"
#include "version.h"

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
int initializSystems(Arena* arena) {
    int bufferError = openScreenBuffer(arena);
    if (bufferError) return EXIT_FAILURE;

    // Interrupt based systems
    timerInit();
    keyboardInit();

    // Resources
    int assetsError = assetsInit(arena);
    if (assetsError) return EXIT_FAILURE;

    // Sound
    int soundError = soundInit();
    if (soundError) return EXIT_FAILURE;

    // Initialize video system
    Video_wait_vsync();
    Video_set_mode(VIDEO_VGA_MODE);

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
    clearScreen();
    closeScreenBuffer();

    // Return to text mode
    Video_wait_vsync();
    Video_set_mode(VIDEO_TEXT_MODE);

    // Free sound drivers
    soundFree();
}

void instructions() {
    system("CLS");

    printf("\n%s", STR_NAME);
    printf(" v%s\n", VERSION);

    printf("%s\n\n", STR_AUTHOR);

    printf(STR_INSTRUCTIONS);
    while (!kbhit());
}

int main(int argc, char* argv[]) {
    Arena arena = arena_create(MiB(1));
    Arena* arena_ptr = &arena;

    instructions();

    int systemErrors = initializSystems(arena_ptr);
    if (systemErrors) return EXIT_FAILURE;

    clearScreen();

    enum Screen currentScreen = SCREEN_INTRO;

    while (currentScreen != SCREEN_EXIT) {
        switch (currentScreen) {
            case SCREEN_INTRO:
                currentScreen = intro(arena_ptr);
                break;
            case SCREEN_MENU:
                resetGameState();
                currentScreen = menu(arena_ptr);
                break;
            case SCREEN_GAME:
                currentScreen = game(arena_ptr);
                break;
            case SCREEN_LEVEL_END:
                currentScreen = levelend();
                break;
            case SCREEN_GAME_OVER:
                currentScreen = gameover();
                break;
            case SCREEN_ENDING:
                currentScreen = ending(arena_ptr);
                break;
            default:
                currentScreen = SCREEN_EXIT;
        }
    }

    uninitializSystems();

    arena_destroy(arena_ptr);

    return EXIT_SUCCESS;
}
