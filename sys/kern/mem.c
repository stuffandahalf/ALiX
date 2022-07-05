#include <stdint.h>
#include <alix/mem.h>

extern void write_serial(const char *str);
extern void put_serial(char c);
extern void printul(unsigned long int num, unsigned char base);
extern void printl(unsigned long int num, unsigned char base);

static struct mmap_entry *sys_mmap = NULL;
static size_t sys_mmap_entryc = 0;

extern uintptr_t kernel_bottom, kernel_top;

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

	/* Add entries to function-local memory map variable */
	for (i = 0; i < entryc; i++) {
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

size_t
kmem_avail(int debug)
{
	size_t i, j;
	size_t total = 0;
	ssize_t exp;
	void *start;
	struct memblk *blk, *buddy;

	if (debug) {
		write_serial("blk->exp\talloced\tstart\tend\n");
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
					printul(blk->exp, 10);
					write_serial("\t\t");
					write_serial(MEMBLK_MAGIC_ALLOCED == blk->magic ? "yes" : "no");
					put_serial('\t');
					printul((uintptr_t)blk, 16);
					put_serial('\t');
					printul((uintptr_t)blk + MEMBLK_SIZE(blk->exp), 16);
					put_serial('\n');
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
kalloc(size_t count)
{
	struct memblk *blk, *buddy;
	void *start;
	ssize_t exp;
	ssize_t i, j;

	count += sizeof(struct memblk);

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type || sys_mmap[i].length < count) {
			continue;
		}

		start = sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			blk = start;

			exp = sys_mmap[i].exp[j];
			do {

				exp = blk->exp;

				if (MEMBLK_MAGIC_ALLOCED != blk->magic && MEMBLK_SIZE(blk->exp) >= count) {
					blk->magic = MEMBLK_MAGIC_ALLOCED;
					while (blk->exp > 0 && MEMBLK_SIZE(blk->exp - 1) > count) {
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
	int found = 0;
	size_t i, j;
	ssize_t max_exp;
	uintptr_t start, end;
	struct memblk *blk, *buddy;

	if (!ptr) {
		return;
	}

	blk = (struct memblk *)ptr - 1;
	if (MEMBLK_MAGIC_ALLOCED != blk->magic) {
		return;
	}

	for (i = 0; i < sys_mmap_entryc && !found; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type) {
			continue;
		}

		start = (uintptr_t)sys_mmap[i].start;
		for (j = 0; !found && j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			end = start + MEMBLK_SIZE(sys_mmap[i].exp[j]);
			if (start <= (uintptr_t)blk && (uintptr_t)blk < end) {
				max_exp = sys_mmap[i].exp[j];
				found = 1;
			}
			start = end;
		}
	}

	if (!found) {
		return;
	}

	blk->magic = 0;
	buddy = BUDDY_ADDR(blk, blk->exp);
	while (blk->exp < max_exp && MEMBLK_MAGIC_ALLOCED != buddy->magic) {
		if (buddy < blk) {
			buddy->exp = blk->exp + 1;
			buddy->magic = 0;
			blk = buddy;
		} else {
			blk->exp++;
		}
		buddy = BUDDY_ADDR(blk, blk->exp);
	}
}
