#ifndef _HUD_H
#define _HUD_H

void drawHUD();
void updatePoints(int score);
void updateLives(int lives);
void updateTime(int timeleft);
void updateDoors(int doors);
void updateWeapon(char deactivated);
void updateLevel();

#endif  // _HUD_H