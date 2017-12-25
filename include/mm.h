#ifndef __MM_H
#define __MM_H

#define STACK_SIZE     8192
#define MEM_MAX_SIZE   0x20000000
#define MEM_PAGE_SIZE  0x1000
#define PAGE_MAX_SIZE  (MEM_MAX_SIZE/MEM_PAGE_SIZE)
#define PHY_PAGE_MASK  0xFFFFF000

// Get __kernel_mem_start and __kernel_mem_end address
extern uint8_t __kernel_mem_start[], __kernel_mem_end[];
extern uint32_t phy_page_count;

void show_memory_map();
void init_mm();
uint32_t alloc_page();
void free_page(uint32_t page);

#endif
