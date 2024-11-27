#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "geom.h"
#include "render/video.h"
#include "map.h"
#include "animations.h"

#include "entities.h"

#define MAX_ENTITIES 10

struct Entity entities[MAX_ENTITIES] = {0};
int lastEntity = 0;

struct Entity* createEntity(int x, int y, uint8_t type, Sprite* spr, void (*update)(struct Entity* entity, struct Entity* player)) {
    // Check if there's not enough space for more entities
    if (&entities[lastEntity] == &entities[MAX_ENTITIES]) return NULL;

    // Initialize entity
    entities[lastEntity].x = x;
    entities[lastEntity].y = y;
    entities[lastEntity].sprite = spr;
    entities[lastEntity].flags = (type << 4) | ENTITY_ALIVE;  // Type is stored in high nibble, flags in low nibble
    entities[lastEntity].update = update;

    struct Entity* entity = &entities[lastEntity];
    lastEntity++;
    return entity;
}

/**
 * Update all entities
 *
 * Should:
 * - Check input for player entity
 * - Update other entities based on behavior/state/logic
 * - Check physics and boundaries
 * - Check collisions
 * - Render entities to avoid looping through all entities twice??? Not sure about this one
 *   - having a separete render loop allows to render them backwards
 *   - render them here avoids looping through all entities twice
 *
 */
// static uint32_t accumulated_time = 0;
void updateEntities(uint32_t delta) {
    // Player is always entity 0
    struct Entity* entity = &entities[lastEntity - 1];

    static uint32_t accumulated_time = 0;

    // Update player logic

    // Handle animations with a global counter
    accumulated_time += delta;
    char frameNeedsUpdate = false;

    if (accumulated_time >= 150) {
        accumulated_time -= 150;  // Reset accumulator
        frameNeedsUpdate = true;
    }

    while (entity->flags & ENTITY_ALIVE) {
        // update each entity logic
        entity->update(entity, &entities[0]);

        // Move this logic inside the update function (?)
        entity->prev_x = entity->x;
        entity->prev_y = entity->y;

        // Move entity using delta time for smooth animations
        entity->x = entity->x + entity->vx * 70 * delta / 1000;  // Move 10 pixels/second
        entity->y = entity->y + entity->vy * 70 * delta / 1000;  // Move 10 pixels/second

        markTouchedTiles((Rect){entity->prev_x, entity->prev_y, entity->sprite->width, entity->sprite->height});

        int idx = getAnimationFrame(frameNeedsUpdate, entity);
        drawSprite(entity->x, entity->y, entity->sprite, idx, entity->flags & ENTITY_FLIP);

        entity--;
    }
}

int getAnimationFrame(char nextFrame, struct Entity* entity) {
    int animationFrame = Animations[entity->frame];
    if (!nextFrame) return animationFrame;

    entity->frame++;
    animationFrame = Animations[entity->frame];

    if (animationFrame < 0) {
        entity->frame = entity->animation;
        animationFrame = Animations[entity->frame];
    }

    return animationFrame;
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