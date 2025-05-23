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
#include "hud/hud.h"
#include "io/sound/sound.h"
#include "settings/controls.h"

#include "player.h"

#define PLAYER_SPEED 0.8
#define PLAYER_SHOOT_RECHARGE_TIME 2000
#define PLAYER_INVULNERABILITY_TIME 3000
#define PLAYER_COLLISION_MASK TYPE_ENEMY_A | TYPE_ENEMY_B

Entity *player;
uint8_t playerCanShoot = false;
Vec2 destination;
PlayerState playerState = STATE_IDLE;
Tile *searchDoorTile = NULL;
static int8_t sfxSteps = -1;

// PRIVATE METHODS

void enableWhip(uint8_t id) {
    playerCanShoot = true;
    updateWeapon(false);
    playSound(SFX_WPN_READY);
}

void invulnerabilityEnd(uint8_t id) {
    player->collisionMask = PLAYER_COLLISION_MASK;
    m_unsetFlag(player->flags, ENTITY_FLASHING);
}

void respawn() {
    updateLives(-1);

    if (gameState.lives > 0) {
        m_setAnimation(player, ANIM_PLAYER_IDLE);
        m_setFlag(player->flags, ENTITY_FLASHING);
        playerState = STATE_IDLE;
        setTimeout(&invulnerabilityEnd, player->id, PLAYER_INVULNERABILITY_TIME);
    }
}

void useStairs(bool up) {
    // Get player jump position
    Vec2 pos = getStairsDestination(player->x + player->sprite->width * 0.5, player->y + player->sprite->height, up);
    if (pos.x <= 0 || pos.y <= 0) {
        // Abort, there's no destination found
        playerState = STATE_IDLE;
        playSound(SFX_DOOR_NO_OP);
        return;
    }
    m_setAnimation(player, ANIM_PLAYER_STAIRS);

    player->vx = 0;
    m_snapXToGrid(player);

    // +1 because all characters are "closer" to camera to force perspective a bit
    destination.y = pos.y - player->sprite->height / 2 + 1;
    destination.x = pos.x;

    player->collisionMask = TYPE_NONE;  // Disable collisions during stairs transition
}

void spawnShot() {
    if (!playerCanShoot) return;

    updateWeapon(true);
    // Can't shoot if too close to the wall player is facing
    if ((player->x < 10 && m_isFlagSet(player->flags, ENTITY_FLIP)) || (player->x > 280 && !m_isFlagSet(player->flags, ENTITY_FLIP))) {
        playerState = STATE_IDLE;
        return;
    }

    // Get position where whip should be shown
    bool facingRight = !m_isFlagSet(player->flags, ENTITY_FLIP);
    uint16_t offset = facingRight ? player->sprite->width - 1 : 1 - player->sprite->width;

    whipSpawn(player->x + offset, player->y + 8, facingRight);

    playerCanShoot = false;
    setTimeout(&enableWhip, player->id, PLAYER_SHOOT_RECHARGE_TIME);
}

void interruptSearching() {
    closeDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
    searchDoorTile->data.door.progress = 254;
    searchDoorTile = NULL;
    playerState = STATE_IDLE;
}

void searchDoor() {
    if (!searchDoorTile) return;

    // Close door when stop
    if (!m_isKeyDown(m_UP)) {
        interruptSearching();
        return;
    }

    m_snapXToGrid(player);
    m_setAnimation(player, ANIM_PLAYER_SEARCH);

    searchDoorTile->data.door.progress -= 2;
    uint8_t p = searchDoorTile->data.door.progress;

    // The map tile repainting takes care of removing leftovers of this. p >> 4 p / 16
    drawRectColor((Rect){player->x, player->y, p >> 4, 2}, 10);
    // Doors progress values is set to 254 so it can reach 0 exactly
    if (p == 0) {
        updateDoors(-1);
        updatePoints(POINTS_PER_DOOR);
        playerState = STATE_IDLE;
        searchDoorTile = NULL;
        playSound(SFX_DOOR_DONE);
        return;
    }
}

void pickupWeapon() {
    m_snapXToGrid(player);
    playerState = STATE_PICKING_UP;
    playerCanShoot = true;
    playSound(SFX_WPN_PICKUP);
    m_setAnimation(player, ANIM_PLAYER_SEARCH);
}

// PUBLIC METHODS

Entity *playerSpawn(uint16_t x, uint16_t y) {
    player = createEntity(x, y, TYPE_PLAYER, playerSprite, playerUpdate);
    player->collisionMask = PLAYER_COLLISION_MASK;
    player->hitbox = (Rect){playerSprite->width / 4, 4, playerSprite->width / 2, playerSprite->height - 4};
    m_setAnimation(player, ANIM_PLAYER_IDLE);

    playerState = STATE_IDLE;
    playerCanShoot = false;
    updateWeapon(true);
    return player;
}

void playerUpdate(struct Entity *entity, uint8_t tileCollisions) {
    if (playerState != STATE_WALKING) {
        stopSound(sfxSteps);
    }

    // if (playerState == STATE_WALKING && sfxSteps < 0 && !isLoopRunning(sfxSteps)) {
    //     sfxSteps = playSound(SFX_PLAYER_MOVE);
    // }

    switch (playerState) {
        case STATE_WALKING:
            m_setAnimation(player, ANIM_PLAYER_WALK);

            // Shoot
            if (m_isKeyDown(m_SHOOT) && playerCanShoot) {
                playerState = STATE_SHOOTING;
            }

            // Move left
            if (m_isKeyDown(m_LEFT)) {
                if (!m_isFlagSet(tileCollisions, COLLISION_WALL_L)) {
                    entity->vx = -PLAYER_SPEED;
                    m_setFlag(entity->flags, ENTITY_FLIP);
                } else {
                    entity->vx = 0;
                    playerState = STATE_IDLE;
                }
                return;
            }

            // Move right
            if (m_isKeyDown(m_RIGHT)) {
                if (!m_isFlagSet(tileCollisions, COLLISION_WALL_R)) {
                    entity->vx = PLAYER_SPEED;
                    m_unsetFlag(entity->flags, ENTITY_FLIP);
                } else {
                    entity->vx = 0;
                    playerState = STATE_IDLE;
                }
                return;
            }

            playerState = STATE_IDLE;
            break;
        case STATE_SEARCHING:
            searchDoor();
            break;

        case STATE_PICKING_UP:
            if (player->frame == ANIM_PLAYER_SEARCH_LEN - 2) {
                disableVendingMachine(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
                updateWeapon(false);
                playerState = STATE_IDLE;
            }
            break;

        case STATE_SHOOTING:
            m_setAnimation(player, ANIM_PLAYER_SHOOT);
            player->vx = 0;

            if (player->frame == 2) spawnShot();
            if (player->frame == ANIM_PLAYER_SHOOT_LEN - 2) playerState = STATE_IDLE;
            break;

        case STATE_STAIRS_IN:
            if (player->frame > 2) {
                player->x = destination.x;
                player->y = destination.y;

                playerState = STATE_STAIRS_OUT;
            }
            break;

        case STATE_STAIRS_OUT:
            if (player->frame >= ANIM_PLAYER_STAIRS_LEN - 2) {
                playerState = STATE_IDLE;
                // Don't restore collision if player was killed recently
                if (!m_isFlagSet(player->flags, ENTITY_FLASHING)) {
                    player->collisionMask = PLAYER_COLLISION_MASK;
                }
            }
            break;

        case STATE_DYING:
            if (player->frame == ANIM_PLAYER_DIE_LEN - 2) respawn();
            break;

        case STATE_IDLE:
            // Stop player & set animation
            player->vx = 0;
            m_setAnimation(player, ANIM_PLAYER_IDLE);

            // Shoot
            if (m_isKeyDown(m_SHOOT) && playerCanShoot) {
                playerState = STATE_SHOOTING;
                break;
            }

            // Use stairs
            if (m_isFlagSet(tileCollisions, COLLISION_STAIRS) && (isKeyJustPressed(m_UP) || isKeyJustPressed(m_DOWN))) {
                playerState = STATE_STAIRS_IN;
                useStairs(m_isKeyDown(m_UP));
                break;
            }

            // Search doors
            if (m_isFlagSet(tileCollisions, COLLISION_DOOR) && isKeyJustPressed(m_UP)) {
                searchDoorTile = openDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
                if (searchDoorTile->data.door.progress > 0) {
                    playSound(SFX_DOOR_OPEN);
                    playerState = STATE_SEARCHING;
                    break;
                } else {
                    playSound(SFX_DOOR_NO_OP);
                    closeDoor(player->x + player->sprite->width * 0.5, player->y + player->sprite->height);
                    break;
                }
            }

            // Search Vending macchine
            if (m_isFlagSet(tileCollisions, COLLISION_VENDING) && isKeyJustPressed(m_UP)) {
                if (!playerCanShoot) {
                    pickupWeapon();
                    break;
                } else {
                    playSound(SFX_DOOR_NO_OP);
                    break;
                }
            }

            // Walk
            if ((m_isKeyDown(m_LEFT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_L)) || (m_isKeyDown(m_RIGHT) && !m_isFlagSet(tileCollisions, COLLISION_WALL_R))) {
                playerState = STATE_WALKING;
                sfxSteps = playSound(SFX_PLAYER_MOVE);
                break;
            }
    }
}

void playerDie() {
    // Close door before dying if player is searching
    if (playerState == STATE_SEARCHING) interruptSearching();

    // Same than above... this is a dirty hack
    sfxSteps = stopSound(sfxSteps);
    playSound(SFX_DEATH);

    m_setAnimation(player, ANIM_PLAYER_DIE);
    player->collisionMask = TYPE_NONE;
    playerState = STATE_DYING;
    player->vx = 0;
}