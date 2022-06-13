#ifndef ALIX_BUS_H
#define ALIX_BUS_H 1

#define BUS_PAYLOAD_SIZE_8 1
#define BUS_PAYLOAD_SIZE_16 2
#define BUS_PAYLOAD_SIZE_32 4
#define BUS_PAYLOAD_SIZE_ARRAY 0

struct device;

typedef unsigned long int bus_target_t;

struct bus {
	struct bus *parent;
	struct bus *children;
	struct device *devices;
	const char *name;
	//~ int (*enumerate)(struct bus *bus);
	int (*send)(bus_target_t target, int size, ...);
	int (*receive)(bus_target_t target, int size, ...);
};

#endif /* ALIX_BUS_H */

