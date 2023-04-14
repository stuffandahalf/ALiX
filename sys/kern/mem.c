#include <stdint.h>
#include <config.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <alix/util.h>

struct mmap_entry *sys_physmmap = NULL;
ssize_t sys_physmmap_sz = 0;

// extern int kernel_bottom, kernel_top;

// const uintptr_t ALWAYS_RESERVE[][2] = {
// 	{ (uintptr_t)&kernel_bottom, (uintptr_t)&kernel_top }
// };
// #define ALWAYS_RESERVE_SZ LEN(ALWAYS_RESERVE)

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

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define overlap(s1, e1, s2, e2) (max((s1), (s2)) < min((e1), (e2)))

int
kmem_init(ssize_t mmap_sz, struct mmap_entry *mmap)//, ssize_t reserve_sz, uintptr_t reserve[][2])
{
	ssize_t i, j, k;
	size_t l;
	uintptr_t *range;
	struct memblk *blk;

	/* sort reserved ranges */
	for (i = 0; i < mmap_reserve_ranges_sz; i++) {
		for (j = i; j < mmap_reserve_ranges_sz; j++) {
			if (mmap_reserve_ranges[j][0] < mmap_reserve_ranges[i][0]) {
				for (k = 0; k < 2; k++) {
					range[k] = mmap_reserve_ranges[j][k];
					mmap_reserve_ranges[j][k] = mmap_reserve_ranges[i][k];
					mmap_reserve_ranges[i][k] = range[k];
				}
			}
		}
	}

#ifndef NDEBUG
	/* print list of reserved ranges */
	for (i = 0; i < mmap_reserve_ranges_sz; i++) {
		klogs("reserve ");
		kloglu(mmap_reserve_ranges[i][0], 16);
		klogc(':');
		kloglu(mmap_reserve_ranges[i][1], 16);
		klogc('\n');
	}
#endif

	/* initialize memory layout */
	for (i = 0; i < mmap_sz; i++) {
		uintptr_t start, end, next;

		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}


		start = (uintptr_t)mmap[i].start;
		do {
			end = (uintptr_t)mmap[i].start + mmap[i].length;
			next = 0;

			/* find any overlaps, adjust start and end sizes accordingly */
			for (j = mmap_reserve_ranges_sz - 1; j >= 0; j--) {
				if (overlap(start, end, mmap_reserve_ranges[j][0], mmap_reserve_ranges[j][1])) {
					klogs("OVERLAP\t");
					kloglu(start, 16);
					klogc(':');
					kloglu(end, 16);
					klogc('\t');
					kloglu(mmap_reserve_ranges[j][0], 16);
					klogc(':');
					kloglu(mmap_reserve_ranges[j][1], 16);
					klogc('\n');

					if (start >= mmap_reserve_ranges[j][0]) {
						start = mmap_reserve_ranges[j][1];
					} else {
						end = min(end, mmap_reserve_ranges[j][0]);
						next = !next ? mmap_reserve_ranges[j][1] : min(next, mmap_reserve_ranges[j][1]);
					}
				}
			}
			
			if (!mmap[i].free) {
				mmap[i].free = (void *)start;
			} else {
				blk->next = (void *)start;
			}
			blk = (void *)start;
			blk->magic = 0;
			blk->length = end - start;
			blk->next = NULL;

			start = next;
		} while (start != 0);
	}

	/* allocate and copy system mmap */
	sys_physmmap = alloc(mmap_sz, mmap, sizeof(struct mmap_entry) * mmap_sz);
	if (!sys_physmmap) {
		return 1;
	}
	memcpy(sys_physmmap, mmap, sizeof(struct mmap_entry) * mmap_sz);
	sys_physmmap_sz = mmap_sz;

	return 0;
}


int
mmap_reserve(ssize_t mmap_sz, struct mmap_entry *mmap, uintptr_t start, uintptr_t end)
{
	ssize_t i, r = 1;
	uintptr_t s, e;
	struct memblk *pblk, *blk, *nblk;

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

				/*if ((uintptr_t)(blk + 1) < start) {
					blk->length = start - (uintptr_t)blk;
				}*/

				/*if (  ) {
					nblk = (void *)min((uintptr_t)blk + blk->length, end);
					if ((uintptr_t)nblk == end) {
						nblk->length = blk->length - ((uintptr_t)nblk - (uintptr_t)blk);
						nblk->next = blk->next;
						blk = nblk;
					}
				}*/
				kmem_avail(1);
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
	// if (rsz % MEMBLK_ALIGNMENT) {
	// 	kloglu(size, 10);
	// 	klogc('\t');
	// 	kloglu(MEMBLK_ALIGNMENT - rsz % MEMBLK_ALIGNMENT, 10);
	// 	klogc('\n');
	// 	rsz += MEMBLK_ALIGNMENT - rsz % MEMBLK_ALIGNMENT;
	// }

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

// TODO: improve realloc efficiency
void *
realloc(ssize_t mmap_sz, struct mmap_entry *mmap, void *ptr, size_t size)
{
	void *new;
	struct memblk *blk = ptr - sizeof(struct memblk);

	new = alloc(mmap_sz, mmap, size);
	memcpy(new, ptr, blk->length);
	free(mmap_sz, mmap, ptr);

	return new;
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
