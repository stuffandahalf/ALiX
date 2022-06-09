#ifndef __ALIX_DEV_H
#define __ALIX_DEV_H 1

typedef struct device *dev_t;

struct dev {
	const char *name;
	void (*open)(dev_t device, int flags);
	void (*read)(dev_t device);
	void (*write)(dev_t device);
	void (*ioctl)(dev_t device);
};

#endif /* __ALIX_DEV_H */