#include "strings.h"

size_t strlen(char const* s)
{
    char const* p = s;
    while(*p != 0)
        ++p;
    return p - s;
}

