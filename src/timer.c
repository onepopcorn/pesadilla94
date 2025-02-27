#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <go32.h>
#include <dpmi.h>
#include <pc.h>

#include "settings/settings.h"
#include "timer.h"

typedef struct {
    void (*callback)(uint8_t);  // Function pointer for the callback
    uint8_t param;
    uint32_t endTime;  // When the callback should be called
} Timeout;

volatile static uint32_t milliseconds = 0;

static Timeout timeouts[MAX_TIMERS] = {0};  // Array to track multiple timeouts

static _go32_dpmi_seginfo old_handler, new_handler;

static void timerHandler() {
    milliseconds += 55;  // ~55 ms per tick at 18.2 Hz

    // Check each registered timeout
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        if (timeouts[i].callback && milliseconds >= timeouts[i].endTime) {
            timeouts[i].callback(timeouts[i].param);
            timeouts[i].callback = NULL;  // Remove callback after calling
        }
    }

    outportb(0x20, 0x20);  // End-of-interrupt (EOI) signal to PIC
}

void timerInit() {
    _go32_dpmi_get_protected_mode_interrupt_vector(IRS_TIMER, &old_handler);
    new_handler.pm_offset = (uint32_t)timerHandler;
    new_handler.pm_selector = _go32_my_cs();
    _go32_dpmi_chain_protected_mode_interrupt_vector(IRS_TIMER, &new_handler);
}

void timerFree() {
    if (_go32_dpmi_set_protected_mode_interrupt_vector(IRS_TIMER, &old_handler) == -1)
        fprintf(stderr, "Failed to free the timer :(\n");
}

uint32_t getMilliseconds() {
    return milliseconds;
}

// Register a timeout callback
uint8_t setTimeout(void (*callback)(uint8_t), uint8_t param, uint32_t ms) {
    if (!callback) return -1;  // Ignore if NULL function

    // Find an empty slot
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        if (timeouts[i].callback == NULL) {
            timeouts[i].callback = callback;
            timeouts[i].param = param;
            timeouts[i].endTime = milliseconds + ms;
            return 0;  // Success
        }
    }

    return 1;  // No available slot
}

void clearAllTimeouts() {
    memset(timeouts, 0, sizeof(timeouts));
}
