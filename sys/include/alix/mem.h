#ifndef ALIX_MEM_H
#define ALIX_MEM_H 1

#include <stddef.h>

#define MEMORY_TYPE_FREE 0
#define MEMORY_TYPE_RESERVED 1
#define MEMORY_TYPE_RECLAIMABLE 2
#define MEMORY_TYPE_UNAVAILABLE 3
struct mmap_entry {
	int type;
	void *start;
	size_t length;
};

void *kalloc(size_t count);
void kfree(void *ptr);

extern struct mmap_entry *mmap;
extern size_t mmap_entries;

#endif /* ALIX_MEM_H */
