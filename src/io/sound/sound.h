#ifndef _SOUND_H
#define _SOUND_H

enum Sfx {
    SFX_FANFARE = 0,
    SFX_DOOR_DONE = 1,
    SFX_DOOR_NO_OP = 2,
    SFX_DOOR_OPEN = 3,
    SFX_ENEMY_HIT = 4,
    SFX_MENU_START = 5,
    SFX_PLAYER_MOVE = 6,
    SFX_WPN_READY = 7,
    SFX_POINTS = 8,
    SFX_DEATH = 9,
    SFX_WHIP = 10,
    SFX_WPN_PICKUP = 11,
    SFX_GAME_OVER = 12,
};

uint8_t soundInit();
void soundFree();
void soundUpdate();

int8_t playSound(enum Sfx sfx);
int8_t stopSound(uint8_t voice);
char isLoopRunning(int8_t voice);
void stopAllSounds();

#endif  // _SOUND_H