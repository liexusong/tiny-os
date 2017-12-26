#include <idt.h>
#include <string.h>
#include <debug.h>
#include <mm.h>
#include <types.h>

pgd_t pgd_k[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));
pte_t pte_k[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

void switch_pgd(uint32_t pgd)
{
	__asm__ volatile ("mov %0, %%cr3" : : "r"(pgd));
}

void init_vmm()
{
	uint32_t k_pte_first_idx = PGD_INDEX(PAGE_OFFSET);
	uint32_t *pte;
	uint32_t pgd_k_addr;
	uint32_t i, j;

	for (i = k_pte_first_idx, j = 0; 
		 i < PTE_COUNT + k_pte_first_idx; 
		 i++, j++)
	{
		pgd_k[i] = 
				((uint32_t)pte_k[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
	}

	for (pte = (uint32_t *)pte_k, i = 1; 
		 i < PTE_COUNT * PTE_SIZE; i++) 
	{
		pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	pgd_k_addr = (uint32_t)pgd_k - PAGE_OFFSET;

	register_interrupt_handler(14, &page_fault);

	switch_pgd(pgd_k_addr);
}

void map(pgd_t *pgd, uint32_t va, uint32_t pa, uint32_t flags)
{
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);
	pte_t *pte;

	pte = (pte_t *)(pgd[pgd_idx] & PAGE_MASK);
	if (!pte) {
		pte = (pte_t *)alloc_page();
		if (!pte) {
			panic("out of memory");
		}

		pgd[pgd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;

		pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

		bzero(pte, PAGE_SIZE);

	} else {
		pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
	}

	pte[pte_idx] = (pa & PAGE_MASK) | flags;

	__asm__ volatile ("invlpg (%0)" : : "a"(va));
}

void unmap(pgd_t *pgd, uint32_t va)
{
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);
	pte_t *pte;

	pte = (pte_t *)(pgd[pgd_idx] & PAGE_MASK);
	if (!pte) {
		return;
	}

	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET); // virtual address

	pte[pte_idx] = 0;

	__asm__ volatile ("invlpg (%0)" : : "a"(va));
}

int get_mapping(pgd_t *pgd, uint32_t va, uint32_t *pa)
{
	uint32_t pgd_idx = PGD_INDEX(va);
	uint32_t pte_idx = PTE_INDEX(va);
	pte_t *pte;

	pte = (pte_t *)(pgd[pgd_idx] & PAGE_MASK);
	if (!pte) {
		return -1;
	}

	pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

	if (pte[pte_idx] != 0 && pa) {
		*pa = pte[pte_idx] & PAGE_MASK;
		return 0;
	}

	return -1;
}
