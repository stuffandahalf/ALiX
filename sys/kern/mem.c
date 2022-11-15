#include <stdint.h>
#include <alix/mem.h>
#include <alix/log.h>

struct mmap_entry *sys_physmmap = NULL;
size_t sys_physmmap_sz = 0;

extern uintptr_t kernel_bottom, kernel_top;

void *
memcpy(void *dest, const void *src, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		*(uint8_t *)dest = *(uint8_t *)src;
	}

	return dest;
}

void *
alloc(struct mmap_entry *mmap, size_t mmap_sz, size_t size)
{
	size_t i, rsz = size + sizeof(struct memblk);
	struct memblk *blk;
	ssize_t e, exp = 0;

	if (rsz % MEMBLK_MIN_SIZE) {
		rsz += MEMBLK_MIN_SIZE - rsz % MEMBLK_MIN_SIZE;
	}

	while ((rsz >> exp) > 0) {
		exp++;
	}


	for (i = 0; i < mmap_sz; i++) {
		if (mmap[i].type != MEMORY_TYPE_FREE || rsz > mmap[i].length) continue;

		/* traverse buddy tree until suitable node is found */

		blk = mmap[i].start;
		if (blk->magic == MEMBLK_MAGIC_ALLOCED) {
			/* if master, find buddy */
			/* if buddy, increase e, find master + 1 buddy */
			// if ()
		}
	}

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
