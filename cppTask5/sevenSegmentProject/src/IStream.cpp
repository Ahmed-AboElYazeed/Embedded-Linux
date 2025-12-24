#include "IStream.hpp"
#include <iostream>

inStream::inStream(std::istream& input, const std::string& path)
    : stream(path), in(input) {}

inStream::~inStream() {}

void inStream::open()
{
    std::cout << "open from inStream\n";
}

std::istream& inStream::operator>>(std::string& data)
{
    in >> data;
    return in;
}
