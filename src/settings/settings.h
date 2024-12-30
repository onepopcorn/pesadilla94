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

/**
 * Entities system configuration
 */
#define MAX_ENTITIES 10        // Limit how many entities we can have at once to not overflow memory
#define MAX_TILE_PER_ENTITY 6  // Approximate the max number of tiles a single entity can "touch"

/**
 * Gameplay configuration
 */
#define MAX_LIVES 4
#define EXTRA_LIFE_SCORE 10000

#endif  // _SETTINGS_H_