#ifndef ALIX_DEVICE_H
#define ALIX_DEVICE H 1

struct bus;
struct dev;

struct device {
	const char *name;
	unsigned long int base; /* base address / identifer */
	struct bus *bus; /* pointer to bus used by driver for device io */
	struct dev *driver; /* pointer to driver used for handling file io */
	void *config; /* driver-specific configuration object */
};

#endif /* ALIX_DEVICE_H */
