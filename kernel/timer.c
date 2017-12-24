#include <common.h>
#include <idt.h>
#include <types.h>
#include <printk.h>

void timer_callback(struct pt_regs *regs)
{
	static uint32_t tick = 0;
	cprintk(rc_red, "Tick: %d\n", tick++);
}

void init_timer(uint32_t hz)
{
	uint8_t low, high;
	uint32_t divisor = 1193180 / hz;

	register_interrupt_handler(IRQ0, timer_callback);

	// set 8253/8254 run at mode 3
	outb(0x43, 0x36);

	low = (uint8_t)(divisor & 0xFF);
	high = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, low);
	outb(0x40, high);
}
