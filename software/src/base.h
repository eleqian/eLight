#ifndef __BASE_H__
#define __BASE_H__

#include <string.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed int s16;
typedef signed long s32;

#ifndef BOOL
#define BOOL u8
#define TRUE 1
#define FALSE 0
#endif

typedef u16 tick_t;

#ifndef ROM
#define ROM code
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef HIGH8
#define HIGH8(v) (u8)((v) >> 8)
#endif

#ifndef LOW8
#define LOW8(v) (u8)(v)
#endif

#ifndef UNUSED
#define UNUSED(v) (v) = (v)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#endif
