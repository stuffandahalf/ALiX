#include <stddef.h>
#include <stdint.h>

#include <alix/config.h>
#include <alix/log.h>
#include <alix/startup.h>

unsigned char *const framebuffer = (unsigned char *)0xb8000;

void
main(void)
{
	// const char *str = "Hello kernel world!";
	// const char *c;
	// int i = 0;
	// for (c = str; *c != '\0'; c++) {
	// 	framebuffer[i++] = *c;
	// 	framebuffer[i++] = 0xf0;
	// }
	anounce();

	init_dev();
	// init_proc();
	init_vfs();
	_klog_flush();

	// kloglu(bus_root_count, 10);

	/* enumerate/initialize available devices */
	/* ensure interrupts are set up */
	/* mount root device */
	/* mount devtab? */
	/* set up process table */
	/* fork/exec /bin/init */
}
