#include <stddef.h>
#include <stdint.h>
#include <multiboot.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/log.h>
#include <arch/log.h>

static int init_mmap(struct multiboot_info *mbd);

//~ extern struct device early_console;

int
memtest(size_t n, size_t esz)
{
	void *ptrs[n];
	size_t before, middle, after;
	ssize_t i;

	before = kmem_avail(0);

	for (i = 0; i < n; i++) {
		ptrs[i] = kalloc(esz);
	}

	middle = kmem_avail(0);
	for (i = n - 1; i >= 0; i--) {
		kfree(ptrs[i]);
	}

	after = kmem_avail(0);

	if (after != before) {
		klogs("MEMORY NOT FREED\nBEFORE: ");
		kloglu(before, 10);
		klogs("  MIDDLE: ");
		kloglu(middle, 10);
		klogs("  AFTER: ");
		kloglu(after, 10);
		klogc('\n');

		kmem_avail(1);
	}
	return after != before;
}

void
setup32(multiboot_info_t *mbd)
{
	int i;

	// klogs((const char *)mbd->boot_loader_name);
	// klogc('\n');
	// klogld(mbd->mmap_length, 10);
	// klogc('\n');

	if (init_mmap(mbd)) {
		klogs("Failed to initialize memory\n");
		return;
	}
	// _klog_flush();
	// memtest(64, 1024 * 1024);
	// memtest(64, 2 * 1024 * 1024);

	// klogs("MEMORY LAYOUT\n");
	// kloglu(kmem_avail(1), 10);
	// klogc('\n');
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
	}

	return 0;
}

static int
init_mmap_simple(size_t lo, size_t hi)
{
	struct mmap_entry mmap[] = {
		{ .type = MEMORY_TYPE_FREE, .start = (void *)0, .length = lo * 1024, .free = NULL, .alloced = NULL },
		{ .type = MEMORY_TYPE_FREE, .start = (void *)0x100000, .length = hi * 1024, .free = NULL, .alloced = NULL }
	};
	uintptr_t reserved[][2] = {
		{ 0x0, 0x500 }
	};

	return kmem_init(sizeof(mmap) / sizeof(struct mmap_entry), mmap, sizeof(reserved) / sizeof(uintptr_t *), reserved);
}
