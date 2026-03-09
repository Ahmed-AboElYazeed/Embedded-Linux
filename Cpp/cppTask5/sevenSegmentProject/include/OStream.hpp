#pragma once
#include "Stream.hpp"
#include <ostream>

class outStream : virtual public stream {
protected:
    std::ostream& out;

public:
    outStream(std::ostream& output, const std::string& path);
    ~outStream();
    std::ostream& operator<<(const std::string& data);
};
