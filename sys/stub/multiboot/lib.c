#include <stdint.h>
#include <stddef.h>
//~ #include <alix/bus.h>
#include <alix/dev.h>
#include <alix/device.h>

#if 0

#define PORT 0x3f8

extern struct dev ns8250;
//~ extern struct bus x86_isa_bus;

struct device early_console = {
	NULL,
	PORT,
	&x86_isa_bus,
	&ns8250,
	NULL
};

int
init_serial(void)
{
	return ns8250.open(&early_console, 0);
}

void
put_serial(char c)
{
	early_console.driver->write(&early_console, c);
}

int
get_serial(void)
{
	return early_console.driver->read(&early_console);
}

void
write_serial(const char *str)
{
	const char *c;
	for (c = str; *c != '\0'; c++) {
		put_serial(*c);
	}
}

void
printul(uint32_t num, uint8_t base)
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
			put_serial('0' + d);
		} else {
			put_serial('A' + d - 10);
		}
		mun /= base;
	}
}

void
printl(int32_t num, uint8_t base)
{
	if (num < 0) {
		put_serial('-');
		num *= -1;
	}
	printul(num, base);
}

#endif
