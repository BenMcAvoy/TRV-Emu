#ifndef LOGGING_HPP
#define LOGGING_HPP

#define LOG_DEBUG(format, ...) { \
    printf("[DEBUG] "); \
    printf(format, ##__VA_ARGS__); \
    printf("\n"); \
}

#define LOG_INFO(format, ...) { \
    printf("[INFO] "); \
    printf(format, ##__VA_ARGS__); \
    printf("\n"); \
}

#define LOG_WARN(format, ...) { \
    printf("[WARN] "); \
    printf(format, ##__VA_ARGS__); \
    printf("\n"); \
}

#define LOG_ERROR(format, ...) { \
    printf("[ERROR] "); \
    printf(format, ##__VA_ARGS__); \
    printf("\n"); \
}

#endif