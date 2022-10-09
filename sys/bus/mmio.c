#include <alix/bus.h>

static int mmio_read(bus_target_t target, struct bus_message *message);
static int mmio_write(bus_target_t target, struct bus_message *message);

struct bus mmio_bus = {
	"mmio",
	NULL,
	mmio_read,
	mmio_write
};

static int
mmio_read(bus_target_t target, struct bus_message *message)
{
	return 0;
}

static int
mmio_write(bus_target_t target, struct bus_message *message)
{
	return 0;
}
