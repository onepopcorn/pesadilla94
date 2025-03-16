#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <mikmod.h>

#include "resources.h"

/**
 * Private. Open given filePath and store its data in memory
 *
 * This function shouldn't be called outside this module
 *
 * @returns Pointer to memory allocated to keep the file contents
 *
 */
static void *loadResource(const char *filePath, uint8_t resourceType) {
    FILE *file = fopen(filePath, "rb");

    if (!file) {
        perror("Error opening asset files");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (resourceType == RES_TYPE_SPRITE) {
        Sprite *sprite = (Sprite *)malloc(fileSize);
        if (!sprite) {
            perror("Not enough memory to open sprite resource");
            fclose(file);
            return NULL;
        }

        fread(sprite, fileSize, 1, file);
        fclose(file);
        return (void *)sprite;
    }

    if (resourceType == RES_TYPE_MAP) {
        Map *map = (Map *)malloc(fileSize);
        if (!map) {
            perror("Not enough memory to open map resource");
            fclose(file);
            return NULL;
        }

        fread(map, fileSize, 1, file);
        fclose(file);
        return (void *)map;
    }

    // if (resourceType == RES_TYPE_SOUND) {
    //     SAMPLE *sfx = (SAMPLE *)malloc(fileSize);
    //     if (!sfx) {
    //         perror("Not enough memory to open sfx resource");
    //         fclose(file);
    //         return NULL;
    //     }

    //     fread(sfx, fileSize, 1, file);
    //     fclose(file);
    //     return (void *)sfx;
    // }

    fclose(file);
    return NULL;
}

/**
 * Load resource on given file path
 *
 * @returns Pointer to loaded Sprite data
 */
Sprite *loadSprite(const char *filePath) {
    Sprite *sprite = loadResource(filePath, RES_TYPE_SPRITE);
    return sprite;
}

/**
 * Load Map resource on given file path
 *
 * @returns Pointer to loaded Tilemap data
 */
Map *loadMap(const char *filePath) {
    Map *map = loadResource(filePath, RES_TYPE_MAP);
    return map;
}

// SAMPLE *loadSfx(const char *filePath) {
//     SAMPLE *sfx = loadResource(filePath, RES_TYPE_SOUND);
//     return sfx;
// }