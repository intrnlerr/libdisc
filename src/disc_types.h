#ifndef LIBDISC_TYPES_H
#define LIBDISC_TYPES_H

#include <stdint.h>

#if defined(_MSC_VER)
#define LIBDISC_EXPORT __declspec(dllexport)
#else
#define LIBDISC_EXPORT extern
#endif


#define LIBDISC_URL "https://github.com/intrnlerr/libdisc"
#define LIBDISC_VER "0.1"

typedef uint64_t disc_snowflake;

#endif