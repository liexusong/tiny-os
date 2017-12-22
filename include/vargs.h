#ifndef __VARGS_H
#define __VARGS_H

#include <types.h>

#ifdef __GNUC__

typedef __builtin_va_list va_list;

#define va_start(p, first) (__builtin_va_start(p, first))
#define va_arg(p, type)    (__builtin_va_arg(p, type))
#define va_end(p)

#else

#define va_list            char *
#define va_start(p, first) (p = (va_list)&first + sizeof(first))
#define va_arg(p, type)    (*(type *)((p += sizeof(type)) - sizeof(type)))
#define va_end(p)          (p = (va_list)NULL)

#endif

#endif
