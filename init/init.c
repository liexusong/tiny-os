#include <types.h>
#include <console.h>
#include <printk.h>

int kernel_start()
{
	console_clear();
	printk("This is Liexsuong's OS, version: %d", 12345678);
	return 0;
}

