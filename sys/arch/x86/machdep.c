#include <stddef.h>
#include <alix/bus.h>

extern struct bus x86_ioport;

struct bus bus_root[] = {
	&x86_ioport,
	NULL
};
