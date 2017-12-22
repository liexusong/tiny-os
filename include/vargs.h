#ifndef __VARGS_H
#define __VARGS_H

#include <types.h>

#define va_list            char *
#define va_start(p, first) (p = (va_list)&first + sizeof(first))
#define va_arg(p, type)    (*(type *)((p += sizeof(type)) - sizeof(type)))
#define va_end(p)          (p = (va_list)NULL)

#endif
