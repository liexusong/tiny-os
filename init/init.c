#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>

int kernel_start()
{
	init_gdt();
	init_idt();
	init_debug();

	console_clear();

	printk("This is Liexsuong's OS, version: %s\n", "v0.1");

	__asm__ volatile ("int $0x3");
	__asm__ volatile ("int $0x4");

	return 0;
}

