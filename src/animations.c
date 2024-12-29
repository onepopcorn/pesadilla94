#include "animations.h"

/**
 * Animations are a sequence of sprite frames (index) that are looped through on
 * each frame. The last frame is marked with -1 to indicate the end of the animation.
 *
 */
int Animations[] = {
    2, 3, 4, 5, 6, 7, -1,              // Player walking
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, -1,  // Player Idle
};