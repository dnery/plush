#ifndef SCRAMBLE_DEBUG_H
#define SCRAMBLE_DEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifndef RELEASE

#define info(exp) do { \
        fprintf(stdout, "Info: " exp); \
} while (0) \

#define warn(exp) do { \
        fprintf(stdout, "Warning: " exp); \
} while (0) \

#define check(exp) do { if (!(exp)) { \
        fprintf(stderr, "%s: %d: %s\n", __FILE__, __LINE__, strerror(errno)); \
        exit(errno); \
} } while (0) \

#else

#define info(exp)

#define warn(exp)

#define check(exp)

#endif

#ifdef __cplusplus
}
#endif

#endif /* SCRAMBLE_DEBUG_H */
