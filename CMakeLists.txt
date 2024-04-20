# Project configuration
cmake_minimum_required(VERSION 3.5...3.19)
project(TRV-Emu)

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 20)

# Build the project
add_executable(TRV-Emu src/main.cpp)

if (UNIX)
	# Enable all warnings and treat them as errors
	target_compile_options(TRV-Emu PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if (MSVC)
	# Enable all warnings and treat them as errors
	target_compile_options(TRV-Emu PRIVATE /W4 /WX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()
