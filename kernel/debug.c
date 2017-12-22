#include <string.h>
#include <elf.h>
#include <common.h>
#include <debug.h>
#include <printk.h>

static void print_stack_trace();
static elf_t kernel_elf;

elf_t elf_from_multiboot(multiboot_t *mb)
{
	int i;
	elf_t elf;
	// elf section array
	elf_section_header_t *sh = (elf_section_header_t *)mb->addr;
	uint32_t shstrtab = sh[mb->shndx].addr;

	for (i = 0; i < mb->num; i++) {
		const char *name = (const char *)(shstrtab + sh[i].name);

		if (!strcmp(name, ".strtab")) {
			elf.strtab = (const char *)sh[i].addr;
			elf.strtabsz = sh[i].size;
		}

		if (!strcmp(name, ".symtab")) {
			elf.symtab = (elf_symbol_t *)sh[i].addr;
			elf.symtabsz = sh[i].size;
		}
	}

	return elf;
}

const char *elf_lookup_symbol(uint32_t addr, elf_t *elf)
{
	int i;

	for (i = 0; i < (elf->symtabsz / sizeof(elf_symbol_t)); i++) {
		if (ELF32_ST_TYPE(elf->symtab[i].info) != 0x02) {
			continue;
		}

		if (addr >= elf->symtab[i].value 
			&& addr < (elf->symtab[i].value + elf->symtab[i].size))
		{
			return (const char *)((uint32_t)elf->strtab + elf->symtab[i].name);
		}
	}

	return NULL;
}

void init_debug()
{
	kernel_elf = elf_from_multiboot(global_mboot_ptr);
}

void print_cur_status()
{
	static int round = 0;
	uint16_t reg1, reg2, reg3, reg4;

	__asm__ volatile (
		"mov %%cs, %0;"
		"mov %%ds, %1;"
		"mov %%es, %2;"
		"mov %%ss, %3;"
		: "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4)
	);

	cprintk(rc_red, "%d: @ring %d\n", round, reg1 & 0x3);
	cprintk(rc_red, "%d: cs = 0x%x\n", round, reg1);
	cprintk(rc_red, "%d: ds = 0x%x\n", round, reg2);
	cprintk(rc_red, "%d: es = 0x%x\n", round, reg3);
	cprintk(rc_red, "%d: ss = 0x%x\n", round, reg4);

	round++;
}

void panic(const char *info)
{
	cprintk(rc_red, "******** System panic ********\n");
	cprintk(rc_red, "Debug info: %s\n", info);
	print_stack_trace();
	cprintk(rc_red, "******************************\n");
	print_cur_status();
	cprintk(rc_red, "******************************\n");

	for ( ; ; ) ;
}

static void print_stack_trace()
{
	uint32_t *ebp, *eip;

	__asm__ volatile ("mov %%ebp, %0" : "=r"(ebp));

	while (ebp) {
		eip = ebp + 1;
		cprintk(rc_red, "    [0x%x] %s\n", 
			    *eip, elf_lookup_symbol(*eip, &kernel_elf));
		ebp = (uint32_t *)*ebp;
	}
}
