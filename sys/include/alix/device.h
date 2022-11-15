#ifndef ALIX_DEVICE_H
#define ALIX_DEVICE H 1

struct bus;
struct dev;

struct device {
	const char *name; /*device instance name */
	struct device *parent; /* pointer to bus used by driver for device io */
	struct dev *driver; /* pointer to driver used for handling file io */
	unsigned int channel; /* channel of this device on the driver */
	void *config; /* driver-specific configuration object */
};

#endif /* ALIX_DEVICE_H */
