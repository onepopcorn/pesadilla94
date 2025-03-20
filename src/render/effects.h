#ifndef _EFFECTS_H
#define _EFFECTS_H

typedef struct {
    uint8_t r, g, b;
} Color;

void initVideoEffects();

void getPalette(Color palette[256]);

void setPalette(Color palette[256]);

void fadeToBlack(int steps, int delay);

void restorePalette();

void fadeIn(int steps, int delay);

#endif  // _EFFECTS_H