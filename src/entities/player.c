
#include "io/resources.h"
#include "io/keyboard.h"
#include "entities/entities.h"
#include "physics/collisions.h"
#include "render/video.h"
#include "assets.h"
#include "animations.h"
#include "map.h"
#include "whip.h"
#include "macros.h"

#include "player.h"

#define PLAYER_SPEED 0.9

Entity *player;

uint8_t playerActions = 0;
uint8_t counter = 0;
uint8_t shootTimer = 0;

Entity *playerSpawn(int x, int y) {
    // should create the player entity
    player = createEntity(x, y, TYPE_PLAYER, playerSprite, playerUpdate);

    // set hitbox
    player->hitbox = (Rect){playerSprite->width / 4, 4, playerSprite->width / 2, playerSprite->height - 4};

    // should set the player initial stats (position, lives, score...)
    player->animation = ANIM_PLAYER_IDLE;
    player->frame = 0;
    return player;
}

void setAnimation() {
    if (m_isFlagSet(playerActions, PLAYER_SEARCHING)) {
        player->animation = ANIM_PLAYER_SEARCH;
    } else if (m_isFlagSet(playerActions, PLAYER_ON_STAIRS)) {
        player->animation = ANIM_PLAYER_STAIRS;
    } else if (player->vx != 0) {
        player->animation = PLAYER_WALK;
    } else {
        player->animation = ANIM_PLAYER_IDLE;
    }
}

void playerUpdate(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    // MOVEMENT
    bool canMove = !m_isFlagSet(playerActions, (PLAYER_DYING | PLAYER_ON_STAIRS | PLAYER_SHOOTING | PLAYER_SEARCHING));
    if (keys[KEY_RIGHT] && !keys[KEY_LEFT] && !keys[KEY_UP] && canMove) {
        entity->vx = PLAYER_SPEED;
        m_unsetFlag(entity->flags, ENTITY_FLIP);
    } else if (keys[KEY_LEFT] && !keys[KEY_RIGHT] && !keys[KEY_UP] && canMove) {
        entity->vx = -PLAYER_SPEED;
        m_setFlag(entity->flags, ENTITY_FLIP);
    } else {
        entity->vx = 0;
    }

    // STOP ON WALLS
    if (entity->vx > 0 && m_isFlagSet(tileCollisions, COLLISION_WALL_R)) {
        entity->vx = 0;
    } else if (entity->vx < 0 && m_isFlagSet(tileCollisions, COLLISION_WALL_L)) {
        entity->vx = 0;
    }

    // USE STAIRS
    if (m_isFlagSet(tileCollisions, COLLISION_STAIRS) && !m_isFlagSet(playerActions, PLAYER_ON_STAIRS)) {
        if (keys[KEY_UP] || keys[KEY_DOWN]) {
            Vec2 pos = getStairsDestination(entity->x + entity->sprite->width * 0.5, entity->y + entity->sprite->height, keys[KEY_UP]);
            if (pos.x > 0 && pos.y > 0) {
                entity->y = pos.y - entity->sprite->height / 2 + 1;  // +1 because all characters are "closer" to camera to force perspective a bit
                entity->x = pos.x;
                m_setFlag(entity->flags, (ENTITY_BLOCKED | ENTITY_INVULNERABLE));
                m_unsetFlag(entity->flags, ENTITY_FLIP);
                m_setFlag(playerActions, PLAYER_ON_STAIRS);
            }
        }
    }

    // SHOOTING
    if (keys[KEY_SPACE] && !keys[KEY_UP] && !m_isFlagSet(playerActions, PLAYER_SHOOTING) && shootTimer == 0) {
        shootTimer = 50;
        bool facingRight = !m_isFlagSet(entity->flags, ENTITY_FLIP);
        int x = facingRight ? entity->x + entity->sprite->width * 0.5 : entity->x - entity->sprite->width;
        whipSpawn(x, entity->y - 5, facingRight);
        m_setFlag(playerActions, PLAYER_SHOOTING);
        m_setFlag(entity->flags, ENTITY_BLOCKED);
    }

    if (m_isFlagSet(playerActions, PLAYER_SHOOTING)) {
        if (--shootTimer == 0) {
            m_unsetFlag(entity->flags, ENTITY_BLOCKED);
            m_unsetFlag(playerActions, PLAYER_SHOOTING);
        };
    }

    // SEARCHING DOORS
    if (m_isFlagSet(tileCollisions, COLLISION_DOOR) && !m_isFlagSet(playerActions, PLAYER_SHOOTING)) {
        if (keys[KEY_UP]) {
            Tile *tile = openDoor(entity->x + entity->sprite->width * 0.5, entity->y + entity->sprite->height);
            uint8_t p = tile->data.door.progress;
            if (p > 0) {
                p--;
                // The map tile repainting takes care of removing leftovers of this. p % 16
                drawRectColor((Rect){entity->x, entity->y, p >> 4, 2}, 10);
                tile->data.door.progress = p;
                m_setFlag(playerActions, PLAYER_SEARCHING);
            } else {
                m_unsetFlag(playerActions, PLAYER_SEARCHING);
            }
            // This works because sprite is same size as tile. If not, we need to adjust the x position to center the sprite with the tile
            entity->x = (int)(entity->x + entity->sprite->width * 0.5) / TILE_SIZE * TILE_SIZE;
        } else if (m_isFlagSet(playerActions, PLAYER_SEARCHING)) {
            Tile *tile = closeDoor(entity->x + entity->sprite->width * 0.5, entity->y + entity->sprite->height);
            // Decide if cancelling serach resets the door timer
            tile->data.door.progress = 255;
            m_unsetFlag(playerActions, PLAYER_SEARCHING);
        }
    }

    // TODO: Dot this better. This is done to prevent "key bouncing" (player moving too fast between stairs)
    // TODO: This should be based on the walk up/down stairs animation.
    if (m_isFlagSet(playerActions, PLAYER_ON_STAIRS)) {
        counter++;
        if (counter >= 36) {
            m_unsetFlag(playerActions, PLAYER_ON_STAIRS);
            m_unsetFlag(entity->flags, (ENTITY_BLOCKED | ENTITY_INVULNERABLE));
            counter = 0;
        }
    }

    setAnimation();
}