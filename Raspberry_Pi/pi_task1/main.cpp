// for testing only
#include <iostream>

//
#include <string>

#include "gpio.cpp" // for now

// for using the MCAL layer directly
using namespace COTS::MCAL;

int main(void)
{
    uint8_t pinState=0;
    uint32_t pin=26;
    GPIO::gpio gpio26(pin);
    gpio26.initPin(GPIO::OUTPUT);

    while (1)
    {
        gpio26.setPin();
        for (volatile int i=0; i<10000000; i++);
        gpio26.clearPin();
        for (volatile int i=0; i<10000000; i++);
        //gpio26.togglePin();
        pinState = gpio26.readPin();
        std::cout << pinState <<"\n";       ////// <<<<<
    }
    
}
