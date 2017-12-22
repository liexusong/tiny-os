#ifndef __MATH_H
#define __MATH_H

#define do_div(n, base) ({                                                 \
	int __res;                                                             \
	__asm__("divl %4" : "=a"(n), "=d"(__res) : "0"(n), "1"(0), "r"(base)); \
	__res;})

#endif
