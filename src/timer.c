#include <stdio.h>
#include <stdint.h>
#include <go32.h>
#include <dpmi.h>

#include "timer.h"

volatile static uint32_t ticks = 0;

static _go32_dpmi_seginfo old_handler, new_handler;

static void timerHandler() {
    ticks++;
}

void timerInit() {
    _go32_dpmi_get_protected_mode_interrupt_vector(IRS_TIMER, &old_handler);
    new_handler.pm_offset = (unsigned long)timerHandler;
    new_handler.pm_selector = _go32_my_cs();
    _go32_dpmi_chain_protected_mode_interrupt_vector(IRS_TIMER, &new_handler);
}

void timerFree() {
    if (_go32_dpmi_set_protected_mode_interrupt_vector(IRS_TIMER, &old_handler) == -1)
        fprintf(stderr, "Failed to free the timer :(\n");
}