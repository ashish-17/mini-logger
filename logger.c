#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "logger.h"

#define LOGGER_BUF_SIZE 1024
#define LOGGER_PREFIX_MAX_SIZE 7

#define LOG_STRING_INFO "INFO"
#define LOG_STRING_WARN "WARN"
#define LOG_STRING_ERROR "ERROR"
#define LOG_STRING_DEBUG "DEBUG"
#define LOG_STRING_VERBOSE "VERBOSE"
#define LOG_STRING_PROLOG "PROLOG"
#define LOG_STRING_EPILOG "EPILOG"

void getDateTime(struct tm *const tm, unsigned *const usec);
void getProcessAndThreadId(int *const pid, int *const tid);
void getPrefix(char* prefix, int log_level, int prefixId);

void _log_write(const char *const func,
                const char *const file,
                const unsigned line,
                const int level,
                const int prefixId,
                const char *const fmt, ...) {
	char output[LOGGER_BUF_SIZE] = "\0";
	char message[LOGGER_BUF_SIZE] = "\0";
    char prefix[LOGGER_PREFIX_MAX_SIZE] = "\0";


    switch(level) {
        case LOG_LEVEL_VERBOSE: {
                switch(prefixId) {
                    case LOG_PREFIX_PROLOG:
                    case LOG_PREFIX_EPILOG: {
                        sprintf(message, "%s()", func);
                        break;
                    }

                    default: {
                        va_list va;
                        va_start(va, fmt);
                        sprintf(message, fmt, va);
                        va_end(va);

                        break;
                    }
                }

                break;
            }

        default: {
            va_list va;
            va_start(va, fmt);
            vsprintf(message, fmt, va);
            va_end(va);

            break;
        }
    }

    struct tm tm;
	unsigned usec = 0;
	int pid = 0;
	int tid = 0;

	getDateTime(&tm, &usec);
	getProcessAndThreadId(&pid, &tid);
    getPrefix(prefix, level, prefixId);

    sprintf(output,
            "%04u-%02u-%02u %02u:%02u:%02u.%03u [%5i - %5i] %6s %s",
            (unsigned) tm.tm_year+1900,
            (unsigned)tm.tm_mon,
            (unsigned)tm.tm_mday,
            (unsigned)tm.tm_hour,
            (unsigned)tm.tm_min,
            (unsigned)tm.tm_sec,
            (unsigned)(usec / 1000),
            pid,
            tid,
            prefix,
            message);

    printf("\n%s", output);
}

void getDateTime(struct tm *const tm, unsigned *const usec) {
	struct timeval tv;
	gettimeofday(&tv, 0);
	const time_t t = tv.tv_sec;
	*tm = *localtime(&t);
	*usec = tv.tv_usec;
}

void getProcessAndThreadId(int *const pid, int *const tid) {
	*pid = getpid();
#if defined(__linux__)
	*tid = syscall(SYS_gettid);
#elif defined(__MACH__)
	*tid = pthread_mach_thread_np(pthread_self());
#else
	*tid= 0;
#endif
}

void getPrefix(char* prefix, int log_level, int prefixId) {
    if (prefix != NULL) {
        switch(log_level) {
            case LOG_LEVEL_INFO: {
                strncat(prefix, LOG_STRING_INFO, (LOGGER_PREFIX_MAX_SIZE - 1));
                break;
            }

            case LOG_LEVEL_WARN: {
                strncat(prefix, LOG_STRING_WARN, (LOGGER_PREFIX_MAX_SIZE - 1));
                break;
            }

            case LOG_LEVEL_ERROR: {
                strncat(prefix, LOG_STRING_ERROR, (LOGGER_PREFIX_MAX_SIZE - 1));
                break;
            }

            case LOG_LEVEL_DEBUG: {
                strncat(prefix, LOG_STRING_DEBUG, (LOGGER_PREFIX_MAX_SIZE - 1));
                break;
            }

            case LOG_LEVEL_VERBOSE: {
                switch(prefixId) {
                    case LOG_PREFIX_PROLOG: {
                        strncat(prefix, LOG_STRING_PROLOG, (LOGGER_PREFIX_MAX_SIZE - 1));
                        break;
                    }

                    case LOG_PREFIX_EPILOG: {
                        strncat(prefix, LOG_STRING_EPILOG, (LOGGER_PREFIX_MAX_SIZE - 1));
                        break;
                    }

                    default: {
                        strncat(prefix, LOG_STRING_VERBOSE, (LOGGER_PREFIX_MAX_SIZE - 1));
                        break;
                    }
                }

                break;
            }
        }
    }
}

