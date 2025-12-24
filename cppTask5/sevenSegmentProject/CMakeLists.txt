cmake_minimum_required(VERSION 3.18)

# ===============================
# Cross Compilation Configuration
# ===============================
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Toolchain root (adjust if needed)
set(TOOLCHAIN_ROOT $ENV{HOME}/x-tools/aarch64-rpi3-linux-gnu)

# Compilers
set(CMAKE_C_COMPILER   ${TOOLCHAIN_ROOT}/bin/aarch64-rpi3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_ROOT}/bin/aarch64-rpi3-linux-gnu-g++)

# Sysroot
set(CMAKE_SYSROOT ${TOOLCHAIN_ROOT}/aarch64-rpi3-linux-gnu/sysroot)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# ===============================
# Project
# ===============================
project(SevenSegmentProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===============================
# Include directories
# ===============================
include_directories(${PROJECT_SOURCE_DIR}/include)

# ===============================
# Static Library
# ===============================
add_library(mylib STATIC
    src/gpio.cpp
    src/Stream.cpp
    src/IStream.cpp
    src/OStream.cpp
    src/IOStream.cpp
    src/SevenSegment.cpp
)

target_include_directories(mylib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# ===============================
# Executable
# ===============================
add_executable(sevenSegmentApp
    app/main.cpp
)

target_link_libraries(sevenSegmentApp
    mylib
)

# ===============================
# Deploy to Raspberry Pi
# ===============================
    #use this commmand to run the next deploy target
    # cmake --build build --target deploy
set(RPI_USER zeerbp)
set(RPI_IP 192.168.87.153)
set(RPI_PATH /home/zeerbp/fromPc/sevenSegment)

add_custom_target(deploy
    DEPENDS sevenSegmentApp
    COMMAND ssh ${RPI_USER}@${RPI_IP} "mkdir -p ${RPI_PATH}"
    COMMAND scp $<TARGET_FILE:sevenSegmentApp> ${RPI_USER}@${RPI_IP}:${RPI_PATH}
    COMMENT "Copying executable to Raspberry Pi"
)
