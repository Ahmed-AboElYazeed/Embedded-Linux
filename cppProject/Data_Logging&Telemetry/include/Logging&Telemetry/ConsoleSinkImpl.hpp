#pragma once
#include <iostream>
#include "Logging&Telemetry/ILogSink.hpp"

class ConsoleSinkImpl: public ILogSink
{
private:
    /* data */
public:
    ConsoleSinkImpl(/* args */);
    ~ConsoleSinkImpl();
    void write (const LogMessage& msg);
};

