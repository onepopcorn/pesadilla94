#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/**
 * USE THIS FILE FOR GLOBAL SETTINGS & CONFIGURATION
 * THAT NEEDS TO BE ACCESSIBLE FROM MULTIPLE FILES
 *
 */

/**
 * Map system configuration
 */
#define TILE_SIZE 16  // Size of a tile in pixels which makes calulations easier. Consider using a power of 2 to enable fast bitshifts
#define SCREEN_Y_OFFSET 40

/**
 * Entities system configuration
 */
#define MAX_ENTITIES 10        // Limit how many entities we can have at once to not overflow memory
#define MAX_TILE_PER_ENTITY 6  // Approximate the max number of tiles a single entity can "touch"
#define MAX_TIMERS 10

/**
 * Gameplay configuration
 */
#define MAX_LIVES 5
#define POINTS_PER_DOOR 30
#define POINTS_PER_STUNT 5
#define EXTRA_LIFE_SCORE 10000

#endif  // _SETTINGS_H_