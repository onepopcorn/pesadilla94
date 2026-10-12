#include <stdint.h>
#include <dos.h>
#include <stdio.h>

#include "beeper.h"

void playBeep(int frequency, int duration_ms) {
    int divisor = 1193180 / frequency;  // PIT frequency divisor

    outp(0x43, 0xB6);            // Set PIT mode
    outp(0x42, divisor & 0xFF);  // Low byte
    outp(0x42, divisor >> 8);    // High byte

    outp(0x61, inp(0x61) | 3);  // Enable speaker

    delay(duration_ms);  // Wait

    outp(0x61, inp(0x61) & ~3);  // Disable speaker
}