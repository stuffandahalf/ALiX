#include <stdint.h>
#include <alix/mem.h>
#include <alix/log.h>

struct mmap_entry *sys_physmmap = NULL;
ssize_t sys_physmmap_sz = 0;

extern uintptr_t kernel_bottom, kernel_top;

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
kmem_init(ssize_t mmap_sz, struct mmap_entry *mmap)
{
	ssize_t i;
	struct memblk *blk;

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		blk = mmap[i].start;
		blk->length = mmap[i].length;
		blk->magic = 0;
		blk->next = NULL;
		mmap[i].free = blk;
	}

	// if (mmap_reserve(mmap_sz, mmap, kernel_bottom, kernel_top - kernel_bottom)) {
	// 	return 1;
	// }

	sys_physmmap = alloc(mmap_sz, mmap, sizeof(struct mmap_entry) * mmap_sz);
	kloglu((uintptr_t)sys_physmmap, 16);
	klogc('\n');
	if (!sys_physmmap) {
		return 1;
	}
	memcpy(sys_physmmap, mmap, sizeof(struct mmap_entry) * mmap_sz);
	sys_physmmap_sz = mmap_sz;

	return 0;
}

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) > (b)) ? (b) : (a))
#define inrange(s, e, v) ((s) <= (v) && (v) <= (e))

void *
mmap_reserve(ssize_t mmap_sz, struct mmap_entry *mmap, uintptr_t start, size_t length)
{
	ssize_t i;
	uintptr_t s, e;

	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE) {
			continue;
		}
		s = max(start, (uintptr_t)mmap[i].start);
		e = min(start + length, (uintptr_t)mmap[i].start + mmap[i].length);
		if (!inrange((uintptr_t)mmap[i].start, (uintptr_t)mmap[i].start + mmap[i].length, s) ||
			!inrange((uintptr_t)mmap[i].start, (uintptr_t)mmap[i].start + mmap[i].length, e)) {
			//return 1;
			continue;
		}


	}
	return NULL;
}

#define PADDED_LENGTH(l) (l + (((l) % MEMBLK_ALIGNMENT) ? MEMBLK_ALIGNMENT - (l) % MEMBLK_ALIGNMENT : 0))

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
	// rsz = PADDED_LENGTH(rsz);

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
				// klogld(blk->length, 10);
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
			(uintptr_t)(blk + blk->length) <= (uintptr_t)(mmap[i].start + mmap[i].length)) {
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

#if 0
int
kmem_init(struct mmap_entry *entries, size_t entryc)
{
	int i, j;
	size_t entryc_real = 0, remainder;
	struct mmap_entry tmp, local_mmap[entryc + 2];
	struct mmap_entry *tmp_mmap, *current = local_mmap;
	struct memblk *blk;
	void *start;
	void *kbot = &kernel_bottom, *ktop = &kernel_top;

	klogld(entryc, 10);
	/* Add entries to function-local memory map variable */
	for (i = 0; i < entryc; i++) {
		klogc('?');
		/* discard any zero-length blocks */
		if (!entries[i].length) {
			continue;
		}

		/* if mmap_entry contains kernel, need to split into 2-3 nodes */
		if (entries[i].start <= kbot && ktop <= entries[i].start + entries[i].length) {
			if (entries[i].start < kbot) {
				current->start = entries[i].start;
				current->length = kbot - entries[i].start;
				current->type = entries[i].type;
				current++;
				entryc_real++;
			}

			current->start = kbot;
			current->length = ktop - kbot;
			current->type = MEMORY_TYPE_KERNEL;
			current++;
			entryc_real++;

			if (ktop < entries[i].start + entries[i].length) {
				current->start = ktop;
				current->length = entries[i].start + entries[i].length - ktop;
				current->type = entries[i].type;
				current++;
				entryc_real++;
			}
		} else {
			*current = entries[i];
			current++;
			entryc_real++;
		}
	}

	/* sort through memory map from smallest to largest */
	for (i = 0; i < entryc_real; i++) {
		for (j = i; j < entryc_real; j++) {
			if (local_mmap[i].length > local_mmap[j].length) {
				tmp = local_mmap[i];
				local_mmap[i] = local_mmap[j];
				local_mmap[j] = tmp;
			}
		}
		/* i'th block is now in order */

		/* set up free memory blocks */
		local_mmap[i].exp[0] = -1;
		if (local_mmap[i].type == MEMORY_TYPE_FREE && local_mmap[i].length) {
			remainder = local_mmap[i].length;
			start = local_mmap[i].start;
			for (j = 0; j < MMAP_MAX_BLOCKS; j++) {
				local_mmap[i].exp[j] = -1;
				while (remainder >> (local_mmap[i].exp[j] + 1) >= MEMBLK_MIN_SIZE) {
					local_mmap[i].exp[j]++;
				}
				if (local_mmap[i].exp[j] >= 0) {
					blk = start;
					blk->magic = 0;
					blk->exp = local_mmap[i].exp[j];

					start += MEMBLK_MIN_SIZE << local_mmap[i].exp[j];
					remainder -= MEMBLK_MIN_SIZE << local_mmap[i].exp[j];
				}

			}
		}
	}

	/* allocate kernel memory map */
	sys_mmap = local_mmap;
	sys_mmap_entryc = entryc_real;
	tmp_mmap = kalloc(sizeof(struct mmap_entry) * entryc_real);
	sys_mmap = NULL;
	if (!tmp_mmap) {
		sys_mmap_entryc = 0;
		return 1;
	}

	/* copy local map to kernel map */
	for (i = 0; i < entryc_real; i++) {
		tmp_mmap[i] = local_mmap[i];
	}
	sys_mmap = tmp_mmap;

	return 0;
}

#define MEMBLK_SIZE(exp) (MEMBLK_MIN_SIZE << (exp))
#define BUDDY_MASK(exp) (MEMBLK_MIN_SIZE << (exp))
#define BUDDY_ADDR(blk, exp) ((void *)((uintptr_t)blk ^ BUDDY_MASK(exp)))

static struct memblk *
blk_combine(struct memblk *blk, struct memblk *buddy)
{
	if (buddy < blk) {
		buddy->exp = blk->exp + 1;
		buddy->magic = 0;
		blk = buddy;
	} else {
		blk->exp++;
	}
	return blk;
}

static ssize_t
blk_exp_max(struct memblk *blk)
{
	ssize_t i, j;
	uintptr_t start, end;

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type) {
			continue;
		}

		start = (uintptr_t)sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			end = start + MEMBLK_SIZE(sys_mmap[i].exp[j]);
			if (start <= (uintptr_t)blk && (uintptr_t)blk < end) {
				return sys_mmap[i].exp[j];
			}
			start = end;
		}
	}

	return -1;
}

size_t
kmem_avail(int debug)
{
	size_t i, j;
	size_t total = 0;
	ssize_t exp;
	void *start;
	struct memblk *blk, *buddy;

	if (debug) {
		klogs("blk->exp\talloced\tstart\tend\n");
	}

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type) {
			continue;
		}

		start = sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			blk = start;

			do {
				if (debug) {
					kloglu(blk->exp, 10);
					klogs("\t\t");
					klogs(MEMBLK_MAGIC_ALLOCED == blk->magic ? "yes" : "no");
					klogc('\t');
					kloglu((uintptr_t)blk, 16);
					klogc('\t');
					kloglu((uintptr_t)blk + MEMBLK_SIZE(blk->exp), 16);
					klogc('\n');
				}

				exp = blk->exp;
				if (MEMBLK_MAGIC_ALLOCED != blk->magic) {
					total += MEMBLK_SIZE(blk->exp);
				}

				buddy = BUDDY_ADDR(blk, exp);
				while ((uintptr_t)buddy < (uintptr_t)blk) {
					exp++;
					buddy = BUDDY_ADDR(buddy, exp);
				}
				blk = buddy;
			} while (exp < sys_mmap[i].exp[j]);

			start += MEMBLK_MIN_SIZE << sys_mmap[i].exp[j];
		}
	}

	return total;
}

void *
alloc(struct mmap_entry *mmap, size_t mmap_sz, size_t size)
{
	return NULL;
}

void *
realloc(struct mmap_entry *mmap, size_t mmap_sz, void *ptr, size_t size)
{
	return NULL;
}

void
free(struct mmap_entry *mmap, size_t mmap_sz, void *ptr)
{

}



#if 0
void *
kalloc(size_t size)
{
	struct memblk *blk, *buddy;
	void *start;
	ssize_t exp;
	ssize_t i, j;

	size += sizeof(struct memblk);

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type || sys_mmap[i].length < size) {
			continue;
		}

		start = sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			blk = start;

			exp = sys_mmap[i].exp[j];
			do {

				exp = blk->exp;

				if (MEMBLK_MAGIC_ALLOCED != blk->magic && MEMBLK_SIZE(blk->exp) >= size) {
					blk->magic = MEMBLK_MAGIC_ALLOCED;
					while (blk->exp > 0 && MEMBLK_SIZE(blk->exp - 1) > size) {
						blk->exp--;
						buddy = BUDDY_ADDR(blk, blk->exp);
						if (MEMBLK_MAGIC_ALLOCED != buddy->magic) {
							buddy->magic = 0;
							buddy->exp = blk->exp;
						}
					}
					return (void *)(blk + 1);
				}

				buddy = BUDDY_ADDR(blk, exp);
				while (buddy < blk) {
					exp++;
					buddy = BUDDY_ADDR(buddy, exp);
				}
				blk = buddy;
			} while (exp < sys_mmap[i].exp[j] && (void *)blk < (start + MEMBLK_SIZE(sys_mmap[i].exp[j])));

			start += MEMBLK_MIN_SIZE << sys_mmap[i].exp[j];
		}
	}

	return NULL;
}

void
kfree(void *ptr)
{
	ssize_t max_exp;
	struct memblk *blk, *buddy;

	if (!ptr) {
		return;
	}

	blk = (struct memblk *)ptr - 1;
	if (MEMBLK_MAGIC_ALLOCED != blk->magic) {
		return;
	}

	max_exp = blk_exp_max(blk);

	if (max_exp < 0) {
		return;
	}

	blk->magic = 0;
	buddy = BUDDY_ADDR(blk, blk->exp);
	while (blk->exp < max_exp && MEMBLK_MAGIC_ALLOCED != buddy->magic) {
		blk = blk_combine(blk, buddy);
		buddy = BUDDY_ADDR(blk, blk->exp);
	}
}

void *
krealloc(void *ptr, size_t size)
{
	struct memblk *blk, *blk1, *buddy;
	ssize_t exp_max, exp_new, exp;
	void *new_ptr;

	if (!ptr) {
		return kalloc(size);
	}

	size += sizeof(struct memblk);

	blk = (struct memblk *)ptr - 1;
	if (MEMBLK_MAGIC_ALLOCED != blk->magic) {
		return NULL;
	}

	exp_max = blk_exp_max(blk);
	if (exp_max < 0) {
		return NULL;
	}

	exp_new = blk->exp;
	blk1 = blk;
	buddy = BUDDY_ADDR(blk, exp_new);
	while (exp_new < exp_max && MEMBLK_SIZE(exp_new) < size &&
			MEMBLK_MAGIC_ALLOCED != buddy->magic) {
		exp_new++;
		if (blk1 > buddy) {
			blk1 = buddy;
		}
		buddy = BUDDY_ADDR(blk1, exp_new);
	}

	if (MEMBLK_SIZE(exp_new) < size || MEMBLK_MAGIC_ALLOCED == buddy->magic) {
		new_ptr = kalloc(size - sizeof(struct memblk));
		if (!new_ptr) {
			return NULL;
		}

		memcpy(new_ptr, ptr, MEMBLK_SIZE(blk->exp) - sizeof(struct memblk));
		kfree(ptr);
		return new_ptr;
	}

	blk1 = blk;
	exp = blk->exp;
	while (blk->exp < exp_new) {
		buddy = BUDDY_ADDR(blk, blk->exp);
		blk = blk_combine(blk, buddy);
	}
	return memcpy(blk + 1, blk1 + 1, MEMBLK_SIZE(exp) - sizeof(struct memblk));
}
#endif

#endif
