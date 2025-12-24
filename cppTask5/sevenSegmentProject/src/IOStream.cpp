#include "IOStream.hpp"

ioStream::ioStream(const std::string& path,
                   std::istream& in,
                   std::ostream& out)
    : stream(path), inStream(in, path), outStream(out, path) {}

ioStream::~ioStream() {}
