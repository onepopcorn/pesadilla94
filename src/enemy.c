#include <stdlib.h>
#include <stdint.h>
#include "io/resources.h"
#include "entities.h"
#include "enemy.h"
#include "animations.h"

#define ENEMY_SPEED 0.6

struct Entity *enemyInit(int x, int y) {
    Sprite *resource = loadSprite("fede.spr");
    struct Entity *enemy = createEntity(x, y, TYPE_ENEMY_A, resource, enemyUpdate);
    enemy->animation = PLAYER_ANIM_WALK;
    enemy->frame = PLAYER_ANIM_WALK;
    enemy->vx = 1 * ENEMY_SPEED;
    enemy->hitbox = (Rect){resource->width / 4, 0, resource->width / 2, resource->height};
    return enemy;
}

void enemyUpdate(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    if ((tileCollisions & 0x01) > 0) {
        entity->vx = 1 * ENEMY_SPEED;
    } else if ((tileCollisions & 0x02) > 0) {
        entity->vx = -1 * ENEMY_SPEED;
    }

    // if (abs(entity->y - player->y) > 5) {
    //     // in patrol mode
    //     if (entity->x > 150) {
    //         entity->vx = -1 * ENEMY_SPEED;
    //     } else if (entity->x <= 20) {
    //         entity->vx = 1 * ENEMY_SPEED;
    //     }
    // } else {
    //     // In pursuit mode
    //     if (player->x > entity->x) {
    //         entity->vx = 1 * ENEMY_SPEED;
    //     } else if (player->x < entity->x) {
    //         entity->vx = -1 * ENEMY_SPEED;
    //     } else {
    //         entity->vx = 0;
    //     }
    // }

    // Flip sprite when needed
    if (entity->vx > 0) {
        entity->flags &= ~ENTITY_FLIP;
    } else {
        entity->flags |= ENTITY_FLIP;
    }
}

void enemyFree() {
}