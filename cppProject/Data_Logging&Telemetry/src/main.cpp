#include <iostream>
#include "Logging&Telemetry/ILogSink.hpp"
#include "Logging&Telemetry/LogMessage.hpp"
#include "Logging&Telemetry/LogManager.hpp"
#include "Logging&Telemetry/FileSinkImpl.hpp"
#include "Logging&Telemetry/ConsoleSinkImpl.hpp"



int main() {
    std::cout << "starting\n";
    
    ILogSink *fileSink = new FileSinkImpl();
    ILogSink *consolSink = new ConsoleSinkImpl();
    
    LogManager logManager;
    logManager.addSink(*fileSink);
    logManager.addSink(*static_cast<ILogSink*>(new FileSinkImpl()));
    logManager.addSink(*consolSink);
    logManager.addSink(*static_cast<ILogSink*>(new ConsoleSinkImpl()));
    
    LogMessage msg("All correct log message from main");
    logManager.addMessage(msg);
    logManager.addMessage(*(new LogMessage("second message")));
    logManager << "third message";
    logManager.writeAll();
    std::cout << "finish writing\n";
    
    std::cout << msg << LogMessage("test message") << "\n";

    delete fileSink;
    delete consolSink;
    return 0;
}




/*      // first test  (the code changed)

    std::cout << "starting\n";
    LogMessage msg("first log message from main");

    ILogSink *fileSink = new FileSinkImpl();
    // fileSink->write(msg);    //reference call
    *fileSink << msg;           //operator overload call
    delete fileSink;
    std::cout << "finish file writing\n";

    ILogSink *consolSink = new ConsoleSinkImpl();
    // consolSink->write(msg);
    *consolSink << msg;
    delete consolSink;
    std::cout << "finish console writing\n";
*/

/*      // second test

    std::cout << "starting\n";
    LogMessage msg("first correct log message from main");

    ILogSink *fileSink = new FileSinkImpl();
    ILogSink *consolSink = new ConsoleSinkImpl();
    
    LogManager logManager;
    // logManager.addSink(*static_cast<ILogSink*>(new FileSinkImpl()));
    logManager.addSink(*fileSink);
    logManager.addSink(*consolSink);
    logManager.addSink(*consolSink);
    
    logManager.writeOne(msg);
    std::cout << "finish writing\n";

    delete fileSink;
    delete consolSink;
*/