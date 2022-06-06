#ifndef ALIX_BUS_H
#define ALIX_BUS_H 1

#include <alix/device.h>

struct bus {
	struct bus *parent;
	struct bus *children;
	struct device *devices;
	const char *name;
	int (*enumerate)(struct bus *bus);
	void (*send)(struct bus *bus, void *payload);
	void (*receive)(struct bus *bus, void *response);
};

#endif /* ALIX_BUS_H */

