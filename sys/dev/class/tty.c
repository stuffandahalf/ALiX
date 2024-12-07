#include <stddef.h>
#include <stdint.h>

#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/dev/class/tty.h>

int
tty_init(dev_t device, struct tty_util *utils)
{
	if (!(device->config = kalloc(sizeof(struct tty_state)))) {
		return 1;
	}

	return 0;
}