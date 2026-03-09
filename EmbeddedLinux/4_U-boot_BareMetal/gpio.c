#include "gpio.h"

void gpio_init(void)
{
    // GPIO 27 is controlled by GPFSEL2 (covers pins 20-29)
    // Each pin = 3 bits. Pin 27 starts at bit (27 % 10) * 3 = 21
    uint32_t reg = GPFSEL2;
    reg &= ~(7u << 21);     // clear bits 23:21
    reg |=  (1u << 21);     // set as OUTPUT
    GPFSEL2 = reg;
}

void gpio_set(void)
{
    GPSET0 = (1u << LED_PIN);
}

void gpio_clr(void)
{
    GPCLR0 = (1u << LED_PIN);
}