#include "Logging&Telemetry/ConsoleSinkImpl.hpp"

ConsoleSinkImpl::ConsoleSinkImpl(/* args */) = default;
ConsoleSinkImpl::~ConsoleSinkImpl() = default;

void ConsoleSinkImpl::write (const LogMessage& msg)
{
    // write to console
    std::cout << msg.getMessage();
}