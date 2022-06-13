#include <stddef.h>
#include <alix/bus.h>

extern struct bus *bus_root[];

int
dev_init(void)
{
	struct bus **b;

	for (b = bus_root; *b != NULL; b++) {

	}
}
