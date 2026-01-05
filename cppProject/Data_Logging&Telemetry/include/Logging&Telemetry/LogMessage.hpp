#pragma once
#include <string>

class LogMessage
{
private:
    std::string appName = "app1",
                context = "context", 
                time = "2001-10-15,10:50:03", 
                severity = "severity", 
                text = "default log message";
public:
    LogMessage(/* args */);
    LogMessage(std::string text);
    ~LogMessage();
    void makeMessage(const std::string& appName,
                     const std::string& context,
                     const std::string& time,
                     const std::string& severity,
                     const std::string& text);
    std::string getMessage() const;
};
