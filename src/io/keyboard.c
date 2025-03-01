#include <stdio.h>
#include <stdint.h>
#include <go32.h>
#include <dpmi.h>
#include <pc.h>

#include "keyboard.h"

#define KEY_RELEASED_MASK 0x80  // 0b10000000
#define KEYCODE_MASK 0x7F       // 0b01111111

volatile uint8_t keys[0xFF] = {0};
volatile uint8_t justPressed[0xFF] = {0};

static _go32_dpmi_seginfo old_handler, new_handler;

static void keyboardHandler() {
    uint8_t k = inportb(0x60);
    uint8_t key = k & KEYCODE_MASK;

    if (k & KEY_RELEASED_MASK) {
        // Key has been released
        keys[key] = 0;
        justPressed[key] = 0;
    } else {
        // Key has been pressed
        if (!keys[key]) justPressed[key] = 1;
        keys[key] = 1;
    }

    // Send end-of-interrupt (EOI) signal to PCI (programmable interrupt controller) to resume its normal operation
    outportb(0x20, 0x20);
}

void keyboardInit() {
    _go32_dpmi_get_protected_mode_interrupt_vector(IRS_KEYBOARD, &old_handler);
    new_handler.pm_offset = (unsigned long)keyboardHandler;
    new_handler.pm_selector = _go32_my_cs();
    _go32_dpmi_allocate_iret_wrapper(&new_handler);
    _go32_dpmi_set_protected_mode_interrupt_vector(IRS_KEYBOARD, &new_handler);
}

void keyboardFree() {
    if (_go32_dpmi_set_protected_mode_interrupt_vector(IRS_KEYBOARD, &old_handler) == -1)
        fprintf(stderr, "Failed to free the keyboard :(\n");
    else
        _go32_dpmi_free_iret_wrapper(&new_handler);
}

uint8_t isKeyJustPressed(uint8_t key) {
    if (justPressed[key]) {
        justPressed[key] = 0;
        return 1;
    }

    return 0;
}