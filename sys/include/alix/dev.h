#ifndef ALIX_DEV_H
#define ALIX_DEV_H 1

#include <stddef.h>
#include <alix/util.h>

typedef struct device *dev_t;

#define RESOURCE_REQUEST_CHANNELS 1
#define RESOURCE_REQUEST_CHANNEL_SIZE 2

struct resource_request {
	int type;
	union {
		unsigned int nchannels;
		struct {
			unsigned int channel;
			unsigned int size;
		} channelsz;
	};
};

struct dev {
	/* driver name */
	const char *name;
	LIST(dev_t) instances;

	/* driver registration functions */
	int (*attach)(dev_t parent);
	void (*detach)(dev_t parent);

	/* driver instance functions */
	int (*open)(dev_t device, unsigned int channel, int flags);
	int (*close)(dev_t device, unsigned int channel);
	int (*read)(dev_t device, unsigned int channel, void *buffer, size_t n);
	int (*write)(dev_t device, unsigned int channel, void *buffer, size_t n);
	void (*ioctl)(dev_t device);

	int (*res_req)(dev_t device, size_t nrequests, const struct resource_request *requests);
};

dev_t create_dev(struct dev *driver, unsigned int nchannels, dev_t parent);
void destroy_dev(dev_t device);

#define DEV_PARENT_OPEN(dev, channel, flags) (dev)->parent->driver->open((dev)->parent, (channel), (flags))
#define DEV_PARENT_WRITE(dev, channel, buf, n) (dev)->parent->driver->write((dev)->parent, (channel), (buf), (n))
#define DEV_PARENT_READ(dev, channel, buf, n) (dev)->parent->driver->read((dev)->parent, (channel), (buf), (n))
#define DEV_PARENT_IOCTL(dev, channel) (dev)->parent->driver->ioctl((dev)->parent, (channel));

#endif /* ALIX_DEV_H */
