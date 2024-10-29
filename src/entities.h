#ifndef _ENTITIES_H
#define _ENTITIES_H

/**
 * Entity flags
 * 
 * First nibble used for flags
 * Second nibble used for entity type
 * 
 * bit 0 | sprite flip
 * bit 1 | alive
 * bit 2 | flying
 * ...
 * bit 4 | entity type
 * bit 5 |
 * bit 6 |
 * bit 7 |
 *  
 */

// Use hight nibble for entity type. Up to 16 types
#define TYPE_PLAYER    0x01
#define TYPE_ENEMY_A   0x02
#define TYPE_ENEMY_B   0x03
#define TYPE_DOOR      0x04
#define TYPE_BULLET    0x05

// Use lower nibble for flags. Limited to 4. Consider if we need the whole byte for flags
#define ENTITY_FLIP    0x01 // bit 0  000000001
#define ENTITY_ALIVE   0x02 // bit 1  00000010
#define ENTITY_FLYING  0x04 // bit 2  00000100
#define ENTITY_LOCKED  0x08 // bit 3  00001000

typedef struct Entity {
    int x,y;
    int vx, vy;
    int prev_x, prev_y;
    int frame;
    uint8_t flags;
    Sprite * sprite;
} Entitiy;

struct Entity* createEntity(int x, int y, uint8_t type, Sprite* spr);
void destroyEntity(uint8_t index);
void updateEntities();
void renderEntities();
#endif