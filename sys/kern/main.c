#include <stddef.h>
#include <stdint.h>

unsigned char *const framebuffer = (unsigned char *)0xb8000;

extern const size_t bus_root_count;
void printul(uint32_t, uint8_t);

void
main(void)
{
	const char *str = "Hello kernel world!";
	const char *c;
	int i = 0;
	for (c = str; *c != '\0'; c++) {
		framebuffer[i++] = *c;
		framebuffer[i++] = 0xf0;
	}

	printul(bus_root_count, 10);

	/* enumerate/initialize available devices */
	/* ensure interrupts are set up */
	/* mount root device */
	/* mount devtab? */
	/* set up process table */
	/* fork/exec /bin/init */
}
