cmake_minimum_required(VERSION 3.21)
project(gb_emulator C)

# Force C23 standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(SOURCES cpu/cpu.h cpu/main.c)

include_directories(cpu)

# Define executable target
add_executable(gb_emulator ${SOURCES})

# Strict warning flags (Werror, extra warnings, and pedantic checks)
if(MSVC)
    target_compile_options(gb_emulator PRIVATE /W4 /WX)
else()
    target_compile_options(gb_emulator PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wshadow
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wconversion
	-Wno-unused-parameter
	-Wno-unused-variable
    )
endif()
