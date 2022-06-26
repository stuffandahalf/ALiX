#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#define __NEED_SSIZE_T 1
#include <stddef.h>

/* type used to identify a block of memory by alloc/free*/
struct memblk {
	size_t length;
	struct memblk *next;
};

/* type used to define the computer's memory map */
#define MEMORY_TYPE_FREE 1
#define MEMORY_TYPE_RESERVED 2
#define MEMORY_TYPE_RECLAIMABLE 3
#define MEMORY_TYPE_UNAVAILABLE 4
#define MEMORY_TYPE_KERNEL 5
struct mmap_entry {
	int type;
	void *start;
	size_t length;
	size_t blockc;
	struct memblk *block;
};

int kmem_init(struct mmap_entry *mmap, size_t entryc);
size_t kmem_avail(void);

void *kalloc(size_t count);
void kfree(void *ptr);

#endif /* ALIX_MEM_H */
