#ifndef ALIX_DEV_H
#define ALIX_DEV_H 1

#include <stddef.h>

typedef struct device *dev_t;

struct dev {
	/* driver name */
	const char *name;

	/* driver registration functions */
	void (*attach)(dev_t parent);
	void (*detach)(dev_t parent);

	/* driver instance functions */
	int (*open)(dev_t device, int flags);
	int (*close)(dev_t device);
	int (*read)(dev_t device, void *buffer, size_t n);
	int (*write)(dev_t device, void *buffer, size_t n);
	void (*ioctl)(dev_t device);
};

dev_t create_dev(struct dev *driver);
void destroy_dev(dev_t d);

#endif /* ALIX_DEV_H */
