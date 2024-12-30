#include <stdlib.h>
#include <stdint.h>

#include "io/resources.h"
#include "io/keyboard.h"
#include "entities.h"
#include "player.h"
#include "animations.h"

#define PLAYER_SPEED 0.8

struct Entity *player;

struct Entity *playerInit(int x, int y) {
    // should load the player sprites
    Sprite *resource = loadSprite("fede.spr");

    // should create the player entity
    player = createEntity(x, y, TYPE_PLAYER, resource, updatePlayer);

    // set hitbox
    player->hitbox = (Rect){resource->width / 4, 6, resource->width / 2, resource->height - 4};

    // should set the player initial stats (position, lives, score...)
    player->animation = PLAYER_ANIM_IDLE;
    player->frame = PLAYER_ANIM_IDLE;
    return player;
}

void updatePlayer(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    //  TODO: Use screen & map limits

    // Update position basde on input
    if (keys[KEY_RIGHT]) {
        entity->vx = PLAYER_SPEED;
        entity->flags &= ~ENTITY_FLIP;
    } else if (keys[KEY_LEFT]) {
        entity->vx = -PLAYER_SPEED;
        entity->flags |= ENTITY_FLIP;
    } else {
        entity->vx = 0;
    }

    // Limit movement when going right and colliding with a wall of type 2
    if (entity->vx > 0 && (tileCollisions & 0x02) > 0) {
        entity->vx = 0;
        // Limit movement when going left and colliding with a wall of type 1
    } else if (entity->vx < 0 && (tileCollisions & 0x01) > 0) {
        entity->vx = 0;
    }

    if (keys[KEY_UP]) {
        entity->vy = -PLAYER_SPEED;
    } else if (keys[KEY_DOWN]) {
        entity->vy = PLAYER_SPEED;
    } else {
        entity->vy = 0;
    }

    // Update animation
    const int currentAnimation = entity->animation;

    if (entity->vx == 0) {
        entity->animation = PLAYER_ANIM_IDLE;
    } else {
        entity->animation = PLAYER_ANIM_WALK;
    }

    // Reset current frame on animation change
    if (currentAnimation != entity->animation) {
        entity->frame = entity->animation;
    }
}

void playerFree() {
    free(player->sprite);
    player = NULL;
}