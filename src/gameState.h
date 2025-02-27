#ifndef _GAME_STATE_H
#define _GAME_STATE_H

typedef struct GameState {
    uint8_t lives;
    uint8_t doorsLeft;
    uint8_t level;
    uint32_t score;
} GameState;

extern GameState gameState;

#endif  // _GAME_STATE_H