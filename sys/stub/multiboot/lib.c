#include <stdint.h>

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t byte);

int
init_serial(uint16_t port)
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
put_serial(uint16_t port, char c)
{
	while (!(inb(port + 5) & 0x20));
	outb(port, c);
}

void
write_serial(uint16_t port, const char *str)
{
	const char *c;
	for (c = str; *c != '\0'; c++) {
		put_serial(port, *c);
	}
	put_serial(port, '\r');
	put_serial(port, '\n');
}

void
printul(uint16_t port, uint32_t num, uint8_t base)
{
	unsigned char d;
	long int i, count = 0;
	unsigned long int mun = 0;

	/* invert number using mun as digit stack */

	do {
		mun += num % base;
		num /= base;
		count++;
		if (num) {
			mun *= base;
		}
	} while (num);
	/* print reverse order digits from mun */
	for (i = 0; i < count; i++) {
		d = mun % base;
		if (d < 10) {
			put_serial(port, '0' + d);
		} else {
			put_serial(port, 'A' + d - 10);
		}
		mun /= base;
	}

	put_serial(port, '\r');
	put_serial(port, '\n');
}

void
printl(uint16_t port, int32_t num, uint8_t base)
{
	if (num < 0) {
		put_serial(port, '-');
		num *= -1;
	}
	printul(port, num, base);
}
