#include <stdint.h>
#include <mikmod.h>
#include "sound.h"

/**
 * Using a module to store all soundfx samples because for some reason
 * loading samples from wav files does not work.
 *
 * Be sure to set at least 1 pattern in the .it file or MikMod will
 * not recognize the module.
 *
 */
MODULE *sfxMod;

#define MAX_SFX_VOICES 4
#define SFX_FREQ 11025

// TODO: Pipe this to the stderror
void errorHandling() {
    printf("MikMod Error occurred: %s\n", MikMod_strerror(MikMod_errno));
    if (MikMod_critical) {
        printf("MikMod is in a unsuable state: %s\n", MikMod_strerror(MikMod_critical));
    }
}

uint8_t soundInit() {
    MikMod_RegisterErrorHandler(errorHandling);

    MikMod_RegisterDriver(&drv_sb);
    MikMod_RegisterLoader(&load_it);

    // md_mode |= DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC;
    md_sndfxvolume = 128;
    md_musicvolume = 64;

    MikMod_Init("");

    sfxMod = Player_Load("soundfx.it", 64, 0);

    MikMod_SetNumVoices(-1, MAX_SFX_VOICES);
    MikMod_EnableOutput();

    // Need to start module to use its samples
    Player_Start(sfxMod);
    Player_Paused();

    return EXIT_SUCCESS;
}

void soundFree() {
    Player_Free(sfxMod);
    MikMod_Exit();
}

int8_t playSound(enum Sfx sfx) {
    SAMPLE *sample = &sfxMod->samples[sfx];

    int8_t voice = Sample_Play(sample, 0, 0);
    Voice_SetVolume(voice, 128);
    Voice_SetFrequency(voice, SFX_FREQ);

    return voice;
}

int8_t stopSound(uint8_t voice) {
    // It seems MikMod does not always stops the voices loops properly
    // This fixes the bug with loops kept running forever
    Voice_SetVolume(voice, 0);
    Voice_Stop(voice);
    return -1;
}

void stopAllSounds() {
    for (uint8_t i = 0; i <= MAX_SFX_VOICES; i++) {
        Voice_Stop(i);
    }
}

int8_t isLoopRunning(int8_t voice) {
    // If MikMod is not updated VoiceStopped may return incorrect values
    MikMod_Update();
    return !Voice_Stopped(voice);
}

volatile static uint8_t divider = 0;
void soundUpdate() {
    if (++divider == 20) {
        divider = 0;
        MikMod_Update();
    }
}