#include "timer.h"

// BCM2837 system timer - free runs at 1 MHz (1 tick = 1 us)
#define TIMER_CLO   (*(volatile uint32_t *)0x3F003004UL)

void delay_ms(uint32_t ms)
{
    uint32_t start = TIMER_CLO;
    uint32_t ticks = ms * 1000;         // ms → us
    while ((TIMER_CLO - start) < ticks);
}