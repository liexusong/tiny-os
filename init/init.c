#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>

int kernel_start()
{
	init_gdt();
	init_debug();

	console_clear();
	
	printk("This is Liexsuong's OS, version: %s\n", "v0.1");

	return 0;
}

