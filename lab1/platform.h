#if defined(__APPLE__) || defined(__MACH__) || defined(__unix__) || defined(__unix)

#include<unistd.h>
#include<sys/types.h>

#define ftrunc(fd, len) ftruncate(fd, len)

#endif