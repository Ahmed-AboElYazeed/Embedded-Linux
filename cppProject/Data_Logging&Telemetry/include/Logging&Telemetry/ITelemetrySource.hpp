// ITelemetrySource.hpp
#pragma once
#include <string>


/********************************************************************************
```

No implementation goes here — it's just the contract every source must honour.

---
*********************************************************************************/

class ITelemetrySource
{
public:
    virtual ~ITelemetrySource() = default;

    // Open the source (file, socket, etc.)
    // Returns true on success, false on failure
    virtual bool openSource() = 0;

    // Read latest data into `out`
    // Returns true if a valid reading was stored, false otherwise
    virtual bool readSource(std::string& out) = 0;
};
