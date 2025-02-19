#include <stdint.h>
#include "render/video.h"
#include "io/resources.h"
#include "io/keyboard.h"
#include "assets.h"
#include "render/hud.h"
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

enum Screen nextScreen = SCREEN_MENU;
uint8_t loopState = 0x00;

// PRIVATE METHODS

void endTransition() {
    m_unsetFlag(loopState, GAME_RUNNING);
}

// PUBLIC METHODS

enum Screen game() {
    // BOOSTRAP

    // TODO: Add checks when failing loading the level
    gameState.doorsLeft = startLevel(gameState.level);
    drawMap();

    // TODO: Make this correctly per map or during game play
    playerSpawn(20, 59);
    enemySpawn(100, 111);
    enemySpawn(160, 63);
    enemySpawn(130, 158);
    enemySpawn(200, 111);

    uint32_t previousTime = getMilliseconds();

    m_setFlag(loopState, GAME_RUNNING);

    // UPDATE
    while (m_isFlagSet(loopState, GAME_RUNNING)) {
        if (isKeyJustPressed(KEY_ESC)) {
            nextScreen = SCREEN_MENU;
            m_unsetFlag(loopState, GAME_RUNNING);
            continue;
        }

        if (isKeyJustPressed(KEY_P)) {
            m_toggleFlag(loopState, GAME_PAUSED);
            previousTime = getMilliseconds();
        }

        if (gameState.doorsLeft == 0 && !m_isFlagSet(loopState, GAME_TRANSITION)) {
            nextScreen = SCREEN_GAME;
            m_setFlag(loopState, GAME_TRANSITION);
            setTimeout(endTransition, 1000);
        }

        if (gameState.lives <= 0 && !m_isFlagSet(loopState, GAME_TRANSITION)) {
            nextScreen = SCREEN_INTRO;
            m_setFlag(loopState, GAME_TRANSITION);
            setTimeout(endTransition, 3000);
        }

        // Prevent game logic to run when paused
        if (m_isFlagSet(loopState, (GAME_PAUSED | GAME_TRANSITION))) continue;

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
    }

    // CLEANUP
    loopState = 0x0;
    clearAllTimeouts();
    destroyAllEntities();
    clearScreen();
    endLevel();

    return nextScreen;
}