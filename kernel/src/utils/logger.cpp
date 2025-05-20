#include "logger.h"

void log(lvl_t lvl, const char* fmt, ...)
{
    const char* prefix = "\033[91m[NO PREFIX]";

    switch (lvl)
    {
        case INFO: prefix = "\033[97;42m[INFO]\033[0m"; break;
        case WARN: prefix = "\033[97;43m[WARN]\033[0m"; break;
        case ERROR: prefix = "\033[97;101m[ERROR]\033[0m"; break;
        case PANIC: prefix = "\033[97;41m[PANIC]\033[0m"; break;
    }

    va_list args;
    va_start(args, fmt);
    printf("%s ", prefix);
    vprintf(fmt, args);
    va_end(args);
}
