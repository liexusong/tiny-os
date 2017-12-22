#include <gdt.h>

#define GDT_NUM 5

gdt_entry_t gdt_entries[GDT_NUM];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(int32_t idx, uint32_t base, 
	    uint32_t limit, uint8_t access, uint8_t gran);

extern uint32_t stack;

void init_gdt()
{
	gdt_ptr.limit = sizeof(gdt_entries) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data segment

	gdt_flush((uint32_t)&gdt_ptr);
}

static void gdt_set_gate(int32_t idx, uint32_t base, 
	    uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[idx].base_low = base & 0xFFFF;
	gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
	gdt_entries[idx].base_high = (base >> 24) & 0xFF;

	gdt_entries[idx].limit_low = limit & 0xFFFF;
	gdt_entries[idx].granularity = (limit >> 16) & 0x0F;

	gdt_entries[idx].granularity |= gran & 0xF0;
	gdt_entries[idx].access = access;
}
