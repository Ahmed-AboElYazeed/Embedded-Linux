#include <iostream>
#include "Logging&Telemetry/ILogSink.hpp"
#include "Logging&Telemetry/LogMessage.hpp"
#include "Logging&Telemetry/LogManager.hpp"
#include "Logging&Telemetry/FileSinkImpl.hpp"
#include "Logging&Telemetry/ConsoleSinkImpl.hpp"



int main() {
    std::cout << "starting\n";
    LogMessage msg("first log message from main");

    ILogSink *fileSink = new FileSinkImpl();
    fileSink->write(msg);
    delete fileSink;
    std::cout << "finish file writing\n";

    ILogSink *consolSink = new ConsoleSinkImpl();
    consolSink->write(msg);
    delete consolSink;
    std::cout << "finish console writing\n";

    return 0;
}