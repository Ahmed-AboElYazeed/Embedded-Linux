#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIO_BASE   0x3F200000UL

#define GPFSEL1     (*(volatile uint32_t *)(GPIO_BASE + 0x04))
#define GPFSEL2     (*(volatile uint32_t *)(GPIO_BASE + 0x08))
#define GPSET0      (*(volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPCLR0      (*(volatile uint32_t *)(GPIO_BASE + 0x28))

#define LED_PIN     27

void gpio_init(void);
void gpio_set(void);
void gpio_clr(void);

#endif