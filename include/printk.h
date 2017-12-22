#ifndef __PRINTK_H
#define __PRINTK_H

#include <vargs.h>
#include <console.h>

void printk(const char *format, ...);
void cprintk(real_color_t color, const char * format, ...);
int vsprintf(char *buf, const char *format, va_list args);

#endif
