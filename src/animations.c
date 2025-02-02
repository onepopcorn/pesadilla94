#include "animations.h"

/**
 * Animations are a sequence of sprite frames (index) that are looped through on
 * each frame. The last frame is marked with -1 to indicate the end of the animation.
 *
 * TODO: Create an animation array per entity type
 * TODO: Use -2 as one time animation and go back to IDLE (first animation)
 */
int Animations[] = {
    2, 3, 4, 5, 6, 7, ANIM_LOOP,              // Player walking (7 frames)
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, ANIM_LOOP,  // Player Idle (11 frames)
    8, 8, 8, 9, 9, 9, 9, ANIM_LOOP,           // Player Searching (8 frames)
    2, 4, 5, 5, 5, ANIM_HOLD,                 // Manoloca (6 frames)
    0, 1, 2, 3, 4, ANIM_LOOP,                 // Joystick walking (6 frames)
    // 10, 13, ANIM_HOLD,                        // Player Stairs (3 frames)
    10, 10, 11, 12, 13, ANIM_JMP, ANIM_PLAYER_IDLE,  // Player Stairs (6 frames)
};