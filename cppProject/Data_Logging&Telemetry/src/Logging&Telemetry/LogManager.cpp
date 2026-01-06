#include "Logging&Telemetry/LogManager.hpp"

// Constructor & Destructor was set to default to handel Vestor -> rule of 0
LogManager::LogManager(/* args */) = default;
LogManager::~LogManager() = default;  

void LogManager::addSink (const ILogSink& logSink)
{
    logSinks.push_back(&logSink);
}
void LogManager::addMessage (const LogMessage& msg)
{
    messages.push_back(msg);
}

void LogManager::writeAll ()
{
    for (unsigned long i=0; i<logSinks.size(); i++)
    {
        for (unsigned long m=0; m<messages.size(); m++)
        {
            logSinks.at(i)->write(messages.at(m));
        }
    }
}
void LogManager::writeOne (const LogMessage& msg)
{
    for (unsigned long i=0; i<logSinks.size(); i++)
    {
        logSinks.at(i)->write(msg);
    }
}

void LogManager::operator<<(std::string messageText)
{
    LogMessage msg(messageText);
    addMessage(msg);
}
