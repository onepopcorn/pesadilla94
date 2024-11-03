#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct Player {
    int x, y;
    int vx, vy;
    int prev_x, prev_y;
    int counter;
    int frame;
    bool flip;
    Sprite* sprite;
} Player;

void playerInit(int x, int y, Sprite* sprite);
void playerUpdate();
void playerRender();
void playerFree();

#endif