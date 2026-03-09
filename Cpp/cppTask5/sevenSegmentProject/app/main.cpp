#include "SevenSegment.hpp"
#include "IOStream.hpp"
#include <iostream>

int main()
{
    std::string path = "/tmp/iofile.txt";
    std::string input;

    sevenSegment display(path, std::cin, std::cout);
    display >> input;
    display.writeNumber(input);

    return 0;
}
