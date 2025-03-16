#include <stdlib.h>
#include <stdio.h>

#include "io/resources.h"
#include "assets.h"

Sprite *font;
Sprite *playerSprite;
Sprite *enemySprite;
Sprite *whipSprite;
Sprite *tileset;
Sprite *miscSprite;

uint8_t
assetsInit() {
    // Fonts
    font = loadSprite("font.fnt");
    if (!font) {
        perror("Error opening font.fnt file");
        return EXIT_FAILURE;
    }

    // Sprites
    playerSprite = loadSprite("fede.spr");
    if (!playerSprite) {
        perror("Error opening fede.spr file");
        return EXIT_FAILURE;
    }

    enemySprite = loadSprite("enemy1.spr");
    if (!enemySprite) {
        perror("Error opening enemy1.spr file");
        return EXIT_FAILURE;
    }

    whipSprite = loadSprite("whip.spr");
    if (!whipSprite) {
        perror("Error opening whip.spr file");
        return EXIT_FAILURE;
    }

    tileset = loadSprite("tiles.spr");
    if (!tileset) {
        perror("Error opening tiles.spr");
        return EXIT_FAILURE;
    }

    miscSprite = loadSprite("misc.spr");
    if (!miscSprite) {
        perror("Error opening misc.spr");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void assetsFree() {
    // Fonts
    free(font);
    font = NULL;

    // Sprites
    free(playerSprite);
    playerSprite = NULL;

    free(enemySprite);
    enemySprite = NULL;

    free(whipSprite);
    whipSprite = NULL;

    free(tileset);
    tileset = NULL;

    free(miscSprite);
    miscSprite = NULL;
}