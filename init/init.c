#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>

int kernel_start()
{
	init_gdt();
	init_idt();
	init_debug();

	console_clear();

	printk("Welcome to Liexsuong's OS, version: %s\n", "v0.1");

	init_timer(200);

	__asm__ volatile ("sti");

	return 0;
}
