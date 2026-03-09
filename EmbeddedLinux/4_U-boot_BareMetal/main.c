#include "gpio.h"
#include "timer.h"

void main(void)
{
    gpio_init();

    for (;;) {
        gpio_set();         // LED ON
        delay_ms(500);

        gpio_clr();         // LED OFF
        delay_ms(500);
    }
}