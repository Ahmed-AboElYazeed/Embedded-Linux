#include "SevenSegment.hpp"

sevenSegment::sevenSegment(const std::string& path,
                           std::istream& in,
                           std::ostream& out)
    : stream(path),
      inStream(in, path),
      outStream(out, path),
      pinA(1), pinB(2), pinC(3),
      pinD(4), pinE(5), pinF(6), pinG(7)
{
    pinA.initPin(COTS::MCAL::GPIO::OUT);
    pinB.initPin(COTS::MCAL::GPIO::OUT);
    pinC.initPin(COTS::MCAL::GPIO::OUT);
    pinD.initPin(COTS::MCAL::GPIO::OUT);
    pinE.initPin(COTS::MCAL::GPIO::OUT);
    pinF.initPin(COTS::MCAL::GPIO::OUT);
    pinG.initPin(COTS::MCAL::GPIO::OUT);
}

void sevenSegment::writeNumber(const std::string& numberStr)
{
    static const uint8_t map[10][7] = {
        {1,1,1,1,1,1,0},{0,1,1,0,0,0,0},{1,1,0,1,1,0,1},
        {1,1,1,1,0,0,1},{0,1,1,0,0,1,1},{1,0,1,1,0,1,1},
        {1,0,1,1,1,1,1},{1,1,1,0,0,0,0},{1,1,1,1,1,1,1},
        {1,1,1,1,0,1,1}
    };

    int d = numberStr[0] - '0';
    COTS::MCAL::GPIO::gpio* pins[7] =
        {&pinA,&pinB,&pinC,&pinD,&pinE,&pinF,&pinG};

    for (int i = 0; i < 7; ++i)
        map[d][i] ? pins[i]->setPin() : pins[i]->clearPin();
}
