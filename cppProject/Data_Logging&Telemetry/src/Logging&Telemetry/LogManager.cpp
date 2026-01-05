#include "Logging&Telemetry/LogManager.hpp"

LogManager::LogManager(/* args */) = default;
LogManager::~LogManager() = default;  

void LogManager::routeMessage(const LogMessage& msg)
{
    logSink->write(msg);   // implementing behavioral design pattern: Strategy.
}