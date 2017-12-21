#include <types.h>
#include <console.h>

int kernel_start()
{
	console_clear();
	console_write_color("Hello, Liexusong's OS\n", rc_black, rc_red);
	return 0;
}

