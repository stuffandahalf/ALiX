#include <stddef.h>
#include <stdint.h>

#include <alix/log.h>

unsigned char *const framebuffer = (unsigned char *)0xb8000;

// extern const size_t bus_root_count;

extern void anounce(void);

void
main(void)
{
	const char *str = "Hello kernel world!";
	const char *c;
	int i = 0;
	// for (c = str; *c != '\0'; c++) {
	// 	framebuffer[i++] = *c;
	// 	framebuffer[i++] = 0xf0;
	// }
	anounce();
	_klog_flush();

	// kloglu(bus_root_count, 10);

	/* enumerate/initialize available devices */
	/* ensure interrupts are set up */
	/* mount root device */
	/* mount devtab? */
	/* set up process table */
	/* fork/exec /bin/init */
}
