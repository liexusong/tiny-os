#ifndef __PRINTK_H
#define __PRINTK_H

#include <vargs.h>

void printk(const char *format, ...);
int vsprintf(char *buf, const char *format, va_list args);

#endif
