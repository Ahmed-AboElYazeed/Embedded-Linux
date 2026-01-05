# cmake commands to build the project:
    #  cmake -S . -B build
    #  cmake --build build
# to run the built executable:
    #  ./build/bin/LoggingAndTelemetry

# to build with tests:
    #  cmake -S . -B build -DBUILD_TESTS=ON
    #  cmake --build build
# to run tests (if built with tests):
    #  ./build/bin/LoggingTests



# Test source files - add your test files here as you create them
set(TEST_SOURCES
    test_LogManager.cpp
    # test_LogMessage.cpp
    # test_ConsoleSink.cpp
    # Add more test files here
)

# Create test executable
add_executable(LoggingTests ${TEST_SOURCES})

# Link against Google Test
target_link_libraries(LoggingTests
    PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    GTest::gtest
    GTest::gmock
    Threads::Threads
)

# Include project headers
target_include_directories(LoggingTests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Add the project source files (excluding main.cpp) to the test executable
target_sources(LoggingTests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/Logging&Telemetry/ConsoleSinkImpl.cpp
    ${PROJECT_SOURCE_DIR}/src/Logging&Telemetry/FileSinkImpl.cpp
    ${PROJECT_SOURCE_DIR}/src/Logging&Telemetry/ILogSink.cpp
    ${PROJECT_SOURCE_DIR}/src/Logging&Telemetry/LogManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Logging&Telemetry/LogMessage.cpp
)

# Set C++ standard for tests
set_target_properties(LoggingTests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(LoggingTests)