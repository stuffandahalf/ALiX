#include <alix/bus.h>

static int pci_enumerate(void);
static int pci_read(bus_target_t target, struct bus_message *message);
static int pci_write(bus_target_t target, struct bus_message *message);

struct bus pci_bus = {
	"pci",
	pci_enumerate,
	pci_read,
	pci_write
};

static int
pci_enumerate(void)
{
	return 0;
}

static int
pci_read(bus_target_t target, struct bus_message *message)
{
	return 0;
}

static int
pci_write(bus_target_t target, struct bus_message *message)
{
	return 0;
}
