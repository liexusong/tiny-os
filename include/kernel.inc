.set CODE_SEL, 0x08	# code segment selector in kernel mode 
.set DATA_SEL, 0x10 # data segment selector in kernel mode
.set IDT_ADDR, 0x40000	# IDT start address
.set IDT_SIZE, (256*8)	# IDT has fixed length
.set GDT_ADDR, (IDT_ADDR+IDT_SIZE)
.set GDT_ENTRIES, 5	# GDT has 5 descriptors
.set GDT_SIZE, (8*GDT_ENTRIES)
.set KERNEL_SECT, 100	# Kernel lenght, counted by sectors
.set STACK_BOT, 0xa0000	# stack starts at 640K

