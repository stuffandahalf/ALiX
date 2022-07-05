#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#define __NEED_SSIZE_T 1
#include <stddef.h>

#define MEMBLK_MAGIC_ALLOCED 0xA10C
#define MEMBLK_MIN_SIZE (1 << 5) /* 32 bytes */
#define MMAP_MAX_BLOCKS 15

/* type used to identify a block of memory by alloc/free */
struct memblk {
	unsigned short int magic;
	ssize_t exp;
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
	ssize_t exp[MMAP_MAX_BLOCKS];
};

int kmem_init(struct mmap_entry *mmap, size_t entryc);
size_t kmem_avail(int debug);

void *memcpy(void *dest, const void *src, size_t n);

void *kalloc(size_t size);
void *krealloc(void *ptr, size_t size);
void kfree(void *ptr);

#endif /* ALIX_MEM_H */
