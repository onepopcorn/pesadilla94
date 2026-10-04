#ifndef _GAME_STATE_H
#define _GAME_STATE_H

typedef struct GameState {
    int lives;
    int doorsLeft;
    int level;
    int score;
    int timeLeft;
} GameState;

extern GameState gameState;

void resetGameState();

#endif  // _GAME_STATE_H