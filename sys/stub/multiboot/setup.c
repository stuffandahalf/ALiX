#include <stddef.h>
#include <stdint.h>
#include <multiboot.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <arch/log.h>

static int init_mmap(struct multiboot_info *mbd);

//~ extern struct device early_console;

void
setup32(multiboot_info_t *mbd)
{
	int i;
	//~ void *a, *b;

	//~ if (init_serial()) {
		//~ return;
	//~ }
	klogs("Hello Serial World!\n");

	klogs((const char *)mbd->boot_loader_name);
	klogc('\n');
	klogld(mbd->mmap_length, 10);
	klogc('\n');
	if (init_mmap(mbd)) {
		klogs("Failed to initialize memory\n");
		return;
	}
	klogs("SURVIVED\n");

	klogs("MEMORY LAYOUT\n");
	kloglu(kmem_avail(1), 10);
	klogc('\n');

	//~ kmem_avail(1);
	//~ b = kalloc(1024);
	//~ kmem_avail(1);

	//~ a = kalloc(1024);
	//~ kmem_avail(1);
	//~ printul((uintptr_t)a, 16);
	//~ put_serial('\n');

	//~ a = krealloc(a, 1024 * 1024);
	//~ kmem_avail(1);
	//~ printul((uintptr_t)a, 16);
	//~ put_serial('\n');

	//~ for (;;) {
		//~ char c = get_serial();
		//~ put_serial(c);
	//~ }
}

static int init_mmap_simple(size_t lo, size_t hi);

/* TODO: Implement new memory map init */
/* - find number of mmap entries */
/* - use first block of sufficient size to allocate new system mmap */
/* - copy details over to new mmap */
static int
init_mmap(multiboot_info_t *mbd)
{
	struct multiboot_mmap_entry *mb_mmap;

	if (0 && mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
#if 0
		mmap_sz = 0;
		for (mb_mmap = (void *)mbd->mmap_addr;
			(uintptr_t)mb_mmap < (mbd->mmap_addr + mbd->mmap_length);
			mb_mmap += mb_mmap->size) {
			mmap_sz++;
		}
		kloglu(mmap_sz, 10);

		mb_mmap = (void *)mbd->mmap_addr;
        while ((uintptr_t)mb_mmap < (mbd->mmap_addr + mbd->mmap_length)) {
			blk.start = (void *)(uintptr_t)mb_mmap->addr;
			blk.length = mb_mmap->len;
			blk.maxexp = 0;
			// while ((blk.length >> blk.maxexp) > MEMBLK_MIN_SIZE) blk.maxexp++;

			kloglu((uintptr_t)blk.start, 16);
			klogc('\t');
			kloglu(blk.length, 10);
			klogc('\t');
			kloglu(log2(blk.length), 10);
			// kloglu(blk.maxexp, 10);
			klogc('\t');
			kloglu(mb_mmap->type, 10);
			klogc('\n');

			switch (mb_mmap->type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				blk.type = MEMORY_TYPE_FREE;
				break;
			case MULTIBOOT_MEMORY_RESERVED:
			case MULTIBOOT_MEMORY_BADRAM:
				blk.type = MEMORY_TYPE_UNAVAILABLE;
				break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				blk.type = MEMORY_TYPE_RECLAIMABLE;
				break;
			case MULTIBOOT_MEMORY_NVS:
				klogs("TODO: Figure out NVS memory type\n");
				break;
			default:
				blk.type = 0;
			}
			if (blk.type) {
				/* attempt to allocate a memory map */
				mmap = alloc(&blk, 1, sizeof(struct mmap_entry) * mbd->mmap_length);
				if (mmap) {
					break;
				}
			}
			mb_mmap = ((void *)mb_mmap) + mb_mmap->size + offsetof(struct multiboot_mmap_entry, addr);
			// mb_mmap = (void *)((uintptr_t)mb_mmap + mb_mmap->size + sizeof(mb_mmap->size));
		}
#endif
	} else if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
		if (init_mmap_simple(mbd->mem_lower, mbd->mem_upper)) {
			return 1;
		}

		// blk.start = (void *)0;
		// blk.length = mbd->mem_lower;
		// blk.type = MEMORY_TYPE_FREE;
		// mmap_sz = 2;
		// mmap = alloc(&blk, 1, sizeof(struct mmap_entry) * mmap_sz);

		// if (!mmap) {
		// 	blk.start = (void *)0x100000;
		// 	blk.length = mbd->mem_upper;
		// 	blk.type = MEMORY_TYPE_FREE;
		// 	mmap = alloc(&blk, 1, sizeof(struct mmap_entry) * mmap_sz);
		// }
	}

	return 0;
}

static int
init_mmap_simple(size_t lo, size_t hi)
{
	struct mmap_entry mmap[] = {
		{ .type = MEMORY_TYPE_FREE, .start = (void *)0x500, .length = lo * 1024 - 0x500, .free = NULL, .alloced = NULL },
		{ .type = MEMORY_TYPE_FREE, .start = (void *)0x100000, .length = hi * 1024, .free = NULL, .alloced = NULL }
	};

	return kmem_init(2, mmap);
}
