#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <mm.h>

int kernel_start()
{
	uint32_t addr;

	init_gdt();
	init_idt();
	init_debug();
	init_mm();

	console_clear();

	printk("Welcome to SuperSong's OS, version: %s\n\n", "v0.1");

	init_timer(200);

	//__asm__ volatile ("sti");

	printk("kernel in memory start: 0x%x\n", __kernel_mem_start);
	printk("kernel in memory end: 0x%x\n", __kernel_mem_end);
	printk("kernel in memory_used: %d KBs\n", 
		(__kernel_mem_end - __kernel_mem_start + 1023) / 1024);

	show_memory_map();

	cprintk(rc_red, 
			"\nThe cout of physical memory pages is: %d\n\n", phy_page_count);

	addr = NULL;
	cprintk(rc_light_brown, "Test physical memory alloc:\n");
	addr = alloc_page();
	cprintk(rc_green, "Alloc physical address: 0x%x\n", addr);
	addr = alloc_page();
	cprintk(rc_green, "Alloc physical address: 0x%x\n", addr);
	addr = alloc_page();
	cprintk(rc_green, "Alloc physical address: 0x%x\n", addr);
	addr = alloc_page();
	cprintk(rc_green, "Alloc physical address: 0x%x\n", addr);
	addr = alloc_page();
	cprintk(rc_green, "Alloc physical address: 0x%x\n", addr);

	return 0;
}
