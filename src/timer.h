#ifndef _TIMER_H
#define _TIMER_H

// #define IRS_TIMER 0x1c
#define IRS_TIMER 0x08

volatile static uint32_t ticks;
void timerInit(void);
void timerFree(void);
uint32_t getMilliseconds();
uint8_t setTimeout(void (*callback)(uint8_t), uint8_t entityId, uint32_t ms);
void clearAllTimeouts();

#endif  // _TIMER_H