#include <multiboot.h>
#include <common.h>
#include <debug.h>
#include <printk.h>
#include <types.h>
#include <mm.h>

static uint32_t mmap_stack[PAGE_MAX_SIZE+1];
static uint32_t mmap_stack_top;

uint32_t phy_page_count;

void show_memory_map()
{
	uint32_t mmap_addr = global_mboot_ptr->mmap_addr + PAGE_OFFSET;
	uint32_t mmap_len = global_mboot_ptr->mmap_len;
	mmap_entry_t *mmap;

	printk("Memory map:\n");

	for (mmap = (mmap_entry_t *)mmap_addr; 
		(uint32_t)mmap < mmap_addr + mmap_len; 
		mmap++)
	{
		printk("base address = 0x%x, length = 0x%x, type = 0x%x\n",
			   (uint32_t)mmap->base_addr_low, 
			   (uint32_t)mmap->length_low,
			   (uint32_t)mmap->type);
	}
}

void init_mm()
{
	mmap_entry_t *mmap_start_addr = 
			(mmap_entry_t *)global_mboot_ptr->mmap_addr;
	mmap_entry_t *mmap_end_addr = 
			(mmap_entry_t *)global_mboot_ptr->mmap_addr 
			+ global_mboot_ptr->mmap_len;
	mmap_entry_t *me;

	uint32_t kernel_size = 
			((uint32_t)(__kernel_mem_end - __kernel_mem_start)
			+ MEM_PAGE_SIZE - 1) & PHY_PAGE_MASK;

	for (me = mmap_start_addr; me < mmap_end_addr; me++) {

		if (me->type == 1 && me->base_addr_low == 0x100000) {

			uint32_t page_addr = me->base_addr_low + kernel_size;
			uint32_t page_end = me->base_addr_low + me->length_low;

			while (page_addr < page_end && page_addr <= MEM_MAX_SIZE) {
				free_page(page_addr);
				page_addr += MEM_PAGE_SIZE;
				phy_page_count++;
			}
		}
	}
}

uint32_t alloc_page()
{
	if (mmap_stack_top == 0) {
		return 0;
	}

	uint32_t page = mmap_stack[mmap_stack_top--];

	return page;
}

void free_page(uint32_t page)
{
	if (mmap_stack_top == PAGE_MAX_SIZE) {
		return;
	}

	mmap_stack[++mmap_stack_top] = page;
}

void page_fault(struct pt_regs *regs)
{
	uint32_t cr2;

	__asm__ volatile ("mov %%cr2, %0" : "=r"(cr2));

	printk(
		"Page fault at 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
	printk("Error code: %x\n", regs->err_code);

	if (!(regs->err_code & 0x1)) {
		cprintk(rc_red, "Because the page wasn't present.\n");
	}

	if (regs->err_code & 0x2) {
		cprintk(rc_red, "Write error.\n");
	} else {
		cprintk(rc_red, "Read error.\n");
	}

	if (regs->err_code & 0x4) {
		cprintk(rc_red, "Fault in user mode.\n");
	} else {
		cprintk(rc_red, "Fault in kernel mode.\n");
	}

	if (regs->err_code & 0x8) {
		cprintk(rc_red, "Reserved bits being overwritten.\n");
	}

	if (regs->err_code & 0x10) {
		cprintk(rc_red, "The fault occurred during an instruction fetch.\n");
	}

	while (0);
}
