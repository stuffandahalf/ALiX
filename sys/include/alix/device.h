#ifndef ALIX_DEVICE_H
#define ALIX_DEVICE H 1

struct bus;
struct dev;

struct device {
	const char *name;
	struct bus *bus;
	struct dev *driver;
};

#endif /* ALIX_DEVICE_H */

