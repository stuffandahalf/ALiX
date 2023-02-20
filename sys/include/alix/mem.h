#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#define __NEED_SSIZE_T 1
#include <stddef.h>
#include <cdefs.h>

#define MEMBLK_MAGIC_ALLOCED 0xA10C
// #define MEMBLK_MIN_SIZE (1 << 5) /* 32 bytes */
#define MEMBLK_ALIGNMENT 16

/* type used to identify a block of memory by alloc/free */
struct memblk {
	unsigned short int magic;
	// causes looping?
	// struct mmap_entry *parent;
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
	struct memblk *free;
	struct memblk *alloced;
};

int kmem_init(ssize_t mmap_sz, struct mmap_entry *mmap);
size_t kmem_avail(int debug);

void *memcpy(void *restrict dest, const void *restrict src, size_t n);

void *mmap_reserve(ssize_t mmap_sz, struct mmap_entry *mmap, uintptr_t start, size_t length);
void *alloc(ssize_t mmap_sz, struct mmap_entry *mmap, size_t size);
void *realloc(ssize_t mmap_sz, struct mmap_entry *mmap, void *ptr, size_t size);
void free(ssize_t mmap_sz, struct mmap_entry *mmap, void *ptr);

extern struct mmap_entry *sys_physmmap;
extern ssize_t sys_physmmap_sz;

#define kalloc(sz) alloc(sys_physmmap_sz, sys_physmmap, (sz))
#define krealloc(ptr, sz) realloc(sys_physmmap_sz, sys_physmmap, (ptr), (sz))
#define kfree(ptr) free(sys_physmmap_sz, sys_physmmap, (ptr))

#endif /* ALIX_MEM_H */
