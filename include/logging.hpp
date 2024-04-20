#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

#define SETCOLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

#define LOG_DEBUG(format, ...) { \
    SYSTEMTIME st; \
    GetLocalTime(&st); \
    SETCOLOR(9); printf("[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); SETCOLOR(7); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_INFO(format, ...) { \
	SYSTEMTIME st; \
	GetLocalTime(&st); \
	SETCOLOR(10); printf("[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); SETCOLOR(7); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_WARN(format, ...) { \
	SYSTEMTIME st; \
	GetLocalTime(&st); \
	SETCOLOR(14); printf("[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); SETCOLOR(7); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_ERROR(format, ...) { \
    SYSTEMTIME st; \
    GetLocalTime(&st); \
    SETCOLOR(12); printf("[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); SETCOLOR(7); \
    printf(format, ##__VA_ARGS__); \
}
#elif __linux__

#include <stdio.h>
#include <time.h>

#define SETCOLOR(color) printf("\033[%dm", color)

#define LOG_DEBUG(format, ...) { \
    time_t rawtime; \
    struct tm *timeinfo; \
    time(&rawtime); \
    timeinfo = localtime(&rawtime); \
    SETCOLOR(34); printf("[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); SETCOLOR(0); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_INFO(format, ...) { \
    time_t rawtime; \
    struct tm *timeinfo; \
    time(&rawtime); \
    timeinfo = localtime(&rawtime); \
    SETCOLOR(32); printf("[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); SETCOLOR(0); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_WARN(format, ...) { \
    time_t rawtime; \
    struct tm *timeinfo; \
    time(&rawtime); \
    timeinfo = localtime(&rawtime); \
    SETCOLOR(33); printf("[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); SETCOLOR(0); \
    printf(format, ##__VA_ARGS__); \
}

#define LOG_ERROR(format, ...) { \
    time_t rawtime; \
    struct tm *timeinfo; \
    time(&rawtime); \
    timeinfo = localtime(&rawtime); \
    SETCOLOR(31); printf("[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); SETCOLOR(0); \
    printf(format, ##__VA_ARGS__); \
}
#else
// If we are not a custom implented platform, we will simply print the messages.
#define LOG_INFO(...) { \
    printf([UNKNOWN], __VA_ARGS__); \
}

#define LOG_WARN(...) { \
    printf([UNKNOWN], __VA_ARGS__); \
}

#define LOG_ERROR(...) { \
    printf([UNKNOWN], __VA_ARGS__); \
}
#endif // _WIN32
#endif // LOGGING_HPP
