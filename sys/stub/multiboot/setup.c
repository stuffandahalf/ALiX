#include <multiboot.h>
#include <alix/mem.h>
//#include <alix/x86/bios.h>

#include <stddef.h>

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
}

static int
init_mmap(multiboot_info_t *mbd)
{
	long int i, j;
	size_t entries = 0;
	unsigned short int *mmap_entry_words;
	struct multiboot_mmap_entry *mb_mmap;
	struct mmap_entry blk;
	extern struct mmap_entry *mmap;

	if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
		mb_mmap = (void *)mbd->mmap_addr;
		for (i = 0; i < mbd->mmap_length; i++) {
			blk.start = (void *)mb_mmap->addr;
			blk.length = mb_mmap->len;
			switch (mb_mmap->type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				blk.type = MEMORY_TYPE_FREE;
				printl(PORT, blk.length, 10);
				write_serial(PORT, "available bytes");
				break;
			case MULTIBOOT_MEMORY_RESERVED:
			case MULTIBOOT_MEMORY_BADRAM:
				write_serial(PORT, "unavailable");
				blk.type = MEMORY_TYPE_UNAVAILABLE;
				break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				blk.type = MEMORY_TYPE_RECLAIMABLE;
				break;
			case MULTIBOOT_MEMORY_NVS:
				write_serial(PORT, "TODO: Figure out NVS memory type");
				break;
			default:
				blk.type = 0;
			}
			if (blk.type) {
				// store for later
				mmap_entry_words = (unsigned short int *)&blk;
				for (j = sizeof(struct mmap_entry) / 2 - 1; j >= 0; j--) {
					__asm__ (
						"pushw %0\n\t"
						:
						: "r"(mmap_entry_words[j])
					);
				}
				entries++;
			}
			mb_mmap = ((void *)mb_mmap) + mb_mmap->size + offsetof(struct multiboot_mmap_entry, addr);
		}
	} else if (mbd->flags & MULTIBOOT_INFO_MEMORY) {

	} else {
		return 1;
	}

	/* set current stack pointer as temporary mmap */
	__asm__ (
		"movl %%esp, %0\n\t"
		: "=r"(mmap)
	);

	/* use temporary mmap to allocate permanent location */
	mmap = kalloc(sizeof(struct mmap_entry) * entries);
	if (!mmap) {
		write_serial(PORT, "Failed to allocate memory for mmap");
		return 1;
	}



	/* free temporary mmap */
	__asm__ (
		"addl %0, %%esp\n\t"
		:
		: "r"(sizeof(struct mmap_entry) * entries)
	);

	return 0;
}
