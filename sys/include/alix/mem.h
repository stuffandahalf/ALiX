#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#define __NEED_SSIZE_T 1
#include <stddef.h>

/* type used to define the computer's memory map */
#define MEMORY_TYPE_FREE 1
#define MEMORY_TYPE_RESERVED 2
#define MEMORY_TYPE_RECLAIMABLE 3
#define MEMORY_TYPE_UNAVAILABLE 4
struct mmap_entry {
	int type;
	void *start;
	size_t length;
};

/* type used to identify a block of memory by alloc/free*/
struct memblk {
	size_t length;
	struct memblk *next;
	struct memblk *prev;
};

int init_sysmem(struct mmap_entry *, size_t);

void *kalloc(size_t count);
void kfree(void *ptr);

#endif /* ALIX_MEM_H */
