#pragma once

#include <cstdint>

namespace COTS {
namespace MCAL {
namespace GPIO {

typedef enum {
    IN  = 0,
    INPUT  = 0,
    OUT = 1,
    OUTPUT = 1
} PinDir_enu_t;

class gpio {
private:
    int export_fd;
    int dir_fd;
    int val_fd;

public:
    explicit gpio(uint32_t pin);

    gpio(const gpio&) = delete;
    gpio& operator=(const gpio&) = delete;
    gpio(gpio&&) = delete;
    gpio& operator=(gpio&&) = delete;

    ~gpio();

    void initPin(PinDir_enu_t dir);
    void setPin();
    void clearPin();
    void writePinValue(int8_t value);
    uint8_t readPin();
};

} } }
