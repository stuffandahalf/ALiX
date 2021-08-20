#ifndef ALIX_SYS_MEM_H
#define ALIX_SYS_MEM_H	1

#include <stddef.h>

#define MEMORY_STATUS_FREE	0
#define MEMORY_STATUS_INUSE	1

struct mblk {
	void *ptr;
	size_t sz;
	int status;
	struct mblk *next;
	struct mblk *prev;
};

#endif /* ALIX_SYS_MEM_H */
