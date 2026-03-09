#include "gpio.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace COTS::MCAL::GPIO;

gpio::gpio(uint32_t pin)
{
    auto pin_str = std::to_string(pin + 512);
    auto gpioDir  = "/sys/class/gpio/gpio" + pin_str + "/direction";
    auto gpioVal  = "/sys/class/gpio/gpio" + pin_str + "/value";

    export_fd = open("/sys/class/gpio/export", O_WRONLY);
    write(export_fd, pin_str.c_str(), pin_str.size());

    dir_fd = open(gpioDir.c_str(), O_WRONLY);
    val_fd = open(gpioVal.c_str(), O_RDWR);
}

gpio::~gpio()
{
    close(export_fd);
    close(dir_fd);
    close(val_fd);
}

void gpio::initPin(PinDir_enu_t dir)
{
    if (dir == OUT)
        write(dir_fd, "out", 3);
    else
        write(dir_fd, "in", 2);
}

void gpio::setPin()
{
    write(val_fd, "1", 1);
}

void gpio::clearPin()
{
    write(val_fd, "0", 1);
}

void gpio::writePinValue(int8_t value)
{
    char v = value ? '1' : '0';
    write(val_fd, &v, 1);
}

uint8_t gpio::readPin()
{
    char v;
    read(val_fd, &v, 1);
    return v;
}
