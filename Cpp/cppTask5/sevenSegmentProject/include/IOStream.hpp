#pragma once
#include "IStream.hpp"
#include "OStream.hpp"

class ioStream : public inStream, public outStream {
public:
    ioStream(const std::string& path,
             std::istream& in,
             std::ostream& out);
    ~ioStream();
};
