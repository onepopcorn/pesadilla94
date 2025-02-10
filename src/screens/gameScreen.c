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

#include "gameScreen.h"

enum Screen gameScreen() {
    // Set level, player and enemies
    // TODO: Add checks when failing loading the level
    startLevel(0);

    // draw map
    drawMap();

    // TODO: Make this correctly per map or during game play
    playerSpawn(20, 59);
    enemySpawn(100, 111);
    enemySpawn(160, 63);
    enemySpawn(130, 158);
    enemySpawn(200, 111);

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
    }

    // Clean entities
    destroyAllEntities();

    // Clear screen
    endLevel();
    clearScreen();

    return SCREEN_MENU;
}