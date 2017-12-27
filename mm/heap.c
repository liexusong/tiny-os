#include <heap.h>
#include <mm.h>
#include <debug.h>

static void alloc_chunk(uint32_t start, uint32_t len);
static void free_chunk(heap_header_t *chunk);
static void split_chunk(heap_header_t *chunk, uint32_t len);
static void glue_chunk(heap_header_t *chunk);

static uint32_t heap_max = HEAP_START_ADDR;
static heap_header_t *heap_frist;

void init_heap()
{
	heap_frist = 0;
}

void *kmalloc(uint32_t len)
{
	heap_header_t *curr, *prev;
	uint32_t chunk_start;

	len += sizeof(heap_header_t); // include sizeof(heap_header_t)

	curr = heap_frist;
	prev = 0;

	while (curr) {
		if (curr->allocated == 0 && curr->length >= len) {
			split_chunk(curr, len);
			curr->allocated = 1;
			return (void *)((uint32_t)curr + sizeof(heap_header_t));
		}
		prev = curr;
		curr = curr->next;
	}

	if (prev) {
		chunk_start = (uint32_t)prev + prev->length;
	} else {
		chunk_start = HEAP_START_ADDR;
		heap_frist = (heap_header_t *)chunk_start;
	}

	alloc_chunk(chunk_start, len);

	curr = (heap_header_t *)chunk_start;
	curr->prev = prev;
	curr->next = 0;
	curr->allocated = 1;
	curr->length = len;

	if (prev) {
		prev->next = curr;
	}

	return (void *)(chunk_start + sizeof(heap_header_t));
}

void kfree(void *p)
{
	heap_header_t *header = 
		(heap_header_t *)((uint32_t)p - sizeof(heap_header_t));

	header->allocated = 0;

	glue_chunk(header);
}

static void alloc_chunk(uint32_t start, uint32_t len)
{
	uint32_t end = start + len;

	while (end > heap_max) {
		uint32_t page = alloc_page(); // alloc a new physical memory page
		map(pgd_k, heap_max, page, PAGE_PRESENT|PAGE_WRITE);
		heap_max += PAGE_SIZE;
	}
}

static void free_chunk(heap_header_t *chunk)
{
	if (chunk->prev == 0) {
		heap_frist = 0;
	} else {
		chunk->prev->next = 0;
	}

	while ((heap_max - PAGE_SIZE) >= (uint32_t)chunk) {
		uint32_t page;

		heap_max -= PAGE_SIZE;
		get_mapping(pgd_k, heap_max, &page);
		unmap(pgd_k, heap_max);
		free_page(page);
	}
}

// split a chunk to two chunks
static void split_chunk(heap_header_t *chunk, uint32_t len)
{
	if (chunk->length - len > sizeof(heap_header_t)) {
		heap_header_t *new_chunk = 
			(heap_header_t *)((uint32_t)chunk + len);

		new_chunk->prev = chunk;
		new_chunk->next = chunk->next;
		new_chunk->allocated = 0;
		new_chunk->length = chunk->length - len;

		chunk->next = new_chunk;
		chunk->length = len;
	}
}

static void glue_chunk(heap_header_t *chunk)
{
	if (chunk->next && chunk->next->allocated == 0) {
		chunk->length += chunk->next->length;
		if (chunk->next->next) {
			chunk->next->next->prev = chunk;
		}
		chunk->next = chunk->next->next;
	}

	if (chunk->prev && chunk->prev->allocated == 0) {
		chunk->prev->length += chunk->length;
		chunk->prev->next = chunk->next;
		if (chunk->next) {
			chunk->next->prev = chunk->prev;
		}
		chunk = chunk->prev;
	}

	if (chunk->next == 0) {
		free_chunk(chunk);
	}
}
