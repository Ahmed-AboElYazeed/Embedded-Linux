#pragma once
#include <string>

class stream {
protected:
    std::string path;

public:
    explicit stream(const std::string& userPath);
    virtual ~stream();
    virtual void open() = 0;
};
