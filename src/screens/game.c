#include <stdint.h>
#include "render/video.h"
#include "io/resources.h"
#include "io/keyboard.h"
#include "assets.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "entities/whip.h"
#include "timer.h"
#include "map.h"
#include "assets.h"
#include "screens.h"
#include "gameState.h"
#include "macros.h"

#include "game.h"

enum Screen nextScreen;
bool running;

// PRIVATE METHODS

void endTransition(uint8_t id) {
    running = false;
}

// TODO: Encode positions in map
Vec2 enemyPositions[4] = {
    {100, 111},
    {160, 63},
    {130, 158},
    {210, 111}};

void spawnEnemy(uint8_t i) {
    uint8_t enemyCount = 0;
    for (uint8_t i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i].type == TYPE_ENEMY_A) enemyCount++;
    }

    Vec2 pos = enemyPositions[i];
    enemySpawn(pos.x, pos.y);

    // stop spawning at max enemy limit
    if (++enemyCount >= 4) return;

    uint8_t nextEnemy = i >= 3 ? 0 : i + 1;
    setTimeout(spawnEnemy, nextEnemy, 3000);
}

// PUBLIC METHODS

enum Screen game() {
    // BOOSTRAP LEVEL

    // TODO: Add checks when failing loading the level
    gameState.doorsLeft = startLevel(gameState.level);
    drawMap();

    // TODO: Make this correctly per map or during game play
    playerSpawn(20, 58);
    // spawnEnemy(0);
    for (uint8_t i = 0; i < 4; i++) {
        Vec2 pos = enemyPositions[i];
        enemySpawn(pos.x, pos.y);
    }

    uint32_t previousTime = getMilliseconds();

    running = true;
    enum GameLoopState gameLoopState = GAME_RUNNING;

    // UPDATE
    while (running) {
        // Manage timers
        uint32_t now = getMilliseconds();
        uint32_t delta = now - previousTime;
        previousTime = now;

        switch (gameLoopState) {
            case GAME_IN_TRANSITION:
                // Just wait until transition ends
                continue;
            case GAME_PAUSED:
                // Quit
                if (isKeyJustPressed(KEY_ESC)) {
                    nextScreen = SCREEN_MENU;
                    running = false;
                }

                // Resume
                if (isKeyJustPressed(KEY_P)) {
                    gameLoopState = GAME_RUNNING;
                }
                break;
            case GAME_RUNNING:

                // Quit
                if (isKeyJustPressed(KEY_ESC)) {
                    nextScreen = SCREEN_MENU;
                    running = false;
                }

                // Pause
                if (isKeyJustPressed(KEY_P)) gameLoopState = GAME_PAUSED;

                // Re-paint tiles that has been overwritten by sprites
                restoreMapTiles();

                // Update entity logic & render in the same loop
                updateEntities(delta);

                // Transition to next level
                if (gameState.doorsLeft == 0) {
                    // TODO: Handle next screen load
                    nextScreen = SCREEN_GAME;
                    gameLoopState = GAME_IN_TRANSITION;
                    setTimeout(&endTransition, 0, 1000);
                }

                // Transition to game over
                if (gameState.lives == 0) {
                    // TODO: Create a GAME OVER screen
                    nextScreen = SCREEN_MENU;
                    gameLoopState = GAME_IN_TRANSITION;
                    setTimeout(&endTransition, 0, 1000);
                }
        }

        waitVSync();
        dumpBuffer();  // Dump backbuffer to video memory
    }

    // CLEANUP
    running = 0x0;
    clearAllTimeouts();
    destroyAllEntities();
    clearScreen();
    endLevel();

    waitVSync();
    dumpBuffer();

    return nextScreen;
}