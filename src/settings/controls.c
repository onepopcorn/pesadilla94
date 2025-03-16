#include <stdint.h>
#include "io/keyboard.h"

#include "controls.h"

uint8_t Controls[CONTROLS_NUM] = {
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_CTRL_L,
    KEY_ESC,
    KEY_P,
    // Cheats
    KEY_S,
    KEY_K,
    KEY_W,
};

void defineKey(enum ControlKey controlKey, uint8_t key) {
    Controls[controlKey] = key;
}