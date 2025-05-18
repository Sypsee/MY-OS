#include <stdint.h>
#include <stdarg.h>
#include <libs/printf.h>

enum lvl_t
{
    INFO,
    WARN,
    ERROR,
};

void log(lvl_t lvl, const char* fmt, ...)
{
    const char* prefix = "\033[91m[NO PREFIX]";

    switch (lvl)
    {
        case INFO: prefix = "\033[92m[INFO]"; break;
        case WARN: prefix = "\033[93m[WARN]"; break;
        case ERROR: prefix = "\033[91m[ERROR]"; break;
    }

    va_list args;
    va_start(args, fmt);
    printf("%s \033[0m", prefix);
    vprintf(fmt, args);
    va_end(args);
}
