#include <types.h>
#include <console.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <mm.h>
#include <multiboot.h>
#include <heap.h>
#include <sched.h>
#include <common.h>

multiboot_t *global_mboot_ptr;
char kernel_stack[STACK_SIZE];
uint32_t kernel_stack_top;

__attribute__((section(".init.data")))
	pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data")))
	pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data")))
	pgd_t *pte_hig = (pgd_t *)0x3000;

int thread(void *arg)
{
	while (1) {
		cprintk(rc_green, "Thread2\n");
	}
}

void kernel_init()
{
	init_gdt();
	init_idt();
	init_debug();
	init_mm();
	init_vmm();
	init_heap();
	init_sched();

	console_clear();

	cprintk(rc_light_brown,
		"Welcome to SuperSong's OS, version: %s\n\n", "v0.1");

	init_timer(200);

	cprintk(rc_light_cyan,
		"kernel in memory start: 0x%x\n", __kernel_mem_start);
	cprintk(rc_light_cyan,
		"kernel in memory end: 0x%x\n", __kernel_mem_end);
	cprintk(rc_light_cyan,
		"kernel in memory_used: %d KBs\n",
		(__kernel_mem_end - __kernel_mem_start + 1023) / 1024);

	show_memory_map();

	cprintk(rc_red,
			"\nThe count of physical memory pages is: %d\n\n", phy_page_count);

	kthread_create(thread, NULL);

	enable_intr();

	while (1) {
		cprintk(rc_red, "Thraed1\n");
	}

	while (1) {
		__asm__ volatile ("hlt");
	}
}

#define set_cr3(pgd) do {                                    \
		__asm__ volatile ("mov %0, %%cr3" : : "r"((pgd)));   \
	} while (0)

#define page_on() do {                                       \
		int __cr0;                                           \
		__asm__ volatile ("mov %%cr0, %0" : "=r"(__cr0));    \
		__cr0 |= 0x80000000;                                 \
		__asm__ volatile ("mov %0, %%cr0" : : "r"(__cr0));   \
	} while (0)

#define set_esp(stack) do {                                  \
		__asm__ volatile ("mov %0, %%esp;"                   \
			"xor %%ebp, %%ebp" : : "r"((stack)));            \
	} while (0)

__attribute__((section(".init.text")))
int kernel_start()
{
	int i;

	// point to low address (0GB ~ ...)
	pgd_tmp[PGD_INDEX(0)] =
		(uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;

	// point to high address (3GB ~ 4GB)
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] =
		(uint32_t)pte_hig | PAGE_PRESENT | PAGE_WRITE;

	for (i = 0; i < 1024; i++) {
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	for (i = 0; i < 1024; i++) {
		pte_hig[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// set page directory
	set_cr3(pgd_tmp);
	page_on();

	kernel_stack_top = ((uint32_t)kernel_stack + STACK_SIZE) & 0xFFFFFFF0;

	set_esp(kernel_stack_top);

	global_mboot_ptr = 
		(multiboot_t *)((uint32_t)global_mboot_tmp + PAGE_OFFSET);

	kernel_init();

	return 0;
}
