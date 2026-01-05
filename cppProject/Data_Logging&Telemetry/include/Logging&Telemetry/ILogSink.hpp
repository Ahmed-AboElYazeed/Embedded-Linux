#pragma once
#include "Logging&Telemetry/LogMessage.hpp"

class ILogSink
{
private:
    /* data */
public:
    ILogSink(/* args */);
    virtual ~ILogSink();
    virtual void write (const LogMessage& msg) = 0;
};
