#include <stddef.h>
#include <alix/bus.h>

extern struct bus x86_ioport;

struct bus *bus_root[] = {
	&x86_ioport,
	NULL
};
const size_t bus_root_count = sizeof(bus_root) / sizeof(struct bus *);

