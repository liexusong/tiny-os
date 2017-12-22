#ifndef __COMMON_H
#define __COMMON_H

#include <types.h>

static inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %1, %0" : : "dN"(port), "a"(value));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;

	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));

	return ret;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;

	__asm__ volatile ("inw %1, %0" : "=a"(ret) : "dN"(port));

	return ret;
}

#endif
