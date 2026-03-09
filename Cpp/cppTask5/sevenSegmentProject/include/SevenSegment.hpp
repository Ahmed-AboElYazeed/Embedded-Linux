#pragma once
#include "IStream.hpp"
#include "OStream.hpp"
#include "gpio.hpp"

class sevenSegment : public inStream, public outStream {
private:
    COTS::MCAL::GPIO::gpio pinA, pinB, pinC, pinD, pinE, pinF, pinG;

public:
    sevenSegment(const std::string& path,
                 std::istream& in,
                 std::ostream& out);

    void writeNumber(const std::string& numberStr);
};
