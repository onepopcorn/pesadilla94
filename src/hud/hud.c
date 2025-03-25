#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "gameState.h"
#include "render/video.h"
#include "assets.h"
#include "text.h"
#include "hud.h"

#define HUD_Y 5
#define HUD_LVL_Y HUD_Y + 22
#define HUD_X_LIVES 5
#define HUD_X_DOORS 60
#define HUD_X_TIME 116
#define HUD_X_HAND 188
#define HUD_X_SCORE 320 - (STR_HUD_SCORE_LEN + 1) * HUD_FONT_W  // One char right padding

#define HUD_FONT_W 8
#define HUD_FONT_H 8
#define m_clearRect(x, y, chars) \
    drawRectColor((Rect){x, y, chars * HUD_FONT_W, HUD_FONT_H}, COLOR_BLACK);

void drawHUD() {
    // TODO: Consider paitign a background for the HUD section

    // draw initial lives
    drawSprite(HUD_X_LIVES, HUD_Y, miscSprite, 1, true, COLOR_TRANSPARENT);
    updateLives(0);

    // draw doors left
    drawSprite(HUD_X_DOORS, HUD_Y, miscSprite, 2, false, COLOR_TRANSPARENT);
    updateDoors(0);

    // draw initial time
    drawSprite(HUD_X_TIME, HUD_Y, miscSprite, 0, false, COLOR_TRANSPARENT);
    updateTime(0);

    // draw sticky hand. Starts deactivated
    updateWeapon(true);

    // draw initial score
    drawText(HUD_X_SCORE, HUD_Y, font, STR_HUD_SCORE, COLOR_TRANSPARENT, 0);
    updatePoints(0);

    // draw current level
    drawText(HUD_X_LIVES, HUD_LVL_Y, font, STR_HUD_LEVEL, COLOR_TRANSPARENT, 0);
    updateLevel();
}

void updatePoints(int8_t points) {
    gameState.score += points;

    char buffer[6] = "00000";
    itoa(gameState.score, buffer, 10);
    uint8_t len = strlen(buffer);

    m_clearRect(312 - len * HUD_FONT_W, HUD_Y + 10, len);
    drawText(312 - len * HUD_FONT_W, HUD_Y + 10, font, buffer, COLOR_TRANSPARENT, 0);
}

void updateLives(int8_t amount) {
    gameState.lives += amount;
    m_clearRect(HUD_X_LIVES + 16, HUD_Y + 7, 4);

    char buffer[5] = {'X', '0' + gameState.lives, '\0'};
    drawText(HUD_X_LIVES + 16, HUD_Y + 7, font, buffer, COLOR_TRANSPARENT, 0);
}

void updateDoors(int8_t amount) {
    gameState.doorsLeft += amount;
    m_clearRect(HUD_X_DOORS + 20, HUD_Y + 7, 4);

    char buffer[5];
    buffer[0] = 'X';
    itoa(gameState.doorsLeft, &buffer[1], 10);
    drawText(HUD_X_DOORS + 20, HUD_Y + 7, font, buffer, COLOR_TRANSPARENT, 0);
}

// TODO: Consider using BCD to prevent using modulo and division operations
void updateTime(int8_t time) {
    gameState.timeLeft += time;
    m_clearRect(HUD_X_TIME + 16, HUD_Y + 7, 6);

    uint8_t minutes = gameState.timeLeft / 60;
    uint8_t seconds = gameState.timeLeft % 60;

    char buffer[6];  // MM:SS\0
    buffer[0] = '0' + (minutes / 10);
    buffer[1] = '0' + (minutes % 10);
    buffer[2] = ':';
    buffer[3] = '0' + (seconds / 10);
    buffer[4] = '0' + (seconds % 10);
    buffer[5] = '\0';

    drawText(HUD_X_TIME + 16, HUD_Y + 7, font, buffer, COLOR_TRANSPARENT, 0);
}

void updateWeapon(char deactivated) {
    uint8_t frame = 3 + deactivated * 2;
    drawSprite(HUD_X_HAND, HUD_Y, miscSprite, frame, false, COLOR_TRANSPARENT);
    drawSprite(HUD_X_HAND + 16, HUD_Y, miscSprite, frame + 1, false, COLOR_TRANSPARENT);
}

void updateLevel() {
    uint8_t xPos = HUD_X_LIVES - 4 + sizeof(STR_HUD_LEVEL) * 8;
    m_clearRect(xPos, HUD_LVL_Y, 2);

    char buffer[6];
    sprintf(buffer, "%d", gameState.level + 1);
    drawText(xPos, HUD_LVL_Y, font, buffer, COLOR_TRANSPARENT, 0);
}