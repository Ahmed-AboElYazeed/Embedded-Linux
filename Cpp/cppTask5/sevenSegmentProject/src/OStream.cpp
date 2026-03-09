#include "OStream.hpp"

outStream::outStream(std::ostream& output, const std::string& path)
    : stream(path), out(output) {}

outStream::~outStream() {}

std::ostream& outStream::operator<<(const std::string& data)
{
    out << data;
    return out;
}
