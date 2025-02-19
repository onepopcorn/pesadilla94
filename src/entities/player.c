
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
#define PLAYER_SHOOT_DELAY 700
#define PLAYER_STAIRS_TRANSITION 950

Entity *player;
uint8_t playerActions = 0x00;

// PRIVATE METHODS

void resetShoot() {
    m_unsetFlag(playerActions, PLAYER_SHOOTING);
}

void endStairsTransitions() {
    m_unsetFlag(playerActions, PLAYER_ON_STAIRS);
    m_unsetFlag(player->flags, ENTITY_INVULNERABLE);
    player->frame = 0;
}

void useStairs(bool up) {
    Vec2 pos = getStairsDestination(player->x + player->sprite->width * 0.5, player->y + player->sprite->height, up);
    if (pos.x <= 0 || pos.y <= 0) return;

    player->y = pos.y - player->sprite->height / 2 + 1;  // +1 because all characters are "closer" to camera to force perspective a bit
    player->x = pos.x;
    m_setFlag(player->flags, (ENTITY_INVULNERABLE));
    m_unsetFlag(player->flags, ENTITY_FLIP);
    m_setFlag(playerActions, PLAYER_ON_STAIRS);
    setTimeout(&endStairsTransitions, PLAYER_STAIRS_TRANSITION);
}

void shoot() {
    bool facingRight = !m_isFlagSet(player->flags, ENTITY_FLIP);
    int x = facingRight ? player->x + player->sprite->width * 0.5 : player->x - player->sprite->width;
    whipSpawn(x, player->y - 5, facingRight);
    m_setFlag(playerActions, PLAYER_SHOOTING);
    setTimeout(&resetShoot, PLAYER_SHOOT_DELAY);
}

// TODO: Implement a justReleasedKey feature to simplify this code here
void searchDoor() {
    if (m_isKeyDown(KEY_UP)) {
        Tile *tile = openDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
        uint8_t p = tile->data.door.progress;
        if (p > 0) {
            p--;
            // The map tile repainting takes care of removing leftovers of this. p % 16
            drawRectColor((Rect){player->x, player->y, p >> 4, 2}, 10);
            tile->data.door.progress = p;
            m_setFlag(playerActions, PLAYER_SEARCHING);
        } else {
            if (m_isFlagSet(playerActions, PLAYER_SEARCHING)) gameState.doorsLeft--;
            m_unsetFlag(playerActions, PLAYER_SEARCHING);
        }
        // This works because sprite is same size as tile. If not, we need to adjust the x position to center the sprite with the tile
        player->x = (int)(player->x + player->sprite->width * 0.5) / TILE_SIZE * TILE_SIZE;
    } else if (m_isFlagSet(playerActions, PLAYER_SEARCHING)) {
        Tile *tile = closeDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
        // TODO Decide if cancelling search resets the door timer
        tile->data.door.progress = 255;
        m_unsetFlag(playerActions, PLAYER_SEARCHING);
    }
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

// PUBLIC METHODS

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

void playerUpdate(struct Entity *entity, struct Entity *player, uint8_t tileCollisions) {
    // MOVEMENT
    bool canMove = !m_isFlagSet(playerActions, NO_MOVE) && !m_isKeyDown(KEY_UP);
    if (m_isKeyDown(KEY_RIGHT) && !m_isKeyDown(KEY_LEFT) && canMove) {
        entity->vx = PLAYER_SPEED;
        m_unsetFlag(entity->flags, ENTITY_FLIP);
    } else if (m_isKeyDown(KEY_LEFT) && !m_isKeyDown(KEY_RIGHT) && canMove) {
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
    if (m_isFlagSet(tileCollisions, COLLISION_STAIRS) && !m_isFlagSet(playerActions, PLAYER_ON_STAIRS) && (isKeyJustPressed(KEY_UP) || isKeyJustPressed(KEY_DOWN))) {
        useStairs(m_isKeyDown(KEY_UP));
    }

    // SHOOTING
    if (isKeyJustPressed(KEY_SPACE) && !m_isFlagSet(playerActions, NO_SHOOT)) {
        shoot();
    }

    // SEARCHING DOORS
    if (m_isFlagSet(tileCollisions, COLLISION_DOOR) && !m_isFlagSet(playerActions, PLAYER_SHOOTING)) {
        searchDoor();
    }

    setAnimation();
}