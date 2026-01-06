#include "Logging&Telemetry/LogMessage.hpp"


LogMessage::LogMessage(/* args */) = default;
// LogMessage::LogMessage(std::string text) : text(std::move(text)) {}
LogMessage::LogMessage(std::string text)
{
    this->text = text;
}
LogMessage::~LogMessage() = default;

void LogMessage::makeMessage(const std::string& appName,
                             const std::string& context,
                             const std::string& time,
                             const std::string& severity,
                             const std::string& text)
{
    this->appName = appName;
    this->context = context;
    this->time = time;
    this->severity = severity;
    this->text = text;
}
std::string LogMessage::getMessage() const
{
    return " [" + appName + "] [" + context + "] [" + time + "] [" + severity + "] [" + text + "]\n";
}

// implementing the global operator<< overloading with LogMessage input:
std::ostream& operator<<(std::ostream& out, const LogMessage& msg)
{
    out << msg.getMessage();
    return out;
}