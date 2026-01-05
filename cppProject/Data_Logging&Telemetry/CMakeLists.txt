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

cmake_minimum_required(VERSION 3.14)

project(LoggingAndTelemetry VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/Logging&Telemetry/ConsoleSinkImpl.cpp
    src/Logging&Telemetry/FileSinkImpl.cpp
    src/Logging&Telemetry/ILogSink.cpp
    src/Logging&Telemetry/LogManager.cpp
    src/Logging&Telemetry/LogMessage.cpp
    src/main.cpp
)

# Header files (for IDE support)
set(HEADERS
    include/Logging&Telemetry/ConsoleSinkImpl.hpp
    include/Logging&Telemetry/FileSinkImpl.hpp
    include/Logging&Telemetry/ILogSink.hpp
    include/Logging&Telemetry/LogManager.hpp
    include/Logging&Telemetry/LogMessage.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================================
# Google Test Integration
# ============================================================================

# this next command will automaticly turn on the unit test file generation.
# option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    # Find pre-installed Google Test
    find_package(Threads REQUIRED)
    find_package(GTest REQUIRED)
    
    # Enable testing
    enable_testing()
    
    # Add test subdirectory if it exists and has CMakeLists.txt
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
        add_subdirectory(test)
    endif()
endif()

# ============================================================================
# Installation (optional)
# ============================================================================

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)