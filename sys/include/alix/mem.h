#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#define __NEED_SSIZE_T 1
#include <stddef.h>
#include <cdefs.h>

#define MEMBLK_MAGIC_ALLOCED 0xA10C
#define MEMBLK_MIN_SIZE (1 << 5) /* 32 bytes */

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
	ssize_t maxexp;
};

/*int kmem_init(struct mmap_entry *mmap, size_t entryc);*/
size_t kmem_avail(int debug);

void *memcpy(void *restrict dest, const void *restrict src, size_t n);

void *alloc(struct mmap_entry *mmap, size_t mmap_sz, size_t size);
void *realloc(struct mmap_entry *mmap, size_t mmap_sz, void *ptr, size_t size);
void free(struct mmap_entry *mmap, size_t mmap_sz, void *ptr);

extern struct mmap_entry *sys_physmmap;
extern size_t sys_physmmap_sz;

#define kalloc(sz) alloc(sys_physmmap, sys_physmmap_sz, (sz))
#define krealloc(ptr, sz) realloc(sys_physmmap, sys_physmmap_sz, (ptr), (sz))
#define kfree(ptr) free(sys_physmmap, sys_physmmap_sz, (ptr))

#endif /* ALIX_MEM_H */
