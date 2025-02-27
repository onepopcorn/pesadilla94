#include <string.h>
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
#include "timer.h"
#include "gameState.h"

#include "player.h"

#define PLAYER_SPEED 0.9
#define PLAYER_SHOOT_RECHARGE_TIME 2000
#define PLAYER_INVULNERABILITY_TIME 800
#define PLAYER_COLLISION_MASK TYPE_ENEMY_A | TYPE_ENEMY_B

Entity *player;
uint8_t playerCanShoot = true;
PlayerState playerState = STATE_IDLE;

// PRIVATE METHODS

void reSpawn() {
}

void enableWhip(uint8_t id) {
    playerCanShoot = true;
}

void invulnerabilityEnd(uint8_t id) {
    player->collisionMask = PLAYER_COLLISION_MASK;
    m_unsetFlag(player->flags, ENTITY_FLASHING);
}

void useStairs(bool up) {
    // Get player jump position
    Vec2 pos = getStairsDestination(player->x + player->sprite->width * 0.5, player->y + player->sprite->height, up);
    if (pos.x <= 0 || pos.y <= 0) return;

    m_setAnimation(player, ANIM_PLAYER_STAIRS);

    player->vx = 0;
    player->y = pos.y - player->sprite->height / 2 + 1;  // +1 because all characters are "closer" to camera to force perspective a bit
    player->x = pos.x;

    // Give player some invulnerability time after using stairs
    player->collisionMask = TYPE_NONE;  // Disable collisions during stairs transition
    m_setFlag(player->flags, ENTITY_FLASHING);
    setTimeout(&invulnerabilityEnd, player->id, PLAYER_INVULNERABILITY_TIME);
}

void shoot() {
    // Can't shoot if too close to the wall player is facing
    if ((player->x < 10 && m_isFlagSet(player->flags, ENTITY_FLIP)) || (player->x > 280 && !m_isFlagSet(player->flags, ENTITY_FLIP))) {
        playerState = STATE_IDLE;
        return;
    }

    // Get position where whip should be shown
    bool facingRight = !m_isFlagSet(player->flags, ENTITY_FLIP);
    uint16_t offset = facingRight ? player->sprite->width - 1 : 1 - player->sprite->width;

    whipSpawn(player->x + offset, player->y + 3, facingRight);

    playerCanShoot = false;
    setTimeout(&enableWhip, player->id, PLAYER_SHOOT_RECHARGE_TIME);
}

void searchDoor() {
    if (!m_isKeyDown(KEY_UP)) {
        closeDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
        // TODO Decide if cancelling search resets the door timer
        // Tile *tile = closeDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
        // tile->data.door.progress = 255;

        playerState = STATE_IDLE;
        return;
    }

    Tile *tile = openDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
    uint8_t p = tile->data.door.progress;

    if (p == 0) {
        gameState.doorsLeft--;
        playerState = STATE_IDLE;
        return;
    }

    // Snap player to door position
    // This works because sprite is same size as tile. If not, we need to adjust the x position to center the sprite with the tile
    player->x = (uint16_t)(player->x + player->sprite->width * 0.5) / TILE_SIZE * TILE_SIZE;

    // The map tile repainting takes care of removing leftovers of this. p % 16
    drawRectColor((Rect){player->x, player->y, p >> 4, 2}, 10);
    tile->data.door.progress--;
    m_setAnimation(player, ANIM_PLAYER_SEARCH);
}

// PUBLIC METHODS

Entity *playerSpawn(uint16_t x, uint16_t y) {
    // should create the player entity
    player = createEntity(x, y, TYPE_PLAYER, playerSprite, playerUpdate);

    // set collision mask
    player->collisionMask = PLAYER_COLLISION_MASK;

    // set hitbox
    player->hitbox = (Rect){playerSprite->width / 4, 4, playerSprite->width / 2, playerSprite->height - 4};

    // should set the player initial stats (position, lives, score...)
    m_setAnimation(player, ANIM_PLAYER_IDLE);
    return player;
}

void playerUpdate(struct Entity *entity, uint8_t tileCollisions) {
    switch (playerState) {
        case STATE_WALKING:
            m_setAnimation(player, ANIM_PLAYER_WALK);

            // Shoot
            if (isKeyJustPressed(KEY_SPACE) && playerCanShoot) {
                playerState = STATE_SHOOTING;
            }

            // Move left
            if (m_isKeyDown(KEY_LEFT) && !m_isKeyDown(KEY_RIGHT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_L)) {
                entity->vx = -PLAYER_SPEED;
                m_setFlag(entity->flags, ENTITY_FLIP);
                return;
            }

            // Move right
            if (m_isKeyDown(KEY_RIGHT) && !m_isKeyDown(KEY_LEFT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_R)) {
                entity->vx = PLAYER_SPEED;
                m_unsetFlag(entity->flags, ENTITY_FLIP);
                return;
            }

            playerState = STATE_IDLE;
            break;
        case STATE_SEARCHING:
            searchDoor();
            break;

        case STATE_SHOOTING:
            m_setAnimation(player, ANIM_PLAYER_SHOOT);
            player->vx = 0;

            if (player->frame == 2) shoot();
            if (player->frame == ANIM_PLAYER_SHOOT_LEN - 2) playerState = STATE_IDLE;
            break;

        case STATE_STAIRS:
            if (player->frame == ANIM_PLAYER_STAIRS_LEN - 2) playerState = STATE_IDLE;
            break;

        case STATE_DYING:

            break;

        case STATE_IDLE:
            // Stop player & set animation
            player->vx = 0;
            m_setAnimation(player, ANIM_PLAYER_IDLE);

            // Shoot
            if (isKeyJustPressed(KEY_SPACE) && playerCanShoot) {
                playerState = STATE_SHOOTING;
            }

            // Use stairs
            if (m_isFlagSet(tileCollisions, COLLISION_STAIRS) && (isKeyJustPressed(KEY_UP) || isKeyJustPressed(KEY_DOWN))) {
                playerState = STATE_STAIRS;
                useStairs(m_isKeyDown(KEY_UP));
            }

            // Search doors
            if (m_isFlagSet(tileCollisions, COLLISION_DOOR) && isKeyJustPressed(KEY_UP)) {
                playerState = STATE_SEARCHING;
            }

            // Walk
            if ((m_isKeyDown(KEY_LEFT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_L)) || (m_isKeyDown(KEY_RIGHT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_R))) {
                playerState = STATE_WALKING;
            }
    }
}

void playerDie() {
    playerState = STATE_DYING;
}