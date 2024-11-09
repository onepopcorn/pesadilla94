#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "geom.h"
#include "io/resources.h"
#include "io/keyboard.h"
#include "render/video.h"
#include "map.h"

#include "entities.h"

#define MAX_ENTITIES 10

struct Entity entities[MAX_ENTITIES] = {0};
int lastEntity = 0;

struct Entity* createEntity(int x, int y, uint8_t type, Sprite* spr) {
    // Check if there's not enough space for more entities
    if (&entities[lastEntity] == &entities[MAX_ENTITIES]) return NULL;

    entities[lastEntity].x = x;
    entities[lastEntity].y = y;
    entities[lastEntity].sprite = spr;
    entities[lastEntity].flags = (type << 4) | ENTITY_ALIVE;  // Type is stored in high nibble, flags in low nibble

    struct Entity* entity = &entities[lastEntity];
    lastEntity++;
    return entity;
}

void renderEntities() {
    struct Entity* entity = entities;

    // TODO: Clean background in an efficient way
    while (entity->flags & ENTITY_ALIVE) {
        // restore background
        if (entity->prev_x != entity->x || entity->prev_y != entity->y) {
            restoreBackground((Rect){entity->prev_x, entity->prev_y, entity->sprite->width, entity->sprite->height});
        }

#ifdef DEBUG
        restoreBackgroundDebug((Rect){entity->x, entity->y, entity->sprite->width, entity->sprite->height});
#endif

        drawSprite(entity->x, entity->y, entity->sprite, entity->frame, entity->flags & ENTITY_FLIP);

        // Use counter interrupts for frame animation
        // entity->frame++;
        // if (entity->frame == entity->sprite->frames) entity->frame = 0;

        entity++;
    }
}

void updateEntities() {
    // Player is always entity 0
    struct Entity* entity = entities;

    /**
     * The game is single player so no need to read input for each entity
     * Also, first entity is ALWAYS the player
     */
    if ((entity->flags >> 4) == TYPE_PLAYER) {
        if (keys[KEY_RIGHT]) {
            entity->vx = 1;
            entity->flags = entity->flags & (0xFF ^ ENTITY_FLIP);
        } else if (keys[KEY_LEFT]) {
            entity->vx = -1;
            entity->flags = entity->flags | ENTITY_FLIP;
        } else {
            entity->vx = 0;
        }

        if (keys[KEY_UP]) {
            entity->vy = -1;
        } else if (keys[KEY_DOWN]) {
            entity->vy = 1;
        } else {
            entity->vy = 0;
        }
    }

    /**
     * Update other entities logic here
     *
     */

    while (entity->flags & ENTITY_ALIVE) {
        entity->prev_x = entity->x;
        entity->prev_y = entity->y;

        entity->x = entity->x + entity->vx;
        entity->y = entity->y + entity->vy;

        entity++;
    }
}

void destroyEntity(uint8_t index) {
    // Move destroyed entity to the last position
    struct Entity entity = entities[index];

    entities[index] = entities[lastEntity];
    memset(&entity, 0, sizeof(Entitiy));
    entities[lastEntity] = entity;

    // Update last entity inde
    lastEntity--;
}
/**
 * EXAMPLE
 *
 */
// #include <stdio.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>

// #define uint8_t unsigned char

// #define TYPE_PLAYER    0x01
// #define TYPE_ENEMY_A   0x02
// #define TYPE_ENEMY_B   0x03
// #define TYPE_DOOR      0x04
// #define TYPE_BULLET    0x05

// #define ENTITY_FLIP    0b00000001
// #define ENTITY_DEAD    0b00000010
// #define ENTITY_FLYING  0b00000100
// #define ENTITY_LOCKED  0b00001000

// uint8_t entity = 0b01000101;

// int main() {
//     uint8_t flags = entity & 0x0f;
//     uint8_t type = entity >> 4;

//     printf("Entity flags %d\n", flags);
//     printf("\n");

//     printf("Entity %d\n", flags & ENTITY_FLYING);

//     printf("Entity dead: %s\n" , (flags & ENTITY_DEAD) > 0 ? "true": "false");
//     printf("Entity flip: %s\n" , (flags & ENTITY_FLIP) > 0 ? "true" : "false");
//     printf("Entity flying %s\n", (flags & ENTITY_FLYING) > 0 ? "true": "false");

//     char *typeStr = malloc(sizeof(char) * 20);
//     strcpy(typeStr, "undefined");

//     if(type == TYPE_PLAYER) strcpy(typeStr, "player");
//     else if(type == TYPE_ENEMY_A) strcpy(typeStr, "enemy a");
//     else if(type == TYPE_ENEMY_B) strcpy(typeStr, "enemy b");
//     else if(type == TYPE_DOOR) strcpy(typeStr, "door");

//     printf("\n");
//     printf("Entity of type %s\n", typeStr);

//     free(typeStr);
//     typeStr = NULL;
//     return 0;
// }