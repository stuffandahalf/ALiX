#include <alix/mem.h>

struct mmap_entry *mmap = NULL;
size_t mmap_entries = 0;

struct memblk {
	void *start;
	size_t length;
};

struct memblk *free;

void *
kalloc(size_t count)
{
	unsigned long int i;

	if (!mmap) {
		return NULL;
	}
	//~ for (i = 0; i < mmap_entries; i++) {

	//~ }

	return NULL;
}

void
kfree(void *ptr)
{
	struct memblk *blk = ptr - sizeof(struct memblk);

	//~ if (blk->start + blk->length
}
