#pragma once
#include <iostream>
#include "Logging&Telemetry/ILogSink.hpp"

class ConsoleSinkImpl: public ILogSink
{
private:
    // std::ostream out;
public:
    ConsoleSinkImpl(/* args */);
    ~ConsoleSinkImpl();
    void write (const LogMessage& msg);
    void operator<<(const LogMessage& msg);
};

