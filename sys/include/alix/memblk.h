#ifndef ALIX_MEMBLK_H
#define ALIX_MEMBLK_H 1

#include <stddef.h>

#define MEMBLK_STATUS_FREE 0
#define MEMBLK_STATUS_ALLOCATED 1
#define MEMBLK_STATUS_RECLAIMABLE 2
#define MEMBLK_STATUS_UNAVAILABLE 3
struct memblk {
	int status;
	void *start;
	size_t length;
	struct memblk *next;
};

#endif