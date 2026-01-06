#include "Logging&Telemetry/ConsoleSinkImpl.hpp"

ConsoleSinkImpl::ConsoleSinkImpl(/* args */) = default;
ConsoleSinkImpl::~ConsoleSinkImpl() = default;

void ConsoleSinkImpl::write (const LogMessage& msg) const
{
    // write to console
    std::cout << msg.getMessage();
}
void ConsoleSinkImpl::operator<<(const LogMessage& msg) const
{
    std::cout << msg.getMessage();
}