#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "render/video.h"
#include "render/hud.h"
#include "io/resources.h"
#include "io/keyboard.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "entities/whip.h"
#include "timer.h"
#include "map.h"

#include <crt0.h>

// #include "sprites.h"

/**
 *
 * Disable memory paging because we use C handler for interrupts
 * That is a drastic solution for not having to lock memory in interrupts
 * more info https://delorie.com/djgpp/v2faq/faq18_9.html
 *
 */
int _crt0_startup_flags = _CRT0_FLAG_LOCK_MEMORY;

int main(int argc, char *argv[]) {
    // Config video
    setVideoMode(VIDEO_VGA_MODE);
    uint8_t bufferError = openScreenBuffer();

    if (bufferError) {
        printf("Error opening franebuffer");
        return EXIT_FAILURE;
    }

    // Init interrupts
    timerInit();
    keyboardInit();
    mapInit();
    hudInit();

    // Entities & game logic
    waitFrames(3);
    fillScreen(0);

    // draw map
    drawMap();

    // TODO: Create a resources manager to load all sprites at the same time
    playerInit();
    enemyInit();
    whipInit();

    playerSpawn(165, 59);
    enemySpawn(100, 111);
    enemySpawn(160, 63);
    enemySpawn(130, 158);
    enemySpawn(200, 111);

    // static const char MESSAGE[] = "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!";
    // static const int MESSAGE_LENGTH = sizeof(MESSAGE);
    // int current_charnum = 0;
    // char *text = malloc(sizeof(char) * MESSAGE_LENGTH);
    // text[0] = '\0';
    // int counter = 0;

    uint32_t previousTime = getMilliseconds();

    while (!keys[KEY_ESC]) {
        // Manage timers
        uint32_t now = getMilliseconds();
        uint32_t delta = now - previousTime;
        previousTime = now;

        waitVSync();

        // This dumps from back buffer to screen buffer all at once
        dumpBuffer();

        // Re-paint tiles that has been overwritten by sprites
        restoreMapTiles();

        // Update entity logic & render in the same loop
        updateEntities(delta);

        // Typewriter effect
        // if (current_charnum <= MESSAGE_LENGTH && ++counter & 1) {
        //     for (int i = 0; i < current_charnum - 1; i++) {
        //         text[i] = MESSAGE[i];
        //         text[i + 1] = '\0';
        //     }
        //     current_charnum++;
        //     drawText(35, 10, font, text, 31);
        // }
    }

    // Restore timer interrupt
    timerFree();
    keyboardFree();

    // Free memory
    closeScreenBuffer();

    playerFree();

    enemyFree();

    whipFree();

    hudFree();

    mapFree();

    // Return to text mode
    setVideoMode(VIDEO_TEXT_MODE);

    return EXIT_SUCCESS;
}