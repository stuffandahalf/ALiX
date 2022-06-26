#include <stdint.h>
#include <alix/mem.h>

/* local constant since NULL is a legal address */
#define NONE ((void *)-1)

extern void write_serial(const char *str);
extern void printul(unsigned long int num, unsigned char base);
extern void printl(unsigned long int num, unsigned char base);

static struct mmap_entry *sys_mmap = NULL;
static size_t sys_mmap_entryc = 0;

static struct memblk *freemem = NONE;

extern uintptr_t kernel_bottom, kernel_top;

int
kmem_init(struct mmap_entry *entries, size_t entryc)
{
	int i, j;
	size_t entryc_real = 0, total = 0;
	struct mmap_entry tmp, local_mmap[entryc + 2];
	struct mmap_entry *tmp_mmap, *current = local_mmap;
	void *kbot = &kernel_bottom, *ktop = &kernel_top;

	write_serial("kernel_bottom");
	printul(kbot, 16);
	write_serial("kernel_top");
	printul(ktop, 16);
	write_serial("entryc");
	printul(entryc, 10);

	/* Add entries to function-local memory map variable */
	for (i = 0; i < entryc; i++) {
		/* discard any zero-length blocks */
		if (!entries[i].length) {
			continue;
		}

		/* if mmap_entry contains kernel, need to split into 2-3 nodes */
		if (entries[i].start <= kbot && ktop <= entries[i].start + entries[i].length) {
			write_serial("Kernel falls between");
			printul(entries[i].start, 16);
			printul(entries[i].start + entries[i].length, 16);

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

	write_serial("entryc_real");
	printul(entryc_real, 10);

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
		if (local_mmap[i].type == MEMORY_TYPE_FREE) {
			local_mmap[i].block = local_mmap[i].start;
			local_mmap[i].block->length = local_mmap[i].length - sizeof(struct memblk);
			local_mmap[i].block->next = NULL;
			local_mmap[i].blockc = 1;
		} else {
			local_mmap[i].block = NULL;
			local_mmap[i].blockc = 0;
		}
	}

	/* allocate kernel memory map */
	sys_mmap = local_mmap;
	sys_mmap_entryc = entryc_real;
	tmp_mmap = kalloc(sizeof(struct mmap_entry) * entryc_real);
	sys_mmap = NULL;
	if (!tmp_mmap) {;
		//~ write_serial("FAILED TO ALLOCATE SYSTEM MEMORY MAP");
		sys_mmap_entryc = 0;
		return 1;
	}

	/* copy local map to kernel map */
	for (i = 0; i < entryc_real; i++) {
		tmp_mmap[i] = local_mmap[i];
	}
	sys_mmap = tmp_mmap;

	/*write_serial("Ordered mmap");
	total = 0;
	for (i = 0; i < entryc_real; i++) {
		//printul(sys_mmap[i].start, 16);
		printul(sys_mmap[i].length, 10);
		total += sys_mmap[i].length;
	}
	write_serial("total memory");
	printul(total, 10);*/

	/*total = 0;
	for (i = 0; i < sys_mmap_entryc; i++) {
		struct memblk *blk = sys_mmap[i].block;
		for (j = 0; j < sys_mmap[i].blockc; j++) {
			total += blk->length;
			blk = blk->next;
		}
	}
	write_serial("available memory");
	printul(total, 10);*/

	return 0;
}

size_t
kmem_avail(void)
{
	size_t total = 0;
	ssize_t i, j;
	struct memblk *blk;

	for (i = 0; i < sys_mmap_entryc; i++) {
		blk = sys_mmap[i].block;

		for (j = 0; j < sys_mmap[i].blockc; j++) {
			total += blk->length;

			blk = blk->next;
		}
	}

	return total;
}

#define blkstart(blk) ((void *)blk + sizeof(struct memblk))

void *
kalloc(size_t count)
{
	struct memblk *prev = NONE, *blk, *remainder, *next;
	ssize_t i, j;

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (sys_mmap[i].length < count) {
			continue;
		}

		blk = sys_mmap[i].block;
		for (j = 0; j < sys_mmap[i].blockc; j++) {
			if (blk->length >= count) {
				if (blk->length > count + sizeof(struct memblk)) {
					remainder = blkstart(blk) + count;

					remainder->length = blk->length - count - sizeof(struct memblk);
					remainder->next = blk->next;
					next = remainder;
				} else {
					next = blk->next;
					sys_mmap[i].blockc--;
				}

				if (NONE != prev) {
					prev->next = next;
				} else {
					sys_mmap[i].block = next;
				}

				return blkstart(blk);
			}

			prev = blk;
			blk = blk->next;
		}
	}

	return NULL;
}

void
kfree(void *ptr)
{
	ssize_t i, j;
	struct memblk *prev, *current, *block = (struct memblk *)ptr - 1;

	for (i = 0; i < sys_mmap_entryc; i++) {
		if (sys_mmap[i].start > block || block + block->length > sys_mmap[i].start + sys_mmap[i].length) {
			continue;
		}

		prev = sys_mmap[i].block;
		current = prev->next;
		for (j = 1; j < sys_mmap[i].blockc; j++) {
			if (prev < block && (!current || block < current)) {
				break;
			}

			prev = current;
			current = current->next;
		}

		/* if sequential, expand current block with next free block */
		if (current && blkstart(block) + block->length == current) {
			block->length += sizeof(struct memblk) + current->length;
			block->next = current->next;
		} else {
			block->next = current;
		}

		/* if sequential, expand previous block with size of current block */
		if (blkstart(prev) + prev->length == block) {
			prev->length += sizeof(struct memblk) + block->length;
			prev->next = block->next;
		} else {
			prev->next = block;
		}

	}
}
