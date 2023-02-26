#include <stdint.h>
#include <alix/mem.h>
#include <alix/log.h>

struct mmap_entry *sys_physmmap = NULL;
ssize_t sys_physmmap_sz = 0;

extern int kernel_bottom, kernel_top;

void *
memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	size_t i;
	void *od = dest;

	if (dest == NULL || src == NULL) {
		return NULL;
	}

	for (i = 0; i < n; i++) {
		*(uint8_t *)dest++ = *(uint8_t *)src++;
	}

	return od;
}

int
kmem_init(ssize_t mmap_sz, struct mmap_entry *mmap, ssize_t reserve_sz, uintptr_t reserve[][2])
{
	ssize_t i;
	struct memblk *blk;

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		/* TODO: alignment */
		blk = mmap[i].start;
		blk->length = mmap[i].length;
		blk->magic = 0;
		blk->next = NULL;
		mmap[i].free = blk;
	}

	if (mmap_reserve(mmap_sz, mmap, (uintptr_t)&kernel_bottom, (uintptr_t)&kernel_top)) {
		return 1;
	}
	klogs("REMOVE THIS " __FILE__ ":");
	kloglu(__LINE__, 10);
	klogc('\n');
	return 1;
	for (i = 0; i < reserve_sz; i++) {
		if (mmap_reserve(mmap_sz, mmap, reserve[i][0], reserve[i][1])) {
			return 1;
		}
	}

	sys_physmmap = alloc(mmap_sz, mmap, sizeof(struct mmap_entry) * mmap_sz);
	if (!sys_physmmap) {
		return 1;
	}
	memcpy(sys_physmmap, mmap, sizeof(struct mmap_entry) * mmap_sz);
	sys_physmmap_sz = mmap_sz;

	return 0;
}

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define overlap(s1, e1, s2, e2) (max((s1), (s2)) < min((e1), (e2)))

int
mmap_reserve(ssize_t mmap_sz, struct mmap_entry *mmap, uintptr_t start, uintptr_t end)
{
	ssize_t i, r = 1;
	uintptr_t s, e;
	struct memblk *pblk, *blk;

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}

		for (pblk = NULL, blk = mmap[i].free; blk != NULL; pblk = blk, blk = blk->next) {
			if (overlap(start, end, (uintptr_t)blk, (uintptr_t)blk + blk->length)) {
				klogs("OVERLAP: ");
				kloglu((uintptr_t)start, 16);
				klogc(':');
				kloglu((uintptr_t)end, 16);
				klogc(' ');
				kloglu((uintptr_t)blk, 16);
				klogc(':');
				kloglu((uintptr_t)blk + blk->length, 16);
				klogc('\n');
			}
		}


	}
	return r;
}

void *
alloc(ssize_t mmap_sz, struct mmap_entry *mmap, size_t size)
{
	size_t rsz = size + sizeof(struct memblk);
	ssize_t i;
	struct memblk *pblk, *blk, *nblk;

	/* correct allignment */
	if (rsz % MEMBLK_ALIGNMENT) {
		kloglu(size, 10);
		klogc('\t');
		kloglu(MEMBLK_ALIGNMENT - rsz % MEMBLK_ALIGNMENT, 10);
		klogc('\n');
		rsz += MEMBLK_ALIGNMENT - rsz % MEMBLK_ALIGNMENT;
	}

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		for (pblk = NULL, blk = mmap[i].free; blk != NULL; pblk = blk, blk = blk->next) {
			if (blk->length >= rsz) {
				/* break down block */
				if (blk->length > (rsz + sizeof(struct memblk))) {
					nblk = (void *)blk + rsz;
					nblk->next = blk->next;
					nblk->length = blk->length - rsz;
					nblk->magic = 0;
					blk->length = rsz;
					blk->next = nblk;
				}

				if (pblk) {
					pblk->next = blk->next;
				}
				if (mmap[i].free == blk) {
					mmap[i].free = blk->next;
				}
				blk->next = mmap[i].alloced;
				mmap[i].alloced = blk;
				/* needed? */
				blk->magic = MEMBLK_MAGIC_ALLOCED;

				// klogs("alloced ");
				// klogld((void *)(blk + 1), 16);
				// klogc('\n');

				return (void *)(blk + 1);
			}
		}
	}

	return NULL;
}

void *
realloc(ssize_t mmap_sz, struct mmap_entry *mmap, void *ptr, size_t size)
{
	return NULL;
}

void
free(ssize_t mmap_sz, struct mmap_entry *mmap, void *ptr)
{
	ssize_t i;
	struct memblk *blk = ptr - sizeof(struct memblk);
	struct memblk *pblk, *cblk;

	if (!ptr) {
		return;
	}

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		if ((uintptr_t)mmap[i].start <= (uintptr_t)blk &&
			((uintptr_t)blk + blk->length) <= (uintptr_t)(mmap[i].start + mmap[i].length)) {
			// free block
			// klogs("found mmap entry\n");
			// for (pblk = NULL; fblk = mmap[i].free

			for (pblk = NULL, cblk = mmap[i].alloced; cblk != blk && cblk != NULL; pblk = cblk, cblk = cblk->next);
			if (!cblk) {
				klogs("not really alloced?\n");
				return;
			}

			if (!pblk) {
				mmap[i].alloced = cblk->next;
			} else {
				pblk->next = cblk->next;
			}

			// if ((uintptr_t)mmap[i].free > (uintptr_t)blk) {
			// 	blk->next = mmap[i].free;
			// 	mmap[i].free = blk;
			// }
			for (pblk = NULL, cblk = mmap[i].free; cblk != NULL; pblk = cblk, cblk = cblk->next) {
				// if (pblk && (uintptr_t)pblk  + pblk->length == blk) {

				// }
				if ((uintptr_t)blk + blk->length == (uintptr_t)cblk) {
					blk->length += cblk->length;
					blk->next = cblk->next;
					cblk = blk;

					if (pblk) {
						// blk->next = pblk->next;
						pblk->next = cblk;
					} else {
						mmap[i].free = cblk;
					}
				}
				// if ((uintptr_t)cblk + cblk->length == (uintptr_t)blk) {
				// 	blk->next = cblk->next;
				// 	cblk->next = blk;
				// 	cblk
				// }
				// if ((uintptr_t)(cblk))
			}
		}
	}
}

size_t
kmem_avail(int debug)
{
	size_t count = 0;
	ssize_t i;
	struct memblk *blk;

	for (i = 0; i < sys_physmmap_sz; i++) {
		if (debug) {
			klogld(i, 10);
			klogs(" | type ");
			kloglu(sys_physmmap[i].type, 10);
			klogs(" | free ");
			kloglu((uintptr_t)sys_physmmap[i].free, 16);
			klogs(" | alloced ");
			kloglu((uintptr_t)sys_physmmap[i].alloced, 16);
			klogc('\n');
		}
		if (sys_physmmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		for (blk = sys_physmmap[i].free; blk != NULL; blk = blk->next) {
			count += blk->length - sizeof(struct memblk);
		}
	}
	return count;
}
