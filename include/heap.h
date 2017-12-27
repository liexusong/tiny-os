#ifndef __HEAP_H
#define __HEAP_H

#include <types.h>

#define HEAP_START_ADDR 0xE0000000

typedef struct heap_header_s {
	struct heap_header_s *prev;
	struct heap_header_s *next;
	uint32_t allocated:1;
	uint32_t length:31;
} heap_header_t;

void init_heap();
void *kmalloc(uint32_t len);
void kfree(void *p);

#endif
