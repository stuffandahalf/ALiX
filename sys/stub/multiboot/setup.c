#include <multiboot.h>
#include <alix/memblk.h>

static unsigned char *framebuffer = (unsigned char *)0xb8000;

void print(unsigned long int);

void
setup32(multiboot_info_t *mbd)
{
	//print(mbd->
	const char *str = (const char *)mbd->boot_loader_name;
	const char *c;
	for (c = str; *c != '\0'; c++) {
		*framebuffer++ = *c;
		*framebuffer++ = 0xf0;
	}
	print(mbd->mmap_length);
}

void
print(unsigned long int num)
{
	unsigned char n = '0' + (num % 10);
	num /= 10;
	if (num > 0) {
		print(num);
	}
	*framebuffer++ = n;
	*framebuffer++ = 0xf0;
}
