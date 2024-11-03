#include <stdlib.h>
#include <stdbool.h>

#include "geom.h"
#include "render/video.h"
#include "io/keyboard.h"
#include "player.h"

Player* player;
uint8_t* clearRect;

void playerInit(int x, int y, Sprite* sprite) {
    // TODO: Use entities and allocate all of them at the same time instead of this ugly shit
    clearRect = (uint8_t*)malloc(sizeof(uint8_t) * sprite->height * sprite->width);
    player = malloc(sizeof(Player));

    player->x = x;
    player->y = y;
    player->vx = 0;
    player->vy = 0;
    player->prev_x = x;
    player->prev_y = y;
    player->counter = 0;
    player->frame = 0;
    player->flip = false;
    player->sprite = sprite;

    getBufferData((Rect){player->x, player->y, player->sprite->width, player->sprite->height}, clearRect);
}

void playerUpdate() {
    if (keys[KEY_RIGHT]) {
        player->vx = 1;
        player->flip = false;
    } else if (keys[KEY_LEFT]) {
        player->vx = -1;
        player->flip = true;
    } else {
        player->vx = 0;
    }

    if (keys[KEY_UP]) {
        player->vy = -1;
    } else if (keys[KEY_DOWN]) {
        player->vy = 1;
    } else {
        player->vy = 0;
    }

    player->prev_x = player->x;
    player->prev_y = player->y;

    player->x = player->x + player->vx;
    player->y = player->y + player->vy;

    if (player->x >= SCREEN_WIDTH - player->sprite->width) player->x = SCREEN_WIDTH - player->sprite->width;
    if (player->x <= 0) player->x = 0;
    if (player->y >= SCREEN_HEIGHT - player->sprite->height) player->y = SCREEN_HEIGHT - player->sprite->height;
    if (player->y <= 0) player->y = 0;

    //
    if (++player->counter >= 12) {
        player->frame = player->frame < 1 ? player->frame + 1 : 0;
        player->counter = 0;
    };
}

void playerRender() {
    // Clean previous position
    drawRect((Rect){player->prev_x, player->prev_y, player->sprite->width, player->sprite->height}, clearRect);

    // Store current data
    getBufferData((Rect){player->x, player->y, player->sprite->width, player->sprite->height}, clearRect);
    drawSprite(player->x, player->y, player->sprite, player->frame, player->flip);
}

void playerFree() {
    free(player);
    player = NULL;

    free(clearRect);
    clearRect = NULL;
}