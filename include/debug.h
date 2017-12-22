#ifndef __DEBUG_H
#define __DEBUG_H

#include <console.h>
#include <vargs.h>
#include <elf.h>

#define assert(x, info)                          \
	do {                                         \
		if (!(x)) {                              \
			panic(info);                         \
		}                                        \
	} while (0)

#define static_assert(x)                         \
	switch (x) { case 0: case (x): ; }

void init_debug();
void panic(const char *info);
void print_cur_status();

#endif
