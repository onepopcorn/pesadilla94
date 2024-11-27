#include <stdlib.h>
#include "io/resources.h"
#include "entities.h"
#include "enemy.h"
#include "animations.h"

struct Entity *enemyInit(int x, int y) {
    Sprite *resource = loadSprite("fede.spr");
    struct Entity *enemy = createEntity(x, y, TYPE_ENEMY_A, resource, enemyUpdate);
    enemy->animation = PLAYER_ANIM_WALK;
    enemy->frame = PLAYER_ANIM_WALK;
    enemy->vx = 1;
    return enemy;
}

void enemyUpdate(struct Entity *entity, struct Entity *player) {
    if (abs(entity->y - player->y) > 5) {
        // in patrol mode
        if (entity->x > 150) {
            entity->vx = -1;
        } else if (entity->x <= 20) {
            entity->vx = 1;
        }
    } else {
        // In pursuit mode
        if (player->x > entity->x) {
            entity->vx = 1;
        } else if (player->x < entity->x) {
            entity->vx = -1;
        } else {
            entity->vx = 0;
        }
    }

    // Flip sprite when needed
    if (entity->vx == 1) {
        entity->flags = entity->flags & (0xFF ^ ENTITY_FLIP);
    } else if (entity->vx == -1) {
        entity->flags = entity->flags | ENTITY_FLIP;
    }
}

void enemyFree() {
}