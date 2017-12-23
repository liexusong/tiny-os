#ifndef __IDT_H
#define __IDT_H

#include <types.h>

void init_idt();

typedef struct {
	uint16_t base_lo;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_hi;
} __attribute__ ((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed)) idt_ptr_t;

#endif
