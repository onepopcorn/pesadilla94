#include <stdlib.h>

#include "io/resources.h"
#include "entities.h"
#include "physics/collisions.h"
#include "whip.h"
#include "animations.h"

Sprite *whipSprite;
void whipInit() {
    whipSprite = loadSprite("whip.spr");
}

Entity *whipSpawn(int x, int y, bool facingRight) {
    Entity *entity = createEntity(x, y, TYPE_PLAYER_BULLET, whipSprite, whipUpdate);
    if (!entity) return NULL;

    entity->animation = ANIM_WHIP;
    entity->frame = 0;
    entity->hitbox = (Rect){1, 1, 1, 1};

    if (!facingRight) {
        entity->flags |= ENTITY_FLIP;
    }

    return entity;
}

void whipUpdate(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    // Last frame makes hitbox bigger
    if (entity->frame == 2) {
        entity->hitbox = (Rect){whipSprite->width / 4, 4, whipSprite->width / 2, whipSprite->height - 4};
    }

    if (entity->frame == ANIM_WHIP_LEN - 2) {
        entity->flags &= ~ENTITY_ALIVE;
    }
}

void whipFree() {
    free(whipSprite);
    whipSprite = NULL;
}