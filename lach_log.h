#ifndef LACH_LOG_H
#define LACH_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <time.h>


typedef enum {
    // Indicates that the message should always be logged regardless of other settings.
    LLOG_ALWAYS,

    // Severe errors that will lead the application to abort or terminate. These are critical events that usually signify the need for immediate attention.
    LLOG_FATAL,

    // Error events that might allow the application to continue running, but something failed or went wrong in the process.
    LLOG_ERROR,

    // Potentially harmful situations that aren't necessarily errors but may require attention. These indicate that something unexpected happened but the application is still running.
    LLOG_WARN,

    // Informational messages that highlight the progress of the application in a general sense, such as startup or shutdown messages.
    LLOG_INFO,

    // General debugging information useful during development, but not usually needed in production environments.
    LLOG_DEBUG,

    // Detailed debug information, typically used for tracing the flow of the application at a fine-grained level.
    LLOG_TRACE
} LLogLevel;

const char *lach_log_level_str[] = {
    "ALWAYS",
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
    "TRACE"
};

void lach_log(LLogLevel level, const char *format, ...)
{
    time_t raw_time;
    struct tm time_info;

    time(&raw_time);

    if (localtime_s(&time_info, &raw_time) == 0) {
        char time_buffer[20];
        fprintf(stderr, "Error converting time to local time.\n");
        strftime(time_buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(stderr, "[%s] [%s] ", time_buffer, log_level_str[level]);
    } else {
        fprintf(stderr, "[TIMESTAMP FAIL] [%s] ", log_level_str[level]);
    }

    // Handle the variable argument list
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

#ifdef __cplusplus
}
#endif

#endif // LACH_LOG_H