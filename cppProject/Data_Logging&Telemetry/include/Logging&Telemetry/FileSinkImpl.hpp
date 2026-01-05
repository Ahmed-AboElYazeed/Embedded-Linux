#pragma once
#include "Logging&Telemetry/ILogSink.hpp"
#include "Logging&Telemetry/LogMessage.hpp"
#include <string>
#include <memory>

class FileSinkImpl: public ILogSink
{
private:
    std::unique_ptr<int> fdPtr = std::make_unique<int>(-1);
public:
    FileSinkImpl(/* args */);
    ~FileSinkImpl();
    void write (const LogMessage& msg);
    void operator<<(const LogMessage& msg);
};