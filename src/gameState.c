#include <stdint.h>

#include "settings/settings.h"
#include "gameState.h"

GameState gameState = {MAX_LIVES, 0, 0, 0};

void resetGameState() {
    gameState.lives = MAX_LIVES;
    gameState.score = 0;
    gameState.level = 0;
}