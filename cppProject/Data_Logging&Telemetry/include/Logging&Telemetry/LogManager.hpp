#pragma once
#include <vector>
#include "Logging&Telemetry/LogMessage.hpp"
#include "Logging&Telemetry/ILogSink.hpp"

class LogManager
{
private:
    std::vector<const ILogSink*> logSinks;
    std::vector<LogMessage> messages;
public:
    LogManager(/* args */);
    ~LogManager();
    void addSink (const ILogSink& logSink);
    void addMessage (const LogMessage& msg);
    void writeOne (const LogMessage& msg);
    void writeAll ();
    void operator<<(std::string messageText);
};