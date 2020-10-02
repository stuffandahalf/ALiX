#include <platform/init.h>
#include <platform/early_console.h>
#include <alix/copyright.h>
#include <alix/version.h>

void kernel_main(void)
{
	struct console cons;

	platform_init();
	console_init(&cons);
	cons.clear();
	cons.print.s("Booting ALiX " ALIX_KERNEL_VERSION "\n");
	cons.print.s(copyright_str);
}

