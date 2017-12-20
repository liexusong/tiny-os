/*
 * kernel.ld -- kernel link script
 */

ENTRY(start)
SECTIONS
{
	/* segment start point */
	. = 0x100000;
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
	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
