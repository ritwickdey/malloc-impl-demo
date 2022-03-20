
#include <stdio.h>
#include <stdarg.h>


void debug_log(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    fprintf(stdout, "[DEBUG] ");
    vfprintf(stdout, format, argList);
    va_end(argList);
    fflush(stdout);
}