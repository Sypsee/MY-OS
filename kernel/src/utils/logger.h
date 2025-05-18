#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <libs/printf.h>

enum lvl_t
{
    INFO,
    WARN,
    ERROR,
    PANIC
};

void log(lvl_t lvl, const char* fmt, ...);