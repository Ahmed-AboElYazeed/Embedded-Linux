// SocketTelemetrySourceImpl.hpp
#pragma once
#include "Logging&Telemetry/ITelemetrySource.hpp"
#include "Logging&Telemetry/SafeSocket.hpp"

class SocketTelemetrySourceImpl : public ITelemetrySource
{
private:
    SafeClientSocket source_socket;   // RAII member — Rule of Zero applies

public:
    SocketTelemetrySourceImpl() = delete;
    explicit SocketTelemetrySourceImpl(const std::string& socket_path);

    ~SocketTelemetrySourceImpl()                                         = default;
    SocketTelemetrySourceImpl(const SocketTelemetrySourceImpl&)          = delete;
    SocketTelemetrySourceImpl& operator=(const SocketTelemetrySourceImpl&) = delete;
    SocketTelemetrySourceImpl(SocketTelemetrySourceImpl&&)               = default;
    SocketTelemetrySourceImpl& operator=(SocketTelemetrySourceImpl&&)    = default;

    bool openSource()                 override;
    bool readSource(std::string& out) override;
};