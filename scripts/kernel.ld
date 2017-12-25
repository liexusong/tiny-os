/*
 * kernel.ld -- kernel link script
 */

ENTRY(start)
SECTIONS
{
	/* segment start point */
	. = 0x100000;
	PROVIDE( __kernel_mem_start = . );
	.text :
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data :
	{
		*(.data)
		*(.rodata)
		. = ALIGN(4069);
	}
	.bss :
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab :
	{
		*(.stab)
		. = ALIGN(4096);
	}
	.stabstr :
	{
		*(.stabstr)
		. = ALIGN(4096);
	}
	PROVIDE( __kernel_mem_end = . );
	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
