#include <multiboot.h>
#include <alix/memblk.h>
//#include <alix/x86/bios.h>

#define PORT 0x3f8

extern unsigned char inb(unsigned short int port);
extern void outb(unsigned short int port, unsigned char byte);

int init_serial(unsigned short int port);
void put_serial(unsigned short int port, char c);
void write_serial(unsigned short int port, const char *str);

void printl(unsigned short int port, long int num, unsigned char base);
void printul(unsigned short int port, unsigned long int num, unsigned char base);

void
setup32(multiboot_info_t *mbd)
{
	if (init_serial(PORT)) {
		return;
	}
	write_serial(PORT, "Hello Serial World!");

	//print(mbd->
	const char *str = (const char *)mbd->boot_loader_name;
	write_serial(PORT, str);
	printl(PORT, mbd->mmap_length, 10);
}

void
printl(unsigned short int port, long int num, unsigned char base)
{
	if (num < 0) {
		put_serial(port, '-');
		num *= -1;
	}
	printul(port, num, base);
}

void
printul(unsigned short int port, unsigned long int num, unsigned char base)
{
	unsigned char d;
	long int i, count = 0;
	unsigned long int mun = 0;

	/* invert number using mun as digit stack */
	while (num) {
		mun += num % base;
		num /= base;
		count++;
		if (num) {
			mun *= base;
		}
	}
	/* print reverse order digits from mun */
	for (int i = 0; i < count; i++) {
		d = mun % base;
		if (d < 10) {
			put_serial(port, '0' + d);
		} else {
			put_serial(port, 'A' + d);
		}
		mun /= base;
	}

	put_serial(port, '\n');
}

int
init_serial(unsigned short int port)
{
	outb(port + 1, 0x00);
	outb(port + 3, 0x80);
	outb(port + 0, 0x03);
	outb(port + 1, 0x00);
	outb(port + 3, 0x03);
	outb(port + 2, 0xc7);
	outb(port + 4, 0x0b);
	outb(port + 4, 0x1e);

	outb(port + 0, 0xae);
	if (inb(port + 0) != 0xae) {
		return 1;
	}
	outb(port + 4, 0x0f);
	return 0;
}

void
put_serial(unsigned short int port, char c)
{
	while (!(inb(port + 5) & 0x20));
	outb(port, c);
}

void
write_serial(unsigned short int port, const char *str)
{
	const char *c;
	for (c = str; *c != '\0'; c++) {
		put_serial(port, *c);
	}
	put_serial(port, '\n');
}
