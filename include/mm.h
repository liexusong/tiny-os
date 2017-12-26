#ifndef __MM_H
#define __MM_H

#include <types.h>
#include <idt.h>

#define STACK_SIZE     8192
#define MEM_MAX_SIZE   0x20000000
#define MEM_PAGE_SIZE  0x1000
#define PAGE_MAX_SIZE  (MEM_MAX_SIZE/MEM_PAGE_SIZE)
#define PHY_PAGE_MASK  0xFFFFF000

#define PAGE_OFFSET    0xC0000000

#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4

#define PAGE_SIZE      4096    // 4KB
#define PAGE_MASK      0xFFFFF000

#define PGD_INDEX(x)    (((x) >> 22) & 0x3FF)
#define PTE_INDEX(x)    (((x) >> 12) & 0x3FF)
#define OFFSET_INDEX(x) ((x) & 0xFFF)

typedef uint32_t pgd_t;
typedef uint32_t pte_t;

#define PGD_SIZE  (PAGE_SIZE / sizeof(pte_t))
#define PTE_SIZE  (PAGE_SIZE / sizeof(uint32_t))

#define PTE_COUNT 128  // map 512MB


// Get __kernel_mem_start and __kernel_mem_end address
extern uint8_t __kernel_mem_start[], __kernel_mem_end[];
extern uint32_t phy_page_count;
extern pgd_t pgd_k[PGD_SIZE];

void show_memory_map();
void init_mm();
uint32_t alloc_page();
void free_page(uint32_t page);
void page_fault(struct pt_regs *regs);

// virtual memory functions
void switch_pgd(uint32_t pgd);
void init_vmm();
void map(pgd_t *pgd, uint32_t va, uint32_t pa, uint32_t flags);
void unmap(pgd_t *pgd, uint32_t va);
int get_mapping(pgd_t *pgd, uint32_t va, uint32_t *pa);

#endif
