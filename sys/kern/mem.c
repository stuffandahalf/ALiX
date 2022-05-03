#include <alix/mem.h>

/* local constant since NULL is a legal address */
#define NONE ((void *)-1)

extern void write_serial(unsigned short port, const char *str);
extern void printul(unsigned short port, unsigned long int num, unsigned char base);
extern void printl(unsigned short port, unsigned long int num, unsigned char base);

static struct mmap_entry *sys_mmap = NULL;
static size_t sys_mmap_entries = 0;

static struct memblk *freemem = (void *)-1;

int
init_sysmem(struct mmap_entry *local_mmap, size_t entries)
{
	struct memblk *prev = NONE, *current;
	ssize_t i;
	// TODO: rework this
	for (i = entries - 1; i >= 0; i--) {
		if (local_mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		current = local_mmap[i].start;
		current->length = local_mmap[i].length - sizeof(struct memblk);
		current->prev = prev;
		current->next = NONE;
		if (prev == NONE) {
			freemem = current;
		} else {
			prev->next = current;
		}

		prev = current;

		/*if (!i) {
			current->next = NONE;
		}*/
	}

	write_serial(0x3f8, "block address");
	printul(0x3f8, freemem, 16);
	for (current = freemem; current != NONE; current = current->next) {
		write_serial(0x3f8, "loop");
		printul(0x3f8, current->length, 10);
	}

	sys_mmap = local_mmap;
	/* use temporary mmap to allocate permanent location */
	sys_mmap = kalloc(sizeof(struct mmap_entry) * entries);
	if (!sys_mmap) {
		return 1;
	}

	for (i = 0; i < entries; i++) {
		sys_mmap[i] = local_mmap[entries - i - 1];
	}
	sys_mmap_entries = entries;

	return 0;
}

void *
kalloc(size_t count)
{
	struct memblk *blk, *leftover;
	printul(0x3f8, sys_mmap, 10);
	printul(0x3f8, freemem, 16);
	if (!sys_mmap || freemem != NONE) {
		write_serial(0x3f8, "NO MMAP OR FREE MEM");
		return NULL;
	}

	for (blk = freemem; blk != NONE; blk = blk->next) {
		write_serial(0x3f8, "BLOCK");
		if (blk->length < count) {
			continue;
		}

		if (blk->length > (count + sizeof(struct memblk))) {
			leftover = (void *)blk + sizeof(struct memblk) + count;
			leftover->length = blk->length - count - sizeof(struct memblk);
			leftover->prev = blk->prev;
			leftover->next = blk->next;
		}
		return (void *)blk + sizeof(struct memblk);
	}

	return NULL;
}

void
kfree(void *ptr)
{
	struct memblk *blk = ptr - sizeof(struct memblk);

	//~ if (blk->start + blk->length

	// TODO: defragment memory blocks
	if (freemem) {
		blk->prev = NULL;
		blk->next = freemem;
		freemem->prev = blk;
	}
	freemem = blk;
}
