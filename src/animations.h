#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

extern int16_t animations[];

struct Entity;

int16_t getAnimationFrame(char nextFrame, struct Entity* entity);

#define ANIM_LOOP -1
#define ANIM_HOLD -2

enum Animation {
    ANIM_PLAYER_WALK_LEN = 7,
    ANIM_PLAYER_IDLE_LEN = 11,
    ANIM_PLAYER_SHOOT_LEN = 7,
    ANIM_PLAYER_SEARCH_LEN = 8,
    ANIM_WHIP_LEN = 4,
    ANIM_ENEMY1_WALK_LEN = 6,
    ANIM_PLAYER_STAIRS_LEN = 6,
    ANIM_ENEMY1_STUNT_LEN = 4
};

enum AnimationLength {
    ANIM_PLAYER_WALK,
    ANIM_PLAYER_IDLE = ANIM_PLAYER_WALK + ANIM_PLAYER_WALK_LEN,
    ANIM_PLAYER_SHOOT = ANIM_PLAYER_IDLE + ANIM_PLAYER_IDLE_LEN,
    ANIM_PLAYER_SEARCH = ANIM_PLAYER_SHOOT + ANIM_PLAYER_SHOOT_LEN,
    ANIM_WHIP = ANIM_PLAYER_SEARCH + ANIM_PLAYER_SEARCH_LEN,
    ANIM_ENEMY1_WALK = ANIM_WHIP + ANIM_WHIP_LEN,
    ANIM_PLAYER_STAIRS = ANIM_ENEMY1_WALK + ANIM_ENEMY1_WALK_LEN,
    ANIM_ENEMY1_STUNT = ANIM_PLAYER_STAIRS + ANIM_PLAYER_STAIRS_LEN,
};

#endif  // _ANIMATIONS_H_