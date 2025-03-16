#ifndef _GAME_STATE_H
#define _GAME_STATE_H

typedef struct GameState {
    uint8_t lives;
    uint8_t doorsLeft;
    uint8_t level;
    uint16_t score;
    uint8_t timeLeft;
} GameState;

extern GameState gameState;

void resetGameState();

#endif  // _GAME_STATE_H