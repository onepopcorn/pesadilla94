#ifndef _ENTITIES_H
#define _ENTITIES_H

#include <stdint.h>
#include "settings/settings.h"
#include "physics/geom.h"

/**
 * Entity types
 *
 */
#define TYPE_PLAYER 0x01
#define TYPE_ENEMY_A 0x02
#define TYPE_ENEMY_B 0x03
#define TYPE_PLAYER_BULLET 0x04
#define TYPE_ENEMY_BULLET 0x05

/**
 * Entity flags
 *
 */

// Common
#define ENTITY_FLIP 0x01             // bit 0  00000001
#define ENTITY_ALIVE 0x02            // bit 1  00000010
#define ENTITY_CHECK_COLLISION 0x04  // bit 2  00000100
#define ENTITY_FLASHING 0x08         // bit 3  00001000
#define ENTITY_BLOCKED 0x10          // bit 4  00010000
#define ENTITY_INVULNERABLE 0x20     // bit 5  00100000
#define ENTITY_STUNT 0x40            // bit 6  01000000

#define m_getEntityRect(entity) \
    (Rect) { entity->x + entity->hitbox.x, entity->y + entity->hitbox.y, entity->hitbox.w, entity->hitbox.h }

struct Sprite;

typedef struct Entity {
    float x, y;
    float vx, vy;
    Rect hitbox;
    uint8_t animation;
    uint8_t frame;
    uint8_t type;
    uint8_t flags;
    void (*update)(struct Entity* entity, struct Entity* player, uint8_t tileCollisions);
    struct Sprite* sprite;
} Entity;

struct Entity* createEntity(int x, int y, uint8_t type, struct Sprite* spr, void (*update)(struct Entity* entity, struct Entity* player, uint8_t tileCollisions));

void destroyEntity(uint8_t index);

void destroyAllEntities();

void updateEntities(uint32_t delta);

extern struct Entity entities[MAX_ENTITIES];

#endif  // _ENTITIES_H