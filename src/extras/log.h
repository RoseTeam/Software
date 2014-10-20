#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef enum
{
LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG
    }LOG_LEVEL;

void log_init(void);

void log_print(LOG_LEVEL lvl, char* file, char* func, int line, char *fmt, ... );

#define LOG(LVL,MSG,...) do {						\
			     log_print(LVL, __FILE__, __func__, __LINE__, MSG, ##__VA_ARGS__); \
			     } while(0)

#endif
