#ifndef _HUD_H
#define _HUD_H

void drawHUD();
void updatePoints(int8_t score);
void updateLives(int8_t lives);
void updateTime(int8_t timeleft);
void updateDoors(int8_t doors);
void updateWeapon(char deactivated);

#endif  // _HUD_H