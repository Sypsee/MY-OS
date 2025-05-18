#include "logger.h"

void log(lvl_t lvl, const char* fmt, ...)
{
    const char* prefix = "\033[91m[NO PREFIX]";

    switch (lvl)
    {
        case INFO: prefix = "\033[92m[INFO]"; break;
        case WARN: prefix = "\033[93m[WARN]"; break;
        case ERROR: prefix = "\033[91m[ERROR]"; break;
        case PANIC: prefix = "\033[31m[PANIC]"; break;
    }

    va_list args;
    va_start(args, fmt);
    printf("%s \033[0m", prefix);
    vprintf(fmt, args);
    va_end(args);
}
