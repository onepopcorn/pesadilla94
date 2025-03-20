#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>  // For usleep()

#include "effects.h"

Color originalPalette[256];

void initVideoEffects() {
    getPalette(originalPalette);
}

// Read the current VGA palette
void getPalette(Color palette[256]) {
    outp(0x3C7, 0);  // Start reading from color index 0
    for (int i = 0; i < 256; i++) {
        palette[i].r = inp(0x3C9);
        palette[i].g = inp(0x3C9);
        palette[i].b = inp(0x3C9);
    }
}

// Write a new palette to VGA memory
void setPalette(Color palette[256]) {
    outp(0x3C8, 0);  // Start writing from color index 0
    for (int i = 0; i < 256; i++) {
        outp(0x3C9, palette[i].r);
        outp(0x3C9, palette[i].g);
        outp(0x3C9, palette[i].b);
    }
}

// Perform the fade to black effect
void fadeToBlack(int steps, int delay) {
    Color palette[256];
    getPalette(palette);  // Get the current palette

    for (int s = 0; s < steps; s++) {
        for (int i = 0; i < 256; i++) {
            if (palette[i].r > 0) palette[i].r--;
            if (palette[i].g > 0) palette[i].g--;
            if (palette[i].b > 0) palette[i].b--;
        }
        setPalette(palette);   // Apply the modified palette
        usleep(delay * 1000);  // Delay in milliseconds
    }
}

void fadeIn(int steps, int delay) {
    Color palette[256] = {0};  // Start from black

    for (int s = 0; s < steps; s++) {
        for (int i = 0; i < 256; i++) {
            if (palette[i].r < originalPalette[i].r) palette[i].r++;
            if (palette[i].g < originalPalette[i].g) palette[i].g++;
            if (palette[i].b < originalPalette[i].b) palette[i].b++;
        }
        setPalette(palette);   // Apply the modified palette
        usleep(delay * 1000);  // Delay in milliseconds
    }
}

// Restore the original palette
void restorePalette() {
    setPalette(originalPalette);
}