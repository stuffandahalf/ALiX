#include <stddef.h>
#include <stdint.h>
#include <multiboot.h>
#include <alix/device.h>
#include <alix/mem.h>
//#include <alix/x86/bios.h>

extern int init_serial(void);
extern void put_serial(uint8_t c);
extern int get_serial(void);
extern void write_serial(const char *str);

extern void printl(int32_t num, uint8_t base);
extern void printul(uint32_t num, uint8_t base);

static int init_mmap(struct multiboot_info *mbd);

extern struct device early_console;

void
setup32(multiboot_info_t *mbd)
{
	int i;
	//~ void *a, *b;

	if (init_serial()) {
		return;
	}
	write_serial("Hello Serial World!\n");

	write_serial((const char *)mbd->boot_loader_name);
	put_serial('\n');
	printl(mbd->mmap_length, 10);
	put_serial('\n');
	if (init_mmap(mbd)) {
		write_serial("Failed to initialize memory\n");
		return;
	}
	write_serial("SURVIVED\n");

	write_serial("MEMORY LAYOUT\n");
	kmem_avail(1);

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


#define PUSH(T, var) { \
	T *sp; \
	__asm__ __volatile__ ( \
		"subl %1, %%esp\n\t" \
		"movl %%esp, %0\n\t" \
		: "=r"(sp) \
		: "r"(sizeof(T)) \
	); \
	*sp = var; \
}

#define PUSHW(word) __asm__ __volatile__ ("pushw %0\n\t" : : "r"(word))
#define PUSHL(longword) __asm__ __volatile__ ("pushl %0\n\t" : : "r"(longword))

#define POP(T, var) { \
	T *sp; \
	__asm__ __volatile__ ( \
		"movl %%esp, %0\n\t" \
		: "=r"(sp) \
	); \
	var = *sp; \
	__asm__ __volatile__ ( \
		"addl %0, %%esp\n\t" \
		: \
		: "r"(sizeof(T)) \
	); \
}
#define POPW(word) __asm__("popw %0\n\t" : "=r"(word))
#define POPL(longword) __asm__("popl %0\n\t" : "=r"(longword))

static int
init_mmap(multiboot_info_t *mbd)
{
	int ret;
	long int i, j;
	uintptr_t adjust;
	size_t entries = 0, mmap_sz;
	unsigned short int *mmap_entry_words;
	struct multiboot_mmap_entry *mb_mmap;
	struct mmap_entry blk, *mmap;

	if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
		mb_mmap = (void *)mbd->mmap_addr;
		for (i = 0; i < mbd->mmap_length; i++) {
			blk.start = (void *)(uintptr_t)mb_mmap->addr;
			blk.length = mb_mmap->len;
			switch (mb_mmap->type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				blk.type = MEMORY_TYPE_FREE;
				//~ printl(PORT, blk.length, 10);
				//~ write_serial(PORT, "available bytes");
				break;
			case MULTIBOOT_MEMORY_RESERVED:
			case MULTIBOOT_MEMORY_BADRAM:
				//~ write_serial(PORT, "unavailable");
				blk.type = MEMORY_TYPE_UNAVAILABLE;
				break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				blk.type = MEMORY_TYPE_RECLAIMABLE;
				break;
			case MULTIBOOT_MEMORY_NVS:
				//~ write_serial(PORT, "TODO: Figure out NVS memory type");
				break;
			default:
				blk.type = 0;
			}
			if (blk.type) {
				// store for later
				PUSH(struct mmap_entry, blk);
				entries++;
			}
			mb_mmap = ((void *)mb_mmap) + mb_mmap->size + offsetof(struct multiboot_mmap_entry, addr);
		}
	} else if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
		blk.start = (void *)0;
		blk.length = mbd->mem_lower;
		blk.type = MEMORY_TYPE_FREE;
		PUSH(struct mmap_entry, blk);
		entries++;

		blk.start = (void *)0x100000;
		blk.length = mbd->mem_upper;
		blk.type = MEMORY_TYPE_FREE;
		PUSH(struct mmap_entry, blk);
		entries++;
	} else {
		return 1;
	}

	/* set current stack pointer as temporary mmap */
	__asm__ __volatile__ (
		"movl %%esp, %0\n\t"
		: "=r"(mmap)
	);
	mmap_sz = sizeof(struct mmap_entry) * entries;

	/* align stack to 4-byte boundary */
	if ((adjust = mmap_sz % sizeof(uint32_t))) {
		__asm__ __volatile__ (
			"subl %0, %%esp\n\t"
			:
			: "m"(adjust)
		);
	}

	ret = kmem_init(mmap, entries);
	/* free temporary mmap */
	__asm__ __volatile__ (
		"addl %0, %%esp\n\t"
		:
		: "r"((uintptr_t)mmap_sz + adjust)
	);
	if (ret) {
		write_serial("Failed to initialize system memory map\n");
		return 1;
	}

	write_serial("initialized memory map\n");

	return 0;
}
