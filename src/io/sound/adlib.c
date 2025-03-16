#include <dos.h>
#include <stdint.h>

#define OPL_PORT 0x388  // AdLib base I/O port

void opl_wait() {
    int i;
    for (i = 0; i < 6; i++) inp(OPL_PORT);  // Small delay using dummy reads
}

void opl_write(uint8_t reg, uint8_t value) {
    outp(OPL_PORT, reg);
    opl_wait();
    outp(OPL_PORT + 1, value);
    opl_wait();
}

void adlib_beep() {
    opl_write(0x20, 0x01);  // Modulator
    opl_write(0x40, 0x10);  // Volume
    opl_write(0x60, 0xF0);  // Attack/Decay
    opl_write(0x80, 0x77);  // Sustain/Release
    opl_write(0xA0, 0x98);  // Frequency low byte
    opl_write(0xB0, 0x31);  // Frequency high byte + key-on

    delay(500);  // Let sound play for 500ms

    opl_write(0xB0, 0x11);  // Key-off (stop sound)
}