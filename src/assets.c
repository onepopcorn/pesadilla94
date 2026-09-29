#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "assets.h"

Sprite* font;
Sprite* playerSprite;
Sprite* enemySprite;
Sprite* whipSprite;
Sprite* tileset;
Sprite* miscSprite;

uint8_t
assetsInit(Arena* arena) {
    // Fonts
    font = loadSprite(arena, "font.fnt");
    if (!font) {
        perror("Error opening font.fnt file");
        return EXIT_FAILURE;
    }

    // Sprites
    playerSprite = loadSprite(arena, "fede.spr");
    if (!playerSprite) {
        perror("Error opening fede.spr file");
        return EXIT_FAILURE;
    }

    enemySprite = loadSprite(arena, "enemy1.spr");
    if (!enemySprite) {
        perror("Error opening enemy1.spr file");
        return EXIT_FAILURE;
    }

    whipSprite = loadSprite(arena, "whip.spr");
    if (!whipSprite) {
        perror("Error opening whip.spr file");
        return EXIT_FAILURE;
    }

    tileset = loadSprite(arena, "tiles.spr");
    if (!tileset) {
        perror("Error opening tiles.spr");
        return EXIT_FAILURE;
    }

    miscSprite = loadSprite(arena, "misc.spr");
    if (!miscSprite) {
        perror("Error opening misc.spr");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}