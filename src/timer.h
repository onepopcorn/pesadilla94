#ifndef _TIMER_H
#define _TIMER_H

// #define IRS_TIMER 0x1c
#define IRS_TIMER 0x08

volatile static uint32_t ticks;
void timerInit(void);
void timerFree(void);
uint32_t getMilliseconds();
int setTimeout(void (*callback)(void), uint32_t seconds);

#endif  // _TIMER_H