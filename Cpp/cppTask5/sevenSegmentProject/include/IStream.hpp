#pragma once
#include "Stream.hpp"
#include <istream>

class inStream : virtual public stream {
protected:
    std::istream& in;

public:
    inStream(std::istream& input, const std::string& path);
    ~inStream();
    void open() override;
    std::istream& operator>>(std::string& data);
};
