#ifndef PRINTF_HOOK_HPP
#define PRINTF_HOOK_HPP

#include "globals.cpp"
#include <iostream>
#include <cstdarg>

typedef int (*printf_type)(const char*, ...);

int printf_hook(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    globals::logContent += buffer;

    int ret = vprintf(format, args);
    va_end(args);
    return ret;
}

#define printf printf_hook

#endif
