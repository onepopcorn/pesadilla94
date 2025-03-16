#include <stdint.h>

#include "screens.h"
#include "gameState.h"

#include "gameover.h"

enum Screen gameover() {
    resetGameState();
    return SCREEN_MENU;
}