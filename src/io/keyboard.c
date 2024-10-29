#include <stdio.h>
#include <stdint.h>
#include <go32.h>
#include <dpmi.h>
#include <pc.h>

#include "keyboard.h"

volatile uint8_t keys[0xff] = { 0 };

static _go32_dpmi_seginfo old_handler, new_handler;

static void keyboardHandler() {
    uint8_t k = inportb(0x60);

    if(k & 128) {
        // Key has been released
        keys[k & 127] = 0;
    } else {
        // Key has been pressed
        keys[k] = 1;
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
        fprintf(stderr, "Failed to free the timer :(\n");
    else
        _go32_dpmi_free_iret_wrapper(&new_handler);
}