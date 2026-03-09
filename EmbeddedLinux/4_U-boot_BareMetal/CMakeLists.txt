cmake_minimum_required(VERSION 3.16)

# Tell CMake we are cross-compiling — must be set BEFORE project()
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Toolchain binaries
set(CROSS /home/zee/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu)
set(CMAKE_C_COMPILER       ${CROSS}-gcc)
set(CMAKE_ASM_COMPILER     ${CROSS}-gcc)
set(CMAKE_OBJCOPY          ${CROSS}-objcopy)

# Stop CMake from testing the compiler (it can't run AArch64 binaries on host)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(rpi3_blink LANGUAGES C ASM)

# ── Compiler flags ────────────────────────────────────────────────
set(BARE_METAL_FLAGS
    -ffreestanding
    -nostdlib
    -nostartfiles
    -march=armv8-a
    -O2
    -Wall
)

# ── Source files ──────────────────────────────────────────────────
set(SOURCES
    boot.S
    gpio.c
    timer.c
    main.c
)

# ── ELF target ────────────────────────────────────────────────────
add_executable(mybaremetalapp.elf ${SOURCES})

target_compile_options( mybaremetalapp.elf PRIVATE ${BARE_METAL_FLAGS})

target_link_options(mybaremetalapp.elf PRIVATE
    ${BARE_METAL_FLAGS}
    -T ${CMAKE_SOURCE_DIR}/linker.ld
)

# ── Strip ELF → raw binary (.img) ─────────────────────────────────
add_custom_command(
    TARGET  mybaremetalapp.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary mybaremetalapp.elf mybaremetalapp.img
    COMMENT "→ Generating mybaremetalapp.img   (strip ELF to raw binary)"
)

# ── 'make clean' also removes the .img ───────────────────────────
set_property(
    TARGET mybaremetalapp.elf
    APPEND PROPERTY ADDITIONAL_CLEAN_FILES mybaremetalapp.img
)