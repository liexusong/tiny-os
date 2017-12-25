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
	uint32_t mmap_addr = global_mboot_ptr->mmap_addr;
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
	mmap_entry_t *mmap_start_addr = global_mboot_ptr->mmap_addr;
	mmap_entry_t *mmap_end_addr = 
			global_mboot_ptr->mmap_addr + global_mboot_ptr->mmap_len;
	mmap_entry_t *me;

	for (me = mmap_start_addr; me < mmap_end_addr; me++) {

		if (me->type == 1 && me->base_addr_low == 0x100000) {

			uint32_t page_addr = me->base_addr_low 
						+ (uint32_t)(__kernel_mem_end - __kernel_mem_start);
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
	assert(mmap_stack_top != 0, "out of memory");

	uint32_t page = mmap_stack[mmap_stack_top--];

	return page;
}

void free_page(uint32_t page)
{
	assert(mmap_stack_top != PAGE_MAX_SIZE, "out of mm stack");

	mmap_stack[++mmap_stack_top] = page;
}
