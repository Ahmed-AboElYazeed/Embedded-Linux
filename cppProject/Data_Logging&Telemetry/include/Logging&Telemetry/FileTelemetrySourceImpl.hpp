#pragma once

#include "Logging&Telemetry/ITelemetrySource.hpp"
#include "Logging&Telemetry/SafeFile.hpp"

class FileTelemetrySourceImpl : public ITelemetrySource
{
private:
    SafeFile source_file;   // RAII member — Rule of Zero applies
public:
    FileTelemetrySourceImpl() = delete;
    explicit FileTelemetrySourceImpl(const std::string& path);

    // Rule of Zero — let compiler handle all special members
    ~FileTelemetrySourceImpl()                                       = default;
    FileTelemetrySourceImpl(const FileTelemetrySourceImpl&)          = delete; // SafeFile is non-copyable
    FileTelemetrySourceImpl& operator=(const FileTelemetrySourceImpl&) = delete;
    FileTelemetrySourceImpl(FileTelemetrySourceImpl&&)               = default;
    FileTelemetrySourceImpl& operator=(FileTelemetrySourceImpl&&)    = default;

    bool openSource()              override;
    bool readSource(std::string& out) override;
};
