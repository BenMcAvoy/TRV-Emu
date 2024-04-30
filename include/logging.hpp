#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "globals.cpp"
#include <string>

#include <cstdarg>
#include <cstdio>
#include <cstring>

#define LOG_DEBUG(format, ...) { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    if (globals::logToTerm) \
        printf("[DEBUG] %s\n", buffer); \
    globals::logContent += "[DEBUG] " + std::string(buffer) + "\n"; \
}

#define LOG_INFO(format, ...) { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    if (globals::logToTerm) \
		printf("[INFO] %s\n", buffer); \
    globals::logContent += "[INFO] " + std::string(buffer) + "\n"; \
}

#define LOG_WARN(format, ...) { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    if (globals::logToTerm) \
		printf("[WARN] %s\n", buffer); \
    globals::logContent += "[WARN] " + std::string(buffer) + "\n"; \
}

#define LOG_ERROR(format, ...) { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    if (globals::logToTerm) \
		printf("[ERROR] %s\n", buffer); \
    globals::logContent += "[ERROR] " + std::string(buffer) + "\n"; \
}

#endif
