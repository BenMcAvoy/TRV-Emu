# Project configuration
cmake_minimum_required(VERSION 3.5...3.19)
project(TRV-Emu)

add_executable(TRV-Emu src/main.cpp)

# Download dependencies with `FetchContent`
include(FetchContent)
find_package(OpenGL)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/)
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)
    add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw)
endif ()

FetchContent_Declare(imgui_club GIT_REPOSITORY https://github.com/ocornut/imgui_club.git GIT_TAG main)
FetchContent_makeAvailable(imgui_club)

# We are not going to be using battery-embed for now
# FetchContent_Declare(battery-embed GIT_REPOSITORY https://github.com/batterycenter/embed.git GIT_TAG main)
# FetchContent_MakeAvailable(battery-embed)

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 20)

# Configure GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Link with dependencies
find_package(OpenGL REQUIRED)
target_link_libraries(TRV-Emu PRIVATE glfw imgui OpenGL::GL)

# Add include dir
#target_include_directories(TRV-Emu PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${PROJECT_SOURCE_DIR}/src)
target_include_directories(TRV-Emu PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/src ${imgui_INCLUDE_DIRS} ${imgui_club_SOURCE_DIR}/imgui_memory_editor)

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
