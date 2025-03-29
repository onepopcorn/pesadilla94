#include <stdint.h>
#include "render/video.h"
#include "io/resources.h"
#include "io/keyboard.h"
#include "io/sound/sound.h"
#include "assets.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "entities/whip.h"
#include "timer.h"
#include "map.h"
#include "hud/hud.h"
#include "screens.h"
#include "gameState.h"
#include "macros.h"
#include "settings/controls.h"
#include "render/effects.h"

#include "game.h"

#ifdef CHEATS
#define CHEAT_MODE 1
#endif

enum Screen nextScreen;
bool running;

enum Screen game() {
    LevelData currentLevel = levels[gameState.level];

    gameState.timeLeft = currentLevel.time;
    gameState.doorsLeft = startLevel(gameState.level);  // TODO: Add checks when failing loading the level

    startSong();

    drawMap();
    drawHUD();

    // Spawn player and enemies
    uint8_t spawnOffset = currentLevel.positionsIdx;
    uint8_t enemyCount = currentLevel.enemyCount;

    Vec2 playerPos = spawnPositions[spawnOffset];
    playerSpawn(playerPos.x, playerPos.y);

    if (enemyCount > 0) {
        for (uint8_t i = 0; i < enemyCount; i++) {
            Vec2 pos = spawnPositions[spawnOffset + 1 + i];  // Offest player position
            enemySpawn(pos.x, pos.y);
        }
    }

    uint32_t previousTime = getMilliseconds();
    uint32_t secondsCounter = previousTime;

    running = true;
    enum GameLoopState gameLoopState = GAME_RUNNING;

    waitVSync();
    dumpBuffer();
    restorePalette();

    // UPDATE
    while (running) {
        // Manage timers
        uint32_t now = getMilliseconds();
        uint32_t delta = now - previousTime;
        previousTime = now;
        if (now - secondsCounter >= 1000) {
            secondsCounter = now;
            if (gameLoopState == GAME_RUNNING && gameState.timeLeft > 0) {
                updateTime(-1);
            }
        }

        switch (gameLoopState) {
            case GAME_PAUSED:
                // Quit
                if (isKeyJustPressed(m_QUIT)) {
                    nextScreen = SCREEN_MENU;
                    running = false;
                }

                // Resume
                if (isKeyJustPressed(m_PAUSE)) {
                    gameLoopState = GAME_RUNNING;
                }
                break;
            case GAME_RUNNING:

                // Quit
                if (isKeyJustPressed(m_QUIT)) {
                    nextScreen = SCREEN_MENU;
                    running = false;
                }

                // Pause
                if (isKeyJustPressed(m_PAUSE)) gameLoopState = GAME_PAUSED;

#if CHEAT_MODE == 1
                if (isKeyJustPressed(KEY_S)) {
                    gameState.doorsLeft = 0;
                    gameState.timeLeft = 0;
                }
                if (isKeyJustPressed(KEY_K)) playerDie();
                if (isKeyJustPressed(KEY_W)) {
                    gameState.lives = 1;
                    playerDie();
                }
#endif

                // Re-paint tiles that has been overwritten by sprites
                restoreMapTiles();

                // Update entity logic & render in the same loop
                updateEntities(delta);

                // Transition to next level
                if (gameState.doorsLeft == 0) {
                    nextScreen = SCREEN_LEVEL_END;
                    running = false;
                }

                // Transition to game over
                if (gameState.lives == 0) {
                    nextScreen = SCREEN_GAME_OVER;
                    running = false;
                }
        }

        waitVSync();
        dumpBuffer();  // Dump backbuffer to video memory
    }

    // CLEANUP
    stopSong();
    running = true;
    clearAllTimeouts();
    destroyAllEntities();
    endLevel();

    waitVSync();
    dumpBuffer();

    return nextScreen;
}