#include <stdint.h>
#include <stddef.h>
#include <alix/bus.h>
#include <alix/dev.h>
#include <alix/device.h>

extern struct dev ns8250;
extern struct bus x86_ioport;

struct device early_console = {
	NULL,
	0x3f8,
	&x86_ioport,
	&ns8250,
	NULL
};

int
init_serial(uint16_t port)
{
	return ns8250.open(&early_console, 0);
}

void
put_serial(uint16_t port, char c)
{
	//~ while (!(inb(port + 5) & 0x20));
	//~ outb(port, c);
	early_console.driver->write(&early_console, c);
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
