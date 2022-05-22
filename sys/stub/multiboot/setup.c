#include <multiboot.h>
#include <alix/mem.h>
//#include <alix/x86/bios.h>

#include <stddef.h>
#include <stdint.h>

#define PORT 0x3f8

extern int init_serial(unsigned short int port);
extern void put_serial(unsigned short int port, char c);
extern void write_serial(unsigned short int port, const char *str);

extern void printl(unsigned short int port, long int num, unsigned char base);
extern void printul(unsigned short int port, unsigned long int num, unsigned char base);

static int init_mmap(struct multiboot_info *mbd);

void
setup32(multiboot_info_t *mbd)
{
	int i;

	if (init_serial(PORT)) {
		return;
	}
	write_serial(PORT, "Hello Serial World!");

	write_serial(PORT, (const char *)mbd->boot_loader_name);
	printl(PORT, mbd->mmap_length, 10);
	if (init_mmap(mbd)) {
		write_serial(PORT, "Failed to initialize memory");
		return;
	}
	write_serial(PORT, "SURVIVED");

	printl(PORT, INTN_MIN(8), 10);
	printul(PORT, INTN_MAX(8), 10);
	printul(PORT, UINTN_MAX(8), 10);
}

#define PUSH(T, var) { \
	T *sp; \
	__asm__( \
		"subl %1, %%esp\n\t" \
		"movl %%esp, %0\n\t" \
		: "=r"(sp) \
		: "r"(sizeof(T)) \
	); \
	*sp = var; \
}

#define PUSHW(word) __asm__("pushw %0\n\t" : : "r"(word))
#define PUSHL(longword) __asm__("pushl %0\n\t" : : "r"(longword))

#define POP(T, var) { \
	T *sp; \
	__asm__( \
		"movl %%esp, %0\n\t" \
		: "=r"(sp) \
	); \
	var = *sp; \
	__asm__( \
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
	size_t entries = 0, mmap_sz;
	unsigned short int *mmap_entry_words;
	struct multiboot_mmap_entry *mb_mmap;
	struct mmap_entry blk, *mmap;

	if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
		mb_mmap = (void *)mbd->mmap_addr;
		for (i = 0; i < mbd->mmap_length; i++) {
			blk.start = (void *)mb_mmap->addr;
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
	__asm__(
		"movl %%esp, %0\n\t"
		: "=r"(mmap)
	);
	mmap_sz = sizeof(struct mmap_entry) * entries;

	/* align stack to 4-byte boundary */
	if (mmap_sz % sizeof(int)) {
		__asm__(
			"subl %0, %%esp\n\t"
			:
			: "r"(mmap_sz % sizeof(int))
		);
	}

	ret = init_sysmem(mmap, entries);
	/* free temporary mmap */
	__asm__ (
		"addl %0, %%esp\n\t"
		:
		: "r"(mmap_sz + mmap_sz % sizeof(int))
	);
	if (ret) {
		write_serial(PORT, "Failed to initialize system memory map");
		return 1;
	}

	write_serial(PORT, "initialized memory map");

	return 0;
}
