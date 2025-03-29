#include <stdint.h>
#include <mikmod.h>
#include "sound.h"

MODULE *music;

void errorHandling() {
    printf("MikMod Error occurred: %s\n", MikMod_strerror(MikMod_errno));
    if (MikMod_critical) {
        printf("MikMod is in a unsuable state: %s\n", MikMod_strerror(MikMod_critical));
    }
}

uint8_t soundInit() {
    printf("\n");
    long engineversion = MikMod_GetVersion();
    printf("Initializing MikMod Sound Library v%ld.%ld.%ld\n\n", (engineversion >> 16) & 255, (engineversion >> 8) & 255, (engineversion) & 255);
    MikMod_RegisterErrorHandler(errorHandling);

    // MikMod_RegisterDriver(&drv_sb);
    MikMod_RegisterAllDrivers();
    // MikMod_RegisterLoader(&load_s3m);
    MikMod_RegisterAllLoaders();

    md_mode |= DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC;
    MikMod_Init("");

    MikMod_SetNumVoices(8, 4);
    MikMod_EnableOutput();

    music = Player_Load("music.s3m", 64, 0);

    return EXIT_SUCCESS;
}

void startSong() {
    Player_Start(music);
}

void stopSong() {
    Player_Stop();
}

void soundFree() {
    Player_Free(music);
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