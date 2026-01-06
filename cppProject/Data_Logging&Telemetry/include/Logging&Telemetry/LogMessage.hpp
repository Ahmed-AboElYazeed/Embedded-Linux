#pragma once
#include <string>
#include <iostream>

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
    // std::ostream& operator<<(std::ostream& out);
    
// implementing the global operator<< overloading with LogMessage input:
    // Declare the overloaded operator<< as a friend function
    // to allow it access to private members (like message_content)
    // friend also to allow douple inputs (allow calling external frind function)
    friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);  
};