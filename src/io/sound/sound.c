#include <stdint.h>
#include <mikmod.h>
#include "sound.h"

SAMPLE *sfx_whip;

uint8_t soundInit() {
    // MikMod_RegisterDriver(&drv_sb);
    MikMod_RegisterAllDrivers();
    md_mode |= DMODE_SOFT_SNDFX;
    // md_device = 1;
    if (MikMod_Init("")) {
        printf("MikMod error: %s\n", MikMod_strerror(MikMod_errno));
        return EXIT_FAILURE;
    }

    sfx_whip = Sample_Load("whip.wav");
    if (!sfx_whip) {
        printf("Failed to load sound effects: %s\n", MikMod_strerror(MikMod_errno));
        return EXIT_FAILURE;
    }

    MikMod_SetNumVoices(-1, 2);
    MikMod_EnableOutput();

    return EXIT_SUCCESS;
}

void soundFree() {
    if (sfx_whip) Sample_Free(sfx_whip);
    MikMod_Exit();
}

volatile static uint8_t divider = 0;
void soundUpdate() {
    // MikMod_Update();
    if (++divider == 20) {
        divider = 0;
        MikMod_Update();
    }
}

void playWhip() {
    int handle = Sample_Play(sfx_whip, 0, SFX_CRITICAL);  // No loop
    Voice_SetVolume(handle, 255);                         // Max volume (0-255)
}