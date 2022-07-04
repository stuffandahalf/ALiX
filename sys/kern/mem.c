#include <stdint.h>
#include <alix/mem.h>

extern void write_serial(const char *str);
extern void printul(unsigned long int num, unsigned char base);
extern void printl(unsigned long int num, unsigned char base);

static struct mmap_entry *sys_mmap = NULL;
static size_t sys_mmap_entryc = 0;

extern uintptr_t kernel_bottom, kernel_top;

//~ #define DEBUG_MEM 1

#ifdef DEBUG_MEM
#define DEBUG_MEM_INIT 1
#define DEBUG_MEM_ALLOC 1
#define DEBUG_MEM_FREE 1
#define DEBUG_MEM_AVAIL 1
#endif

int
kmem_init(struct mmap_entry *entries, size_t entryc)
{
	int i, j, k;
	size_t entryc_real = 0, total = 0, remainder;
	struct mmap_entry tmp, local_mmap[entryc + 2];
	struct mmap_entry *tmp_mmap, *current = local_mmap;
	struct memblk *blk;
	void *start;
	void *kbot = &kernel_bottom, *ktop = &kernel_top;

#if DEBUG_MEM_INIT > 0
	write_serial("kernel_bottom:\t");
	printul((uintptr_t)kbot, 16);
	write_serial("\nkernel_top:\t");
	printul((uintptr_t)ktop, 16);
	write_serial("\nentryc:\t");
	printul(entryc, 10);
	put_serial('\n');
#endif

	/* Add entries to function-local memory map variable */
	for (i = 0; i < entryc; i++) {
		/* discard any zero-length blocks */
		if (!entries[i].length) {
			continue;
		}

		/* if mmap_entry contains kernel, need to split into 2-3 nodes */
		if (entries[i].start <= kbot && ktop <= entries[i].start + entries[i].length) {
#if DEBUG_MEM_INIT > 0
			write_serial("Kernel falls between:\t");
			printul((uintptr_t)entries[i].start, 16);
			write_serial(" - ");
			printul((uintptr_t)entries[i].start + entries[i].length, 16);
			put_serial('\n');
#endif

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

#if DEBUG_MEM_INIT > 0
	write_serial("entryc_real:\t");
	printul(entryc_real, 10);
	put_serial('\n');
#endif

	/* sort through memory map from smallest to largest */
	total = 0;
	//~ write_serial("MEMORY BLOCKS");
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
			total += local_mmap[i].length;
			//~ write_serial("NEW ENTRY");
			//~ printul(local_mmap[i].length, 10);

			remainder = local_mmap[i].length;
			start = local_mmap[i].start;
			for (k = 0; k < MMAP_MAX_BLOCKS; k++) {
				local_mmap[i].exp[k] = -1;
				while (remainder >> (local_mmap[i].exp[k] + 1) >= MEMBLK_MIN_SIZE) {
					local_mmap[i].exp[k]++;
				}
				if (local_mmap[i].exp[k] >= 0) {
					blk = start;
					blk->magic = 0;
					blk->exp = local_mmap[i].exp[k];

					//~ printul((uintptr_t)blk, 16);

					start += MEMBLK_MIN_SIZE << local_mmap[i].exp[k];
					remainder -= MEMBLK_MIN_SIZE << local_mmap[i].exp[k];
				}

			}
		}
	}
#if DEBUG_MEM_INIT > 0
	write_serial("real mem:\t");
	printul(total, 10);
	put_serial('\n');
#endif

	/* allocate kernel memory map */
	sys_mmap = local_mmap;
	sys_mmap_entryc = entryc_real;
#if DEBUG_MEM_INIT > 0
	write_serial("computed mem:\t");
	printul(kmem_avail(0), 10);
	put_serial('\n');
#endif
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

//~ #if DEBUG_MEM_AVAIL > 0
	if (debug) {
		write_serial("blk->exp\talloced\tstart\tend\n");
	}
//~ #endif

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (MEMORY_TYPE_FREE != sys_mmap[i].type) {
			continue;
		}

		start = sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			blk = start;

			do {
//~ #if DEBUG_MEM_AVAIL > 0
				if (debug) {
					printul(blk->exp, 10);
					write_serial("\t\t");
					write_serial(MEMBLK_MAGIC_ALLOCED == blk->magic ? "yes" : "no");
					put_serial('\t');
					printul(blk, 16);
					put_serial('\t');
					printul((void *)blk + MEMBLK_SIZE(blk->exp), 16);
					put_serial('\n');
				}
//~ #endif

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
#if DEBUG_MEM_ALLOC > 0
	write_serial("REQUESTED SIZE:\t");
	printul(count, 10);
	put_serial('\n');
#endif

	for (i = 0; i < sys_mmap_entryc; i++) {
#if DEBUG_MEM_ALLOC > 0
		write_serial("LOOP A\n");
#endif
		if (MEMORY_TYPE_FREE != sys_mmap[i].type || sys_mmap[i].length < count) {
			continue;
		}

		start = sys_mmap[i].start;
		for (j = 0; j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
#if DEBUG_MEM_ALLOC > 0
			write_serial("LOOP B\n");
#endif
			blk = start;

			exp = sys_mmap[i].exp[j];
			do {
#if DEBUG_MEM_ALLOC > 0
				write_serial("LOOP C\n");
				write_serial("exp\tblk->exp\tblk\tbuddy\n");
				printl(exp, 10);
				put_serial('\t');
				printul(blk->exp, 10);
				write_serial("\t\t");
				printul((uintptr_t)blk, 16);
				//~ printul((uintptr_t)BUDDY_ADDR(blk, blk->exp), 16);
				put_serial('\t');
				//~ put_serial('\n');
#endif

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
#if DEBUG_MEM_ALLOC > 0
					write_serial("\nRETURNED:\t");
					printul((uintptr_t)(blk + 1), 16);
					put_serial('\t');
					printl(blk->exp, 10);
					put_serial('\n');
#endif
					return (void *)(blk + 1);
				}

				buddy = BUDDY_ADDR(blk, exp);
				while (buddy < blk) {
					exp++;
					buddy = BUDDY_ADDR(buddy, exp);
				}
				blk = buddy;
#if DEBUG_MEM_ALLOC > 0
				printul((uintptr_t)blk, 16);
				put_serial('\n');
#endif
			} while (exp < sys_mmap[i].exp[j] && blk < (start + MEMBLK_SIZE(sys_mmap[i].exp[j])));

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

		start = sys_mmap[i].start;
		for (j = 0; !found && j < MMAP_MAX_BLOCKS && sys_mmap[i].exp[j] >= 0; j++) {
			end = start + MEMBLK_SIZE(sys_mmap[i].exp[j]);
			if (start <= blk && blk < end) {
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
