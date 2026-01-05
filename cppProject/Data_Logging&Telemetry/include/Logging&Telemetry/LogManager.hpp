#pragma once
#include <vector>
#include "Logging&Telemetry/LogMessage.hpp"
#include "Logging&Telemetry/ILogSink.hpp"

class LogManager
{
private:
    ILogSink* logSink;
    std::vector<LogMessage*> message;
public:
    LogManager(/* args */);
    ~LogManager();
    void routeMessage(const LogMessage& msg);
};