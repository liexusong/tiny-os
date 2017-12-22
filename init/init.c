#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>

int kernel_start()
{
	console_clear();
	init_debug();
	printk("This is Liexsuong's OS, version: %d\n", 12345678);
	panic("Panic occur!!!");
	return 0;
}

