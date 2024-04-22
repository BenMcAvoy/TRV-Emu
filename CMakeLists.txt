# Project configuration
cmake_minimum_required(VERSION 3.5...3.19)
project(TRV-Emu)

# Lib paths
set(IMGUI_PATH ${PROJECT_SOURCE_DIR}/libs/imgui)
set(IMGUI_CLUB_PATH ${PROJECT_SOURCE_DIR}/libs/imgui_club)
set(GLFW_PATH ${PROJECT_SOURCE_DIR}/libs/glfw)

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 20)

# Configure GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_PATH})

# Configure ImGui
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH} ${GLFW_PATH}/include ${IMGUI_CLUB_PATH}/imgui_memory_editor)

# Build the project
add_executable(TRV-Emu src/main.cpp)

# Link with GLFW and ImGui
target_link_libraries(TRV-Emu glfw imgui)

# Add include dir
target_include_directories(TRV-Emu PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# Link with OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(TRV-Emu OpenGL::GL)

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
