#ifndef ALIX_DEV_H
#define ALIX_DEV_H 1

#include <stddef.h>
#include <alix/util.h>

typedef struct device *dev_t;

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
	int (*ioctl)(dev_t device, unsigned long int request, ...);
};

#define DEV_FATTACH (1 << 0)
#define DEV_FDETACH (1 << 1)
#define DEV_FOPEN (1 << 2)
#define DEV_FCLOSE (1 << 3)
#define DEV_FREAD (1 << 4)
#define DEV_FWRITE (1 << 5)
#define DEV_FIOCTL (1 << 6)

#define DEV_FINIT (DEV_FATTACH | DEV_FDETACH)
#define DEV_FOPENCLOSE (DEV_FOPEN | DEV_FCLOSE)
#define DEV_FREADWRITE (DEV_FREAD | DEV_FWRITE)
#define DEV_FALL (DEV_FINIT | DEV_FOPENCLOSE | DEV_FREADWRITE | DEV_FIOCTL)

// #define DEV_FEATURES(drvnm, features) static const unsigned long int drvnm ## _features = (features)
#define DEV_FEATURE_TEST(features, feature) ((features) & (feature))
#define DEV_INIT(drvnm, features) { \
	.name = (#drvnm), \
	.instances = LIST_INIT, \
	 \
	.attach = DEV_FEATURE_TEST((features), DEV_FATTACH) ? (drvnm ## _attach) : NULL, \
	.detach = DEV_FEATURE_TEST((features), DEV_FDETACH) ? (drvnm ## _detach) : NULL, \
	 \
	.open = DEV_FEATURE_TEST((features), DEV_FOPEN) ? (drvnm ## _open) : NULL, \
	.close = DEV_FEATURE_TEST((features), DEV_FCLOSE) ? (drvnm ## _close) : NULL, \
	 \
	.read = DEV_FEATURE_TEST((features), DEV_FREAD) ? (drvnm ## _read) : NULL, \
	.write = DEV_FEATURE_TEST((features), DEV_FWRITE) ? (drvnm ## _write) : NULL, \
	.ioctl = DEV_FEATURE_TEST((features), DEV_FIOCTL) ? (drvnm ## _ioctl) : NULL, \
}

dev_t create_dev(struct dev *driver, unsigned int nchannels, dev_t parent);
void destroy_dev(dev_t device);

#define DEV_PARENT_CALL(dev, func, ...) ((dev)->parent->driver->func \
	? dev->parent->driver->func((dev)->parent, __VA_ARGS__) \
	: (0))
// #define DEV_PARENT_OPEN(dev, channel, flags) (dev)->parent->driver->open((dev)->parent, (channel), (flags))
#define DEV_PARENT_OPEN(dev, channel, flags) DEV_PARENT_CALL((dev), open, (channel), (flags))
#define DEV_PARENT_CLOSE(dev, channel) DEV_PARENT_CALL((dev), close, (channel))
#define DEV_PARENT_WRITE(dev, channel, buf, n) DEV_PARENT_CALL((dev), write, (channel), (buf), (n))
#define DEV_PARENT_READ(dev, channel, buf, n) DEV_PARENT_CALL((dev), read, (channel), (buf), (n))
#define DEV_PARENT_IOCTL(dev, channel, ...) DEV_PARENT_CALL((dev), ioctl, (channel), __VA_ARGS__)

#endif /* ALIX_DEV_H */
