#ifndef _HUD_H
#define _HUD_H

void hudInit();

void writeMessage(char *message);

void showProgress(uint8_t progress);

void clearProgress();

void hudFree();

#endif  // _HUD_H