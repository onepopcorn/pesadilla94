#include "../vendor/bucket-lib/bucket-lib.h"
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
static void* loadResource(Arena* arena, const char* filePath) {
    FILE* file = fopen(filePath, "rb");

    if (!file) {
        perror("Error opening asset files");
        goto error;
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    void* resource = arena_alloc(arena, fileSize);
    if (!resource) {
        perror("Failed to load resource");
        goto error;
    }

    fread(resource, fileSize, 1, file);
    fclose(file);
    return resource;

error:
    if (file) fclose(file);
    return NULL;
}

/**
 * Load resource on given file path
 *
 * @returns Pointer to loaded Sprite data
 */
Sprite* loadSprite(Arena* arena, const char* filePath) {
    return (Sprite*)loadResource(arena, filePath);
}

/**
 * Load Map resource on given file path
 *
 * @returns Pointer to loaded Tilemap data
 */
Map* loadMap(Arena* arena, const char* filePath) {
    return (Map*)loadResource(arena, filePath);
}

// SAMPLE *loadSfx(Arena* arena, const char *filePath) {
//     return (SAMPLE *)loadResource(filePath, RES_TYPE_SOUND);
// }