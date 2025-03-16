#include "entities/entities.h"
#include "animations.h"

/**
 * Animations are a sequence of sprite frames (index) that are looped through on
 * each frame. The last frame is marked with -1 to indicate the end of the animation.
 *
 */
int8_t animations[] = {
    2, 3, 4, 5, 6, 7, ANIM_LOOP,                    // Player walking (7 frames)
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, ANIM_LOOP,        // Player Idle (11 frames)
    15, 15, 16, 16, 16, 16, ANIM_HOLD,              // Player Shooting (7)
    8, 8, 8, 9, 9, 9, 9, ANIM_LOOP,                 // Player Searching (8 frames)
    0, 0, 0, ANIM_HOLD,                             // Manoloca (4 frames)
    0, 1, 2, 3, 4, ANIM_LOOP,                       // Joystick walking (6 frames)
    10, 12, 11, 11, 12, 13, 14, 0, ANIM_HOLD,       // Player Stairs (10 frames)
    5, 5, 6, 6, ANIM_LOOP,                          // Joystick stunt (5 frames)
    17, 17, 18, 19, 20, 21, 21, 20, 19, ANIM_HOLD,  // Player dying (10 frames)
};

int8_t getAnimationFrame(char nextFrame, Entity* entity) {
    int16_t currentAnimation = entity->animation;

    // No need to update the frame, just return current frame
    if (!nextFrame) return animations[currentAnimation + entity->frame];

    // Get next animation frame
    int8_t animationFrame = animations[++entity->frame + currentAnimation];

    // Reset frame when at the end of animation to loop back
    if (animationFrame == ANIM_LOOP) {
        entity->frame = 0;
        animationFrame = animations[currentAnimation];
    }

    // Substract 1 frame to hold to the last animation frame
    if (animationFrame == ANIM_HOLD) {
        animationFrame = animations[--entity->frame + currentAnimation];
    }

    return animationFrame;
}