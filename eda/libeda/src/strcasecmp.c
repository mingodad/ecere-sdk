#include <string.h>

// TODO: Improve how this is resolved... output in .main.c?
#if defined(__clang__) && defined(_MSC_VER)
int strcasecmp(const char * a, const char * b)
{
   return strcmpi(a, b);
}

int strncasecmp(const char * a, const char * b, size_t n)
{
   return strnicmp(a, b, n);
}
#endif
